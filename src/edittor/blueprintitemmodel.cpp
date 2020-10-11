#include "blueprintitemmodel.h"

#include <QDebug>

#ifndef Q_MOC_RUN
//#include "common/log.h"
#endif

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void doTreeItemUpdate( BlueprintItemModel::TreeItem* pRootItem, BlueprintItemModel& model )
{
    //LOG_PROFILE_BEGIN( tree_update );
    ASSERT( pRootItem );
    pRootItem->update( model );
    //LOG_PROFILE_END( tree_update );
}
BlueprintItemModel::TreeItem* createRootTreeItem( Blueprint::Node::PtrCst pNode )
{
    return new BlueprintItemModel::TreeItem( pNode );
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
BlueprintItemModel::BlueprintItemModel(QObject *parent) :
        QAbstractItemModel(parent)
{
}

QModelIndex BlueprintItemModel::getNodeIndex( Blueprint::Node::PtrCst pNode ) const
{
    QModelIndex result;
    if( pNode )
    {
        if( m_pRoot.get() )
        {
            if( Blueprint::Node::PtrCst pParent = pNode->getParent() )
                result = index( pNode->getIndex(), 0, getNodeIndex( pParent ) );
            else
                result = QModelIndex();
        }
    }
    return result;
}

Blueprint::Node::PtrCst BlueprintItemModel::getIndexNode( const QModelIndex& index ) const
{
    Blueprint::Node::PtrCst pNode;
    if( TreeItem* pItem = index.isValid() ? static_cast< TreeItem* >( index.internalPointer() ) : m_pRoot.get() )
        pNode = pItem->getNode();
    return pNode;
}

void BlueprintItemModel::OnBlueprintUpdate()
{
    if( m_pRoot.get() )
        doTreeItemUpdate( m_pRoot.get(), *this );
}

void BlueprintItemModel::OnBlueprintSelected( BlueprintMsg msg )
{
    qDebug() << "BlueprintItemModel::OnBlueprintSelected: " << msg.pNode->getName().c_str();
    if( m_pRoot.get() && ( m_pRoot->rowCount() > 0 ) )
    {
        beginRemoveRows( QModelIndex(), 0, m_pRoot->rowCount() - 1 );
        m_pRoot.reset();
        endRemoveRows();
    }

    m_pNode = msg.pNode;

    if( msg.pNode )
    {
        m_pRoot.reset( createRootTreeItem( msg.pNode ) );
        doTreeItemUpdate( m_pRoot.get(), *this );
    }
}

Qt::ItemFlags BlueprintItemModel::flags(const QModelIndex &index) const
{
    //qDebug() << "BlueprintItemModel::flags " << index;
    if (!index.isValid())
        return 0;
    return QAbstractItemModel::flags( index );
}

QModelIndex BlueprintItemModel::index(int row, int column, const QModelIndex& parentIndex ) const
{
    if( TreeItem* pParentItem = parentIndex.isValid() ?
                static_cast< TreeItem* >( parentIndex.internalPointer() ) : m_pRoot.get() )
    {
        if ( TreeItem* pChildItem = pParentItem->getChildAt( row ) )
            return createIndex( row, column, pChildItem );
    }
    return QModelIndex();
}

QModelIndex BlueprintItemModel::parent( const QModelIndex& childIndex ) const
{
    //qDebug() << "BlueprintItemModel::parent " << childIndex;
    if ( !childIndex.isValid() )
        return QModelIndex();

    if( TreeItem* pChildItem = static_cast< TreeItem* >( childIndex.internalPointer() ) )
    {
        if( TreeItem* pParentItem = pChildItem->getParent() )
        {
            if ( pParentItem == m_pRoot.get() )
                return QModelIndex();
            return createIndex( pParentItem->getRowNumber(), 0, pParentItem);
        }
    }
    return QModelIndex();
}

int BlueprintItemModel::rowCount(const QModelIndex &parent) const
{
    //qDebug() << "BlueprintItemModel::rowCount " << parent;
    if ( parent.column() > 0 )
        return 0;

    if( TreeItem *parentItem = parent.isValid() ? static_cast<TreeItem*>(parent.internalPointer()) : m_pRoot.get() )
        return parentItem->rowCount();
    else
        return 0;
}

int BlueprintItemModel::columnCount(const QModelIndex &parent) const
{
    //qDebug() << "BlueprintItemModel::columnCount " << parent;
    if( parent.isValid() )
    {
        if( TreeItem* pItem = static_cast<TreeItem*>( parent.internalPointer() ) )
            return pItem->columnCount();
    }
    return m_pRoot.get() ? m_pRoot->columnCount() : 0;
}

QVariant BlueprintItemModel::data(const QModelIndex &index, int role) const
{
    //qDebug() << "BlueprintItemModel::data " << index << " " << role;
    if( !index.isValid() )
        return QVariant();

    if( role != Qt::DisplayRole )
        return QVariant();

    if( TreeItem* pItem = static_cast<TreeItem*>( index.internalPointer() ) )
        return pItem->getData( index.column() );
    else
        return QVariant();
}

QVariant BlueprintItemModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
    //qDebug() << "BlueprintItemModel::headerData " << section << " " << orientation << " " << role;
    if( orientation == Qt::Horizontal && role == Qt::DisplayRole )
    {
        switch( section )
        {
            case 0:     return QVariant( "Item" );
            case 1:     return QVariant( "Value" );
            default:    return QVariant();
        }
    }

    return QVariant();
}




















