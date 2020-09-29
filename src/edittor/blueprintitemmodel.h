#ifndef BLUEPRINTITEMMODEL_H
#define BLUEPRINTITEMMODEL_H

#include <list>
#include <set>
#include <map>
#include <algorithm>
#include <memory>

#include <QAbstractItemModel>
#include <QDebug>

#ifndef Q_MOC_RUN

#include "common/assert_verify.hpp"
#include "common/stl.hpp"
#include "common/compose.hpp"
#include "common/tick.hpp"

#include "blueprint/node.h"
#endif

#include "messages.h"

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
class BlueprintItemModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    //////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////
    struct TreeItem
    {
        friend void doTreeItemUpdate( TreeItem*, BlueprintItemModel& );
        friend TreeItem* createRootTreeItem( Blueprint::Node::PtrCst );
    private:
        typedef std::vector< TreeItem* > Vector;
        typedef std::set< TreeItem* > Set;
        typedef std::map< std::string, TreeItem* > StringItemMap;
        typedef std::map< Blueprint::Node::PtrCstWeak, TreeItem* > PtrTreeItemMap;

        TreeItem( Blueprint::Node::PtrCst pNode, TreeItem* pParent = 0u )
            :   m_pNode( pNode ),
                m_pParent( pParent ),
                m_qName( QVariant( pNode->getName().c_str() ) ),
                m_qValue( QVariant( pNode->getStatement().c_str() ) )
        {

        }
    public:
        //public interface used by the BlueprintItemModel
        ~TreeItem()
        {
            generics::deleteAndClear( m_children );
        }

        QVariant getData( int iColumn ) const
        {
            switch( iColumn )
            {
                case 0: return m_qName;
                case 1: return m_qValue;
            }
            return QVariant();
        }
        TreeItem* getChildAt( int iRow ) const { return ( iRow < static_cast< int >( m_children.size() ) ) ? m_children[ iRow ] : 0u; }
        TreeItem* getParent() const { return m_pParent; }
        int columnCount() const { return 2; }
        int rowCount() const { return static_cast< int >( m_children.size() ); }
        int getRowNumber() const
        {
            if( m_pParent )
            {
                const int iRowNum = std::find( m_pParent->m_children.begin(),
                                               m_pParent->m_children.end(), this ) - m_pParent->m_children.begin();
                ASSERT( iRowNum < static_cast< int >( m_pParent->m_children.size() ) && m_pParent->m_children[ iRowNum ] == this );
                return iRowNum;
            }
            else
                return 0;
        }

        Blueprint::Node::PtrCst getNode() const { return m_pNode.lock(); }
    private:

        const Timing::UpdateTick& getTick() const { return m_lastUpdateTick; }

        struct CompareIters
        {
            bool operator()( PtrTreeItemMap::const_iterator i1, Blueprint::Node::PtrVector::const_iterator i2 ) const
            {
                return i1->first.lock() < *i2;
            }
            bool opposite( PtrTreeItemMap::const_iterator i1, Blueprint::Node::PtrVector::const_iterator i2 ) const
            {
                return *i2 < i1->first.lock();
            }
        };
        friend class CompareItersUpdate;
        struct CompareItersUpdate
        {
            bool operator()( PtrTreeItemMap::const_iterator i1, Blueprint::Node::PtrVector::const_iterator i2 ) const
            {
                ASSERT( i1->first.lock() == *i2 );
                return i1->second->getTick() < (*i2)->getLastModifiedTick();
            }
        };

        void update( BlueprintItemModel& model, const QModelIndex& parentModelIndex = QModelIndex() )
        {
            Blueprint::Node::PtrCst pNode = m_pNode.lock();
            ASSERT( pNode );
            if( !pNode ) return;
            m_lastUpdateTick.update();

            const QModelIndex modelIndex = m_pParent ? model.index( getRowNumber(), 0, parentModelIndex ) : QModelIndex();

            //calculate the required additions, removals and updates
            PtrTreeItemMap removals;
            Blueprint::Node::PtrSet additions;
            Set updates;
            {
                Blueprint::Node::PtrVector nodes = pNode->getChildren();
                std::sort( nodes.begin(), nodes.end() );
                generics::matchGetUpdates( m_nodeMap.begin(), m_nodeMap.end(), nodes.begin(), nodes.end(),
                                                CompareIters(), CompareItersUpdate(),
                                                generics::collect( removals, generics::deref< PtrTreeItemMap::const_iterator >() ),
                                                generics::collect( additions, generics::deref< Blueprint::Node::PtrVector::const_iterator >() ) ,
                                                generics::collect( updates, generics::second< PtrTreeItemMap::const_iterator >() ) );
            }

            //remove all the removals
            for( PtrTreeItemMap::iterator
                 i = removals.begin(),
                 iEnd = removals.end(); i!=iEnd; ++i )
            {
                //remove the treeitem
                TreeItem* pItem = i->second;
                m_nodeMap.erase( i->first );
                const int iRowNumber = pItem->getRowNumber();
                {
                    model.beginRemoveRows( modelIndex, iRowNumber, iRowNumber );
                    m_children.erase( m_children.begin() + iRowNumber );
                    delete pItem;
                    model.endRemoveRows();
                }
            }

            //process the additions, updates and moves
            int iRow = 0;
            for( Blueprint::Node::PtrVector::const_iterator
                 j = pNode->getChildren().begin(),
                 jEnd = pNode->getChildren().end(); j!=jEnd; ++j, ++iRow )
            {
                Blueprint::Node::Ptr pIter = *j;
                if( additions.find( pIter ) != additions.end() )
                {
                    //node needs to be added at this position
                    TreeItem* pNewTreeItem = new TreeItem( pIter, this );
                    m_nodeMap.insert( std::make_pair( Blueprint::Node::PtrCstWeak( pIter ), pNewTreeItem ) );
                    {
                        model.beginInsertRows( modelIndex, iRow, iRow );
                        m_children.insert( m_children.begin() + iRow, pNewTreeItem );
                        model.endInsertRows();
                    }
                }
                else
                {
                    ASSERT( iRow < static_cast< int >( m_children.size() ) );
                    TreeItem* pChildItem = m_children[ iRow ];
                    //if the nodes match at this position
                    if( pChildItem->m_pNode.lock() == pIter )
                    {
                        //then just test if need to update
                        if( updates.find( pChildItem ) != updates.end() )
                        {
                            //do the update
                            pChildItem->m_qName     = QVariant( pIter->getName().c_str() );
                            pChildItem->m_qValue    = QVariant( pIter->getStatement().c_str() );
                            model.dataChanged(  model.index( iRow, 0, modelIndex ),
                                                model.index( iRow, 2, modelIndex ) );
                        }
                    }
                    else
                    {
                        //move the treeItem to this position
                        PtrTreeItemMap::iterator iFind = m_nodeMap.find( pIter );
                        ASSERT( iFind != m_nodeMap.end() );
                        TreeItem* pSourceItem = iFind->second;
                        ASSERT( pSourceItem->m_pNode.lock() == pIter );
                        int iSource = pSourceItem->getRowNumber();
                        ASSERT( iSource > iRow );
                        {
                            model.beginMoveRows( modelIndex, iSource, iSource, modelIndex, iRow );
                            std::swap( *( m_children.begin() + iSource ), *( m_children.begin() + iRow ) );
                            pSourceItem->m_qName     = QVariant( pIter->getName().c_str() );
                            pSourceItem->m_qValue    = QVariant( pIter->getStatement().c_str() );
                            ASSERT( m_children[ iRow ] == pSourceItem );
                            model.endMoveRows();
                        }
                    }
                }
            }

            ASSERT( m_children.size() == pNode->getChildren().size() );
            Vector::iterator c = m_children.begin();
            for( Blueprint::Node::PtrVector::const_iterator
                 i = pNode->getChildren().begin(),
                 iEnd = pNode->getChildren().end(); i!=iEnd; ++i, ++c )
            {
                (*c)->update( model, modelIndex );
            }
        }

    private:
        TreeItem* m_pParent;
        Vector m_children;
        QVariant m_qName, m_qValue;

        Timing::UpdateTick m_lastUpdateTick;
        Blueprint::Node::PtrCstWeak m_pNode;
        PtrTreeItemMap m_nodeMap;
    };

    //////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////
    explicit BlueprintItemModel(QObject *parent = 0);

    QModelIndex getNodeIndex( Blueprint::Node::PtrCst pNode ) const;
    Blueprint::Node::PtrCst getIndexNode( const QModelIndex& index ) const;

    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    virtual QModelIndex parent(const QModelIndex &child) const;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
signals:

public slots:
    void OnBlueprintUpdate();
    void OnBlueprintSelected( BlueprintMsg msg );

private:
    Blueprint::Node::PtrCstWeak m_pNode;
    std::auto_ptr< TreeItem > m_pRoot;
};

#endif // BLUEPRINTITEMMODEL_H
