#include "blueprintview.h"

#include <QDebug>
#include <QScrollBar>
#include <QVector2D>
#include <QList>
#include <QFileDialog>
#include <QMessageBox>
#include <QProcessEnvironment>

#include <cmath>
#include <sstream>

#ifndef Q_MOC_RUN
#include <boost/optional.hpp>
#include "common/math.hpp"
#include "common/rounding.hpp"
#include "blueprint/factory.h"
#include "blueprint/property.h"
#endif

#include "blueprintselectionmodel.h"
#include "blueprintitemmodel.h"

#include "preview.h"
#include "property.h"


namespace Ed
{
    inline IShorthandStream& operator>>( IShorthandStream& is, QColor& data )
    {
        int red,green,blue,alpha;
        is >> red >> green >> blue >> alpha;
        data = QColor( red,green,blue,alpha );
        return is;
    }
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
BlueprintView::BlueprintView(QWidget *parent) :
    QGraphicsView(parent),
    m_v2ZoomLevel( 1.0f, 1.0f ),
    m_interactionMode( eNone ),
    m_selectTool( *this ),
    m_penTool( *this ),
    m_propertyTool( *this ),
    m_pActiveTool( &m_selectTool ),
    m_iQuantisation( 16 ),
    m_fDefaultZoom( 256.0f ),
    m_bInitialising( true )
{
    //setup the scene
    m_pBlueprintScene = new BlueprintScene;
    setScene( m_pBlueprintScene );
    CalculateOversizedSceneRect();
    SetZoom( m_v2ZoomLevel );
    CalculateRulerItems();
    
    //OnNewBlueprint() is called once mainwindow has finished initialising - m_pToolBox will then be set
    //so that defaults can be read in...
}

BlueprintView::~BlueprintView()
{
}

void BlueprintView::showEvent( QShowEvent* pEvent )
{
    QGraphicsView::showEvent( pEvent );
    VERIFY_RTE( m_pToolBox );
    if( m_bInitialising )
    {
        m_bInitialising = false;
        OnNewBlueprint();
    }
}

void BlueprintView::OnNewBlueprint()
{
    VERIFY_RTE( m_pToolBox );
    if( m_pToolBox )
    {
        m_pToolBox->getConfigValue( ".view.zoom.default", m_fDefaultZoom );
        m_pToolBox->getConfigValue( ".view.quantise.default", m_iQuantisation );
    }
    
    m_pActiveContext = 0u;
    m_pBlueprintEdit.reset();
    VERIFY_RTE( m_itemMap.empty() );
    VERIFY_RTE( m_specMap.empty() );

    Blueprint::Factory factory;
    //create a blank blueprint
    m_pBlueprint = factory.create( "NewBlueprint" );

    //and start a new edit session
    m_pBlueprintEdit = Blueprint::Edit::create( *this, m_pBlueprint );

    //the the initial context to the edit object
    m_pActiveContext = m_pBlueprintEdit.get();

    OnBlueprintSelected( BlueprintMsg( m_pBlueprint ) );
    
    OnWindowTitleModified( "NewBlueprint" );
    
    OnCmd_ZoomToAll();
}

void BlueprintView::OnLoadBlueprint()
{
    QProcessEnvironment environment = QProcessEnvironment::systemEnvironment();
    QString strDefaultPath = environment.value( "BLUEPRINT_TOOLBOX_PATH" );
    if( m_pBlueprintEdit && !m_pBlueprintEdit->getFilePath().empty() )
    {
        strDefaultPath = QString::fromUtf8( m_pBlueprintEdit->getFilePath().c_str() );
    }
    
    Blueprint::Site::Ptr pNewBlueprint;
    QString strFilePath =
            QFileDialog::getOpenFileName( this,
                tr( "Open Blueprint" ), 
                strDefaultPath,
                tr( "Blueprint Files (*.blu)" ) );
    if( !strFilePath.isEmpty() )
    {
        try
        {
            Blueprint::Factory factory;
            pNewBlueprint = factory.load( strFilePath.toStdString() );
            OnWindowTitleModified( strFilePath );
        }
        catch( ... )
        {
            QMessageBox::warning( this,
                                  tr( "Blueprint Edittor" ),
                                  "The blueprint failed to load correctly." );
        }
        if( pNewBlueprint )
        {
            m_pActiveContext = 0u;
            m_pBlueprintEdit.reset();
            VERIFY_RTE( m_itemMap.empty() );
            VERIFY_RTE( m_specMap.empty() );
            m_pBlueprint = pNewBlueprint;
            m_pBlueprintEdit = Blueprint::Edit::create( *this, m_pBlueprint, strFilePath.toStdString() );
            m_pActiveContext = m_pBlueprintEdit.get();

            OnBlueprintSelected( BlueprintMsg( m_pBlueprint ) );
        }
    }
    
    OnCmd_ZoomToAll();
}

void BlueprintView::OnSaveBlueprint()
{
    if( m_pBlueprintEdit && m_pBlueprint )
    {
        if( m_pBlueprintEdit->getFilePath().empty() )
            OnSaveAsBlueprint();
        else
        {
            try
            {
                Blueprint::Factory factory;
                factory.save( m_pBlueprint, m_pBlueprintEdit->getFilePath() );
                OnWindowTitleModified( QString::fromUtf8( m_pBlueprintEdit->getFilePath().c_str() ) );
            }
            catch( ... )
            {
                QMessageBox::warning( this,
                                      tr( "Blueprint Edittor" ),
                                      "The blueprint failed to save correctly." );
            }
        }
    }
}

void BlueprintView::OnSaveAsBlueprint()
{
    QProcessEnvironment environment = QProcessEnvironment::systemEnvironment();
    QString strDefaultPath = environment.value( "BLUEPRINT_TOOLBOX_PATH" );
    if( m_pBlueprintEdit && !m_pBlueprintEdit->getFilePath().empty() )
    {
        strDefaultPath = QString::fromUtf8( m_pBlueprintEdit->getFilePath().c_str() );
    }
    
    QString strFilePath =
            QFileDialog::getSaveFileName( this,
                tr( "Open Blueprint" ), 
                strDefaultPath,
                tr( "Blueprint Files (*.blu)" ) );
    if( !strFilePath.isEmpty() )
    {
        try
        {
            Blueprint::Factory factory;
            factory.save( m_pBlueprint, strFilePath.toStdString() );
            m_pBlueprintEdit->setFilePath( strFilePath.toStdString() );
            OnWindowTitleModified( strFilePath );
        }
        catch( ... )
        {
            QMessageBox::warning( this,
                                  tr( "Blueprint Edittor" ),
                                  "The blueprint failed to save correctly." );
        }
    }
}

void BlueprintView::OnCmd_Delete()
{
    ASSERT( m_pActiveContext );
    ASSERT( m_pActiveTool );
    m_pActiveTool->reset();
    m_pActiveContext->cmd_delete( getSelection() );
    OnBlueprintModified();
    invalidateScene();
}

void BlueprintView::OnCmd_Cut()
{
    ASSERT( m_pActiveContext );
    if( Blueprint::Site::Ptr pCut = boost::dynamic_pointer_cast< Blueprint::Site >(
        m_pActiveContext->cmd_cut( getSelection() ) ) )
    {
        m_pToolBox->add( "clipboard", pCut, true );
        OnClipboardAction();
    }
    OnBlueprintModified();
}

void BlueprintView::OnCmd_Copy()
{
    ASSERT( m_pActiveContext );
    if( Blueprint::Site::Ptr pCopy = boost::dynamic_pointer_cast< Blueprint::Site >(
        m_pActiveContext->cmd_copy( getSelection() ) ) )
    {
        m_pToolBox->add( "clipboard", pCopy, true );
        OnClipboardAction();
    }
}

void BlueprintView::OnCmd_Paste()
{
    if( Blueprint::Toolbox::Palette::Ptr pPalette = m_pToolBox->getPalette( "clipboard" ) )
    {
        if( Blueprint::Site::Ptr pClip = pPalette->getSelection() )
        {
            QPointF pos( 0.0f, 0.0f );
            QVector2D q = calculateQuantisation();
            m_pActiveContext->cmd_paste( pClip, pos.x(), pos.y(), q.x(), q.y() );
            OnBlueprintModified();
        }
    }
}

void BlueprintView::OnCmd_SelectAll()
{
    if( m_pActiveContext )
    {
        ASSERT( m_pActiveTool );
        SelectionSet selection;
        for( ItemMap::const_iterator i = m_itemMap.begin(),
             iEnd = m_itemMap.end(); i!=iEnd; ++i )
        {
            if( Selectable* pSelectable = Selection::glyphToSelectable( i->second ) )
            {
                if( pSelectable->isImage() && m_pActiveContext->canEdit( i->second, m_pActiveTool->getToolType(), m_toolMode ) )
                    selection.insert( i->second );
            }
        }
        if( getSelection() != selection )
            setSelected( selection );
        else
            setSelected( SelectionSet() );
    }
}

void BlueprintView::OnCmd_ZoomToAll()
{
    ASSERT( m_pActiveTool );
    QRectF rect( -m_fDefaultZoom / 2.0f, -m_fDefaultZoom / 2.0f, m_fDefaultZoom, m_fDefaultZoom );
    for( ItemMap::const_iterator i = m_itemMap.begin(),
         iEnd = m_itemMap.end(); i!=iEnd; ++i )
    {
        if( !m_pActiveContext || m_pActiveContext->canEdit( i->second, m_pActiveTool->getToolType(), m_toolMode ) )
            rect = rect.united( i->first->sceneBoundingRect() );
    }
    fitInView( rect, Qt::KeepAspectRatio );
    SetZoom( QVector2D( this->transform().m11(), this->transform().m22() ) );
}

void BlueprintView::OnCmd_TabOut()
{
    if( m_pActiveContext && m_pActiveContext->getParent() )
        SelectContext( m_pActiveContext->getParent() );
}

void BlueprintView::OnCmd_Extrude()
{
    Preview* pPreviewDlg = new Preview( this, m_pBlueprintEdit );
    pPreviewDlg->setAttribute( Qt::WA_DeleteOnClose );
    pPreviewDlg->show();
}

void BlueprintView::OnCmd_AddProperty()
{
    ASSERT( m_pActiveContext );
    ASSERT( m_pSelectionModel );
    
    Blueprint::Node::PtrCstVector nodes;
    {
        QModelIndexList selection = 
            m_pSelectionModel->selectedIndexes();
        for( const QModelIndex& index : selection )
        {
            Blueprint::Node::PtrCst pNode = m_pModel->getIndexNode( index );
            nodes.push_back( pNode );
        }
    }
        
    std::string strPropertyName;
    std::string strPropertyValue;
    
    //create property editor dialog
    Property* pPropertyDlg = new Property( this, strPropertyName, strPropertyValue );
    pPropertyDlg->setAttribute( Qt::WA_DeleteOnClose );
    if( QDialog::Accepted == pPropertyDlg->exec() )
    {
        if( !strPropertyName.empty() )
        {
            m_pBlueprintEdit->cmd_addProperties( nodes, strPropertyName, strPropertyValue );
        }
        else
        {
            QMessageBox::warning( this,
                                  tr( "Property Edit" ), tr( "Invalid property name" ) );
        }
    }
    
    update();
    invalidateScene();
    OnBlueprintModified();
}

void BlueprintView::OnCmd_DelProperty()
{
    ASSERT( m_pActiveContext );
    ASSERT( m_pSelectionModel );
    
    Blueprint::Node::PtrCstVector nodes;
    {
        QModelIndexList selection = 
            m_pSelectionModel->selectedIndexes();
        for( const QModelIndex& index : selection )
        {
            Blueprint::Node::PtrCst pNode = m_pModel->getIndexNode( index );
            nodes.push_back( pNode );
        }
    }
    
    {
        m_pSelectionModel->reset();
        m_pActiveTool->reset();
        SelectionSet selection;
        setSelected( selection );
    }
    
    m_pBlueprintEdit->cmd_deleteProperties( nodes );
    
    update();
    invalidateScene();
    OnBlueprintModified();
}

void BlueprintView::OnCmd_EditProperty()
{
    ASSERT( m_pActiveContext );
    ASSERT( m_pSelectionModel );
    
    std::string strPropertyName;
    std::string strPropertyValue;
        
    Blueprint::Node::PtrCstVector nodes;
    {
        QModelIndexList selection = 
            m_pSelectionModel->selectedIndexes();
        for( const QModelIndex& index : selection )
        {
            Blueprint::Node::PtrCst pNode = m_pModel->getIndexNode( index );
            
            if( Blueprint::Property::PtrCst pProperty = 
                boost::dynamic_pointer_cast< const Blueprint::Property >( pNode ) )
            {
                strPropertyName     = pProperty->getName();
                strPropertyValue    = pProperty->getValue();
            }
            nodes.push_back( pNode );
        }
    }
    
    //create property editor dialog
    Property* pPropertyDlg = new Property( this, strPropertyName, strPropertyValue );
    pPropertyDlg->setAttribute( Qt::WA_DeleteOnClose );
    if( QDialog::Accepted == pPropertyDlg->exec() )
    {
        if( !strPropertyName.empty() )
        {
            m_pBlueprintEdit->cmd_editProperties( nodes, strPropertyName, strPropertyValue );
        }
        else
        {
            QMessageBox::warning( this,
                                  tr( "Property Edit" ), tr( "Invalid property name" ) );
        }
    }
    
    update();
    invalidateScene();
    OnBlueprintModified();
}
    
void BlueprintView::OnSelectTool_Selector()
{
    ASSERT( m_pActiveTool );
    m_pActiveTool->reset();
    m_pActiveTool = &m_selectTool;
}

void BlueprintView::OnSelectTool_Pen()
{
    ASSERT( m_pActiveTool );
    m_pActiveTool->reset();
    m_pActiveTool = &m_penTool;
}

void BlueprintView::OnSelectTool_Property()
{
    ASSERT( m_pActiveTool );
    m_pActiveTool->reset();
    m_pActiveTool = &m_propertyTool;
}

void BlueprintView::OnSelectMode_Area()
{
    ASSERT( m_pActiveTool );
    m_toolMode = Blueprint::IEditContext::eArea;
}

void BlueprintView::OnSelectMode_Contour()
{
    ASSERT( m_pActiveTool );
    m_toolMode = Blueprint::IEditContext::eContour;
}

//void BlueprintView::OnSelectMode_Connection()
//{
//    ASSERT( m_pActiveTool );
//    //m_toolMode = Blueprint::IEditContext::eConnection;
//}

void BlueprintView::OnRotateLeft()
{
    ASSERT( m_pActiveContext );
    m_pActiveContext->cmd_rotateLeft( getSelection() );
    
    update();
    invalidateScene();
    OnBlueprintModified();
}

void BlueprintView::OnRotateRight()
{
    ASSERT( m_pActiveContext );
    m_pActiveContext->cmd_rotateRight( getSelection() );
    
    update();
    invalidateScene();
    OnBlueprintModified();
}

void BlueprintView::OnFlipHorizontally()
{
    ASSERT( m_pActiveContext );
    m_pActiveContext->cmd_flipHorizontally( getSelection() );
    
    update();
    invalidateScene();
    OnBlueprintModified();
}

void BlueprintView::OnFlipVeritcally()
{
    ASSERT( m_pActiveContext );
    m_pActiveContext->cmd_flipVertically( getSelection() );
    
    update();
    invalidateScene();
    OnBlueprintModified();
}
    
void BlueprintView::OnSetQuantise( int iQuantisation )
{
    switch( iQuantisation )
    {
        default:
        case 0: m_iQuantisation = 1;    break;
        case 1: m_iQuantisation = 2;    break;
        case 2: m_iQuantisation = 4;    break;
        case 3: m_iQuantisation = 8;    break;
        case 4: m_iQuantisation = 16;    break;
        case 5: m_iQuantisation = 32;    break;
        case 6: m_iQuantisation = 64;    break;
        case 7: m_iQuantisation = 128;    break;
        case 8: m_iQuantisation = 256;    break;
            break;
    }
}

void BlueprintView::OnCurrentSelectionItemChanged( const QModelIndex& , const QModelIndex&  )
{
    qDebug() << "BlueprintView::OnCurrentSelectionItemChanged";
}

Selectable* BlueprintView::selectableFromNode( Blueprint::Node::PtrCst pNode ) const
{
    Selectable* pSelectable = 0u;

    if( Blueprint::Site::PtrCst pSite =
            boost::dynamic_pointer_cast< const Blueprint::Site >( pNode ) )
    {
        if( const Blueprint::GlyphSpec* pSpec = dynamic_cast< const Blueprint::GlyphSpec* >( pSite.get() ) )
        {
            SpecMap::const_iterator iFind = m_specMap.find( pSpec );
            if( iFind != m_specMap.end() )
            {
                ItemMap::const_iterator iFind2 = m_itemMap.find( iFind->second );
                if( iFind2 != m_itemMap.end() )
                {
                    pSelectable = Selection::glyphToSelectable( iFind2->second );
                }
            }
        }
    }
    return pSelectable;
}

void BlueprintView::OnSelectionChanged( const QItemSelection& selected, const QItemSelection& deselected )
{
    qDebug() << "BlueprintView::OnSelectionChanged";

    const QModelIndexList selectedIndices = selected.indexes();
    for( QModelIndexList::const_iterator i = selectedIndices.begin(),
         iEnd = selectedIndices.end(); i!=iEnd; ++i )
    {
        if( Selectable* pSelectable = selectableFromNode( m_pModel->getIndexNode( *i ) ) )
            pSelectable->setSelected( true );
    }
    const QModelIndexList deselectedIndices = deselected.indexes();
    for( QModelIndexList::const_iterator i = deselectedIndices.begin(),
         iEnd = deselectedIndices.end(); i!=iEnd; ++i )
    {
        if( Selectable* pSelectable = selectableFromNode( m_pModel->getIndexNode( *i ) ) )
            pSelectable->setSelected( false );
    }
}

void BlueprintView::setViewMode( bool bArrangement, bool bCellComplex, bool bClearance )
{
    qDebug() << "OnBitmapChanged called: " << bArrangement << " " << bCellComplex << " " << bClearance;
    
    m_pBlueprintEdit->setViewMode( bArrangement, bCellComplex, bClearance );
    
    update();
    invalidateScene();
    OnBlueprintModified();
}

void BlueprintView::SelectContext( Blueprint::IEditContext* pNewContext )
{
    ASSERT( m_pActiveContext );
    qDebug() << "SelectContext called.";
    setSelected( SelectionSet() );

    Blueprint::IEditContext* pOldContext = m_pActiveContext;
    m_pActiveContext = pNewContext;
    m_pActiveContext->activated();

    {
        SpecMap::const_iterator iFind = m_specMap.find( pOldContext->getOrigin() );
        if( iFind != m_specMap.end() )
        {
            ItemMap::const_iterator iFind2 = m_itemMap.find( iFind->second );
            if( iFind2 != m_itemMap.end() )
                iFind2->second->update();
        }
    }
    {
        SpecMap::const_iterator iFind = m_specMap.find( m_pActiveContext->getOrigin() );
        if( iFind != m_specMap.end() )
        {
            ItemMap::const_iterator iFind2 = m_itemMap.find( iFind->second );
            if( iFind2 != m_itemMap.end() )
                iFind2->second->update();
        }
    }

    OnEditContextChanged( BlueprintContext( m_pActiveContext ) );
}

void BlueprintView::setSelected( const SelectionSet& selection )
{
    for( ItemMap::const_iterator i = m_itemMap.begin(),
         iEnd = m_itemMap.end(); i!=iEnd; ++i )
    {
        if( Selectable* pSelectable = Selection::glyphToSelectable( i->second ) )
        {
            if( !pSelectable->isImage() )
                pSelectable->setSelected( selection.find( i->second ) != selection.end() );
        }
    }

    ASSERT( m_pSelectionModel );
    ASSERT( m_pModel );

    QItemSelection itemSelection;

    for( SelectionSet::const_iterator i = selection.begin(),
         iEnd = selection.end(); i!=iEnd; ++i )
    {
        Blueprint::IGlyph* pGlyph = *i;
        if( const Blueprint::Site* pSite =
                dynamic_cast< const Blueprint::Site* >( pGlyph->getGlyphSpec() ) )
        {
            itemSelection.push_back( QItemSelectionRange( m_pModel->getNodeIndex( pSite->getPtr() ) ) );
        }
    }

    m_pSelectionModel->select( itemSelection, QItemSelectionModel::ClearAndSelect );

}

Blueprint::Site::Ptr BlueprintView::getCurrentPaletteItem() const
{
    Blueprint::Site::Ptr pItem;
    if( m_pToolBox )
        pItem = m_pToolBox->getCurrentItem();
    return pItem;
}

SelectionSet BlueprintView::getSelection() const
{
    SelectionSet selection;
    for( ItemMap::const_iterator i = m_itemMap.begin(),
         iEnd = m_itemMap.end(); i!=iEnd; ++i )
    {
        if( Selectable* pSelectable = Selection::glyphToSelectable( i->second ) )
        {
            if( pSelectable->isSelected() )
                selection.insert( i->second );
        }
    }
    return selection;
}

SelectionSet BlueprintView::getSelectedByRect( const QRectF& rect ) const
{
    ASSERT( m_pActiveTool );
    SelectionSet selection;
    QList< QGraphicsItem* > stack = items( mapFromScene( rect ) );
    for( QList< QGraphicsItem* >::iterator i = stack.begin(),
         iEnd = stack.end(); i!=iEnd; ++i )
    {
        if( Blueprint::IGlyph* pTest = findGlyph( *i ) )
        {
            if( m_pActiveContext->canEdit( pTest, m_pActiveTool->getToolType(), m_toolMode ) )
            {
                if( Selectable* pSelectable = Selection::glyphToSelectable( pTest ) )
                {
                    //const bool bIsCustomTool = m_pActiveTool->getToolType() != Blueprint::IEditContext::eSelect && 
                    //    m_pActiveTool->getToolType() != Blueprint::IEditContext::eDraw;
                    //if( ( !bIsCustomTool && pSelectable->isImage() ) || 
                    //    ( bIsCustomTool && !pSelectable->isImage() ) )
                        selection.insert( pTest );
                }
            }
        }
    }
    return selection;
}

SelectionSet BlueprintView::getSelectedByPath( const QPainterPath& path ) const
{
    ASSERT( m_pActiveTool );
    SelectionSet selection;
    QList< QGraphicsItem* > stack = items( mapFromScene( path ) );
    for( QList< QGraphicsItem* >::iterator i = stack.begin(),
         iEnd = stack.end(); i!=iEnd; ++i )
    {
        if( Blueprint::IGlyph* pTest = findGlyph( *i ) )
        {
            if( m_pActiveContext->canEdit( pTest, m_pActiveTool->getToolType(), m_toolMode ) )
            {
                if( Selectable* pSelectable = Selection::glyphToSelectable( pTest ) )
                {
                    //if( !pSelectable->isImage() )
                        selection.insert( pTest );
                }
            }
        }
    }
    return selection;
}

QGraphicsItem* BlueprintView::findActiveContextItem() const
{
    ASSERT( m_pActiveContext );
    QGraphicsItem* pItem = 0u;
    if( const Blueprint::GlyphSpec* pGlyphSpec = m_pActiveContext->getOrigin() )
    {
        SpecMap::const_iterator iFind = m_specMap.find( pGlyphSpec );
        if( iFind != m_specMap.end() )
            pItem = iFind->second;
    }
    return pItem;
}

QVector2D BlueprintView::calculateQuantisation() const
{
    float fXStep = 1.0f;
    while( fXStep < ( m_iQuantisation / m_v2ZoomLevel.x() ) )
        fXStep *= 2.0f;
    float fYStep = 1.0f;
    while( fYStep < ( m_iQuantisation / m_v2ZoomLevel.y() ) )
        fYStep *= 2.0f;
    return QVector2D( fXStep, fYStep );
}

QPointF BlueprintView::mapToActiveContext( const QPointF& scenePos ) const
{
    QPointF pos = scenePos;
    if( QGraphicsItem* pContextItem = findActiveContextItem() )
        pos = pContextItem->mapFromScene( scenePos );
    return pos;
}

Blueprint::IGlyph* BlueprintView::findGlyph( QGraphicsItem* pItem ) const
{
    Blueprint::IGlyph* pGlyph = 0u;
    ItemMap::const_iterator iFind = m_itemMap.find( pItem );
    if( iFind != m_itemMap.end() )
        pGlyph = iFind->second;
    return pGlyph;
}

Blueprint::IGlyph* BlueprintView::findSelectableTopmostGlyph( const QPointF& pos ) const
{
    Blueprint::IGlyph* pGlyph = nullptr;
    if( m_pActiveContext && m_pActiveContext && m_pActiveTool )
    {
        QList< QGraphicsItem* > stack = items( pos.x(), pos.y() );
        for( QList< QGraphicsItem* >::iterator i = stack.begin(),
             iEnd = stack.end(); i!=iEnd; ++i )
        {
            if( Blueprint::IGlyph* pTest = findGlyph( *i ) )
            {
                if( m_pActiveContext->canEdit( pTest, m_pActiveTool->getToolType(), m_toolMode ) )
                {
                    if( Selection::glyphToSelectable( pTest ) )
                    {
                        pGlyph = pTest;
                        break;
                    }
                }
            }
        }
    }
    return pGlyph;
}

//glyph factory interface
Blueprint::IGlyph::Ptr BlueprintView::createControlPoint( Blueprint::ControlPoint* pControlPoint, Blueprint::IGlyph::Ptr pParent )
{
    Blueprint::IGlyph::Ptr pNewGlyph( new GlyphControlPoint( pParent, m_pBlueprintScene, 
        GlyphMap( m_itemMap, m_specMap ), pControlPoint, m_v2ZoomLevel.y(), true, m_pToolBox ) );
    CalculateOversizedSceneRect();
    return pNewGlyph;
}

Blueprint::IGlyph::Ptr BlueprintView::createOrigin( Blueprint::Origin* pOrigin, Blueprint::IGlyph::Ptr pParent )
{
    Blueprint::IGlyph::Ptr pNewGlyph( new GlyphOrigin( pParent, m_pBlueprintScene, 
        GlyphMap( m_itemMap, m_specMap ), pOrigin, m_pActiveContext, true, m_pToolBox ) );
    return pNewGlyph;
}

Blueprint::IGlyph::Ptr BlueprintView::createMarkupPath( Blueprint::MarkupPath* pMarkupPath, Blueprint::IGlyph::Ptr pParent )
{
    Blueprint::IGlyph::Ptr pNewGlyph( new GlyphPath( pParent, m_pBlueprintScene, 
        GlyphMap( m_itemMap, m_specMap ), pMarkupPath, m_v2ZoomLevel.y(), true, m_pToolBox ) );
    return pNewGlyph;
}

Blueprint::IGlyph::Ptr BlueprintView::createMarkupPolygonGroup( Blueprint::MarkupPolygonGroup* pMarkupPolygonGroup, Blueprint::IGlyph::Ptr pParent )
{
    Blueprint::IGlyph::Ptr pNewGlyph( new GlyphPolygonGroup( pParent, m_pBlueprintScene, 
        GlyphMap( m_itemMap, m_specMap ), pMarkupPolygonGroup, m_v2ZoomLevel.y(), true, m_pToolBox ) );
    return pNewGlyph;
}

Blueprint::IGlyph::Ptr BlueprintView::createMarkupText( Blueprint::MarkupText* pMarkupText, Blueprint::IGlyph::Ptr pParent )
{
    Blueprint::IGlyph::Ptr pNewGlyph( new GlyphText( pParent, m_pBlueprintScene, 
        GlyphMap( m_itemMap, m_specMap ), pMarkupText, true, m_pToolBox ) );
    return pNewGlyph;
}

void BlueprintView::mouseDoubleClickEvent(QMouseEvent * event)
{
    ASSERT( m_pActiveContext );
    if( event->button() == Qt::LeftButton )
    {
        m_interactionMode = eTool;
        QList< QGraphicsItem* > stack = items( event->pos() );
        std::vector< Blueprint::IGlyph* > glyphStack;
        for( QList< QGraphicsItem* >::iterator i = stack.begin(),
             iEnd = stack.end(); i!=iEnd; ++i )
        {
            if( Blueprint::IGlyph* pTest = findGlyph( *i ) )
                glyphStack.push_back( pTest );
        }
        if( Blueprint::IEditContext* pNewContext =
                m_pActiveContext->getNestedContext( glyphStack ) )
        {
            SelectContext( pNewContext );
        }
        else if( m_pActiveContext != m_pBlueprintEdit.get() )
        {
            SelectContext( m_pActiveContext->getParent() );  
        }   
    }
    else if( event->button() == Qt::RightButton )
    {
        if( m_pActiveContext != m_pBlueprintEdit.get() )
            SelectContext( m_pActiveContext->getParent() );
    }
    else
    {
        QGraphicsView::mouseDoubleClickEvent( event );
    }
}

void BlueprintView::mousePressEvent(QMouseEvent *event)
{
    ASSERT( m_pActiveTool );
    if( event->button() == Qt::LeftButton )
    {
        m_interactionMode = eTool;
        m_pActiveTool->mousePressEvent( event );
        OnBlueprintModified();
        
        if( Blueprint::Site::Ptr pInteractionSite = m_pActiveTool->GetInteractionSite() )
        {
            if( !m_pActiveContext->isSiteContext( pInteractionSite ) )
            {
                if( Blueprint::IEditContext* pNewContext =
                        m_pActiveContext->getSiteContext( pInteractionSite ) )
                {
                    SelectContext( pNewContext );
                }
            }
        }
    }
    else if( event->button() == Qt::MiddleButton )
    {
        setCursor( Qt::ClosedHandCursor );
        m_interactionMode = eScroll;
        QPointF scrollPositions( horizontalScrollBar()->value(), verticalScrollBar()->value() );
        m_pScrollData.reset( new ScrollData( event->pos(), scrollPositions ) );
    }
    else if( event->button() == Qt::RightButton )
    {
        m_interactionMode = eTool;
        m_pActiveTool->mousePressEvent( event );
        OnBlueprintModified();
    }
}

void BlueprintView::mouseHover( QMouseEvent* event )
{
    ASSERT( m_pActiveTool );
    m_pActiveTool->mouseHover( event );
}

void BlueprintView::mouseMoveEvent(QMouseEvent *event)
{
    ASSERT( m_pActiveTool );
    switch( m_interactionMode )
    {
        case eScroll:
            setCursor( Qt::ClosedHandCursor );
            if( m_pScrollData.get() )
            {
                const QPointF delta = m_pScrollData->m_downPos - event->pos();
                horizontalScrollBar()->setValue(    m_pScrollData->m_downScrollPos.x() + delta.x() );
                verticalScrollBar()->setValue(      m_pScrollData->m_downScrollPos.y() + delta.y() );
            }
            break;
        case eTool:
            {
                //LOG_PROFILE_BEGIN( tool_mouse_move_time );
                m_pActiveTool->mouseMoveEvent( event );
                OnBlueprintModified();
                //LOG_PROFILE_END( tool_mouse_move_time );
                //LOG_PROFILE_STATIC( tool_mouse_move );
            }
            break;
        case eNone:
        default:
            setCursor( Qt::ArrowCursor );
            mouseHover( event );
            break;
    }

    QGraphicsView::mouseMoveEvent( event );
}

void BlueprintView::mouseReleaseEvent(QMouseEvent *event)
{
    ASSERT( m_pActiveTool );
    m_pActiveTool->mouseReleaseEvent( event );
    m_pScrollData.reset();
    m_interactionMode = eNone;

    mouseHover( event );
    update();
    invalidateScene();
    OnBlueprintModified();

    QGraphicsView::mouseReleaseEvent( event );
}

void BlueprintView::keyPressEvent(QKeyEvent *event)
{
    ASSERT( m_pActiveTool );
    ASSERT( m_pActiveContext );
    if( ( event->key() == Qt::Key_Space ) || ( event->key() == Qt::Key_Tab ) )
    {
        if( Blueprint::IEditContext* pParentContext = m_pActiveContext->getParent() )
            SelectContext( pParentContext );
    }
    else
    {
        m_pActiveTool->keyPressEvent( event );
        QGraphicsView::keyPressEvent( event );
    }
}

void BlueprintView::SetZoom( QVector2D v2NewZoomLevel )
{
    m_v2ZoomLevel = v2NewZoomLevel;
    setTransform( QTransform::fromScale( m_v2ZoomLevel.x(), m_v2ZoomLevel.y() ) );
    for( ItemMap::const_iterator i = m_itemMap.begin(),
         iEnd = m_itemMap.end(); i!=iEnd; ++i )
    {
        if( ZoomDependent* pZoomItem = dynamic_cast< ZoomDependent* >( i->second ) )
            pZoomItem->OnNewZoomLevel( m_v2ZoomLevel.y() );
    }
    CalculateOversizedSceneRect();
    CalculateRulerItems();
}

void BlueprintView::DoZoom( float fAmt )
{
    if( m_interactionMode == eNone )
    {
        ViewportAnchor oldAnchor = transformationAnchor();
        setTransformationAnchor( AnchorUnderMouse );
        SetZoom( QVector2D( m_v2ZoomLevel.y() * fAmt, m_v2ZoomLevel.y() * fAmt  ) );
        setTransformationAnchor( oldAnchor );
    }
}

void BlueprintView::CalculateOversizedSceneRect()
{
    QRectF sceneRect( 0.0f,0.0f,0.0f,0.0f );
    QList< QGraphicsItem* > allItems = items();
    for( QList< QGraphicsItem* >::iterator i = allItems.begin(),
         iEnd = allItems.end(); i!=iEnd; ++i )
        sceneRect = sceneRect.united( ( *i )->boundingRect() );
    const float f = 1024.0f;
    m_pBlueprintScene->setSceneRect(
                QRectF( sceneRect.left() - f, sceneRect.top() - f,
                        sceneRect.width() + f*2.0f, sceneRect.height() + f*2.0f ) );
}

void BlueprintView::scrollContentsBy(int dx, int dy )
{
    QGraphicsView::scrollContentsBy( dx, dy );

    CalculateRulerItems();
}

void BlueprintView::CalculateRulerItems()
{
    const QRect viewportRect = viewport()->rect();
    const QRectF rect( mapToScene( viewportRect.topLeft() ), mapToScene( viewportRect.bottomRight() ) );

    int iMainLineStep = 4;
    if( m_pToolBox )
    {
        m_pToolBox->getConfigValue( ".background.step", iMainLineStep );
    }
        
    float fXStep = 1.0f;
    while( fXStep / 16.0f < ( iMainLineStep / m_v2ZoomLevel.x() ) )
        fXStep *= 2.0f;
    float fYStep = 1.0f;
    while( fYStep / 16.0f < ( iMainLineStep / m_v2ZoomLevel.y() ) )
        fYStep *= 2.0f;

    const float fOffsetX = 8.0f / m_v2ZoomLevel.x();
    const float fOffsetY = 8.0f / m_v2ZoomLevel.y();

    QColor textColor( 0,0,0,255 );
    
    if( m_pToolBox )
    {
        m_pToolBox->getConfigValue( ".background.text.colour", textColor );
    }

    const float fQuantLeft    =   Math::quantize< float >( rect.left(),      fXStep );
    const float fQuantRight   =   Math::quantize< float >( rect.right(),     fXStep ) + fXStep;
    const float fQuantTop     =   Math::quantize< float >( rect.top(),       fYStep );
    const float fQuantBottom  =   Math::quantize< float >( rect.bottom(),    fYStep ) + fYStep;

    {
        TextItemVector::iterator xIter = m_rulerVertItems.begin();
        for( float x = fQuantLeft; x <= fQuantRight; x += fXStep )
        {
            QGraphicsSimpleTextItem* pItem = 0u;
            std::ostringstream os;
            os << x;
            if( xIter != m_rulerVertItems.end() )
            {
                pItem = *xIter;
                ++xIter;
            }
            else
            {
                pItem = m_pBlueprintScene->addSimpleText( os.str().c_str() );
                pItem->setFlag( QGraphicsItem::ItemIgnoresTransformations );
                pItem->setZValue( 0.0f );
                m_rulerVertItems.push_back( pItem );
                xIter = m_rulerVertItems.end();
            }
            pItem->setText( os.str().c_str() );
            pItem->setBrush( QBrush( textColor ) );
            pItem->setPos( x, rect.bottom() + fOffsetY * 2.0f );
        }
        for( TextItemVector::iterator x = xIter ;x != m_rulerVertItems.end(); ++x )
        {
            m_pBlueprintScene->removeItem( *x );
            delete *x;
        }
        m_rulerVertItems.erase( xIter, m_rulerVertItems.end() );
    }

    {
        TextItemVector::iterator yIter = m_rulerHoriItems.begin();
        for( float y = fQuantTop; y <= fQuantBottom; y += fYStep )
        {
            QGraphicsSimpleTextItem* pItem = 0u;
            std::ostringstream os;
            os << y;
            if( yIter != m_rulerHoriItems.end() )
            {
                pItem = *yIter;
                ++yIter;
            }
            else
            {
                pItem = m_pBlueprintScene->addSimpleText( os.str().c_str() );
                pItem->setFlag( QGraphicsItem::ItemIgnoresTransformations );
                pItem->setZValue( 0.0f );
                m_rulerHoriItems.push_back( pItem );
                yIter = m_rulerHoriItems.end();
            }
            pItem->setText( os.str().c_str() );
            pItem->setBrush( QBrush( textColor ) );
            pItem->setPos( rect.left() + fOffsetX, y + fOffsetY * 2.0f );
        }
        for( TextItemVector::iterator y = yIter ;y != m_rulerHoriItems.end(); ++y )
        {
            m_pBlueprintScene->removeItem( *y );
            delete *y;
        }
        m_rulerHoriItems.erase( yIter, m_rulerHoriItems.end() );
    }
}

const float BlueprintView::m_fZoomRate = 0.1f;

void BlueprintView::wheelEvent(QWheelEvent *event)
{
    if( m_interactionMode == eNone )
    {
        if( event->angleDelta().y() > 0 )
            DoZoom( 1.0f + m_fZoomRate );
        else if( event->angleDelta().y() < 0 )
            DoZoom( 1.0f - m_fZoomRate );
        else
            QGraphicsView::wheelEvent( event );
    }
}

void BlueprintView::drawBackground(QPainter *painter, const QRectF &rect)
{
    float fXStep = 1.0f;
    while( fXStep / 16.0f < ( 1 / m_v2ZoomLevel.x() ) )
        fXStep *= 2.0f;
    float fYStep = 1.0f;
    while( fYStep / 16.0f < ( 1 / m_v2ZoomLevel.y() ) )
        fYStep *= 2.0f;

    QColor mainLineColour( 225, 225, 225, 125 );
    QColor otherLineColour( 100, 100, 100, 125 );
    QColor bkgrnd( 255, 255, 255 );
    
    int iMainLineStep = 4;
    float lineWidth = 0.5f;
    if( m_pToolBox )
    {
        m_pToolBox->getConfigValue( ".background.colour", bkgrnd );
        m_pToolBox->getConfigValue( ".background.step", iMainLineStep );
        m_pToolBox->getConfigValue( ".background.lines.width", lineWidth );
        m_pToolBox->getConfigValue( ".background.lines.main.colour", mainLineColour );
        m_pToolBox->getConfigValue( ".background.lines.other.colour", otherLineColour );
    }
    
    painter->fillRect( rect, bkgrnd );

    //grid lines
    QPen oldPen = painter->pen();
    {
        QPen gridPen;
        gridPen.setStyle( Qt::DotLine );
        gridPen.setWidth( lineWidth );
        painter->setPen( gridPen );

        const float fQuantLeft    =   Math::quantize< float >( rect.left(),      fXStep * iMainLineStep );
        const float fQuantRight   =   Math::quantize< float >( rect.right(),     fXStep ) + fXStep * iMainLineStep;
        const float fQuantTop     =   Math::quantize< float >( rect.top(),       fYStep * iMainLineStep );
        const float fQuantBottom  =   Math::quantize< float >( rect.bottom(),    fYStep ) + fYStep * iMainLineStep;

        unsigned int uiX = 0u;
        for( float x = fQuantLeft; x <= fQuantRight; x += fXStep, ++uiX )
        {
            if( x == 0 )
            {
                gridPen.setStyle( Qt::SolidLine );
                gridPen.setColor( mainLineColour );
            }
            else 
            {
                gridPen.setStyle( Qt::DotLine );
                if( uiX % iMainLineStep )
                {
                    gridPen.setColor( otherLineColour );
                }
                else
                {
                    gridPen.setColor( mainLineColour );
                }
            }
            painter->setPen( gridPen );
            painter->drawLine( QPoint( x, fQuantTop ), QPoint( x, fQuantBottom ) );
        }
        unsigned int uiY = 0u;
        for( float y = fQuantTop; y <= fQuantBottom; y += fYStep, ++uiY )
        {
            if( y == 0 )
            {
                gridPen.setStyle( Qt::SolidLine );
                gridPen.setColor( mainLineColour );
            }
            else
            {
                gridPen.setStyle( Qt::DotLine );
                if( uiY % iMainLineStep )
                    gridPen.setColor( otherLineColour );
                else
                    gridPen.setColor( mainLineColour );
            }
            painter->setPen( gridPen );
            painter->drawLine( QPoint( fQuantLeft, y ), QPoint( fQuantRight, y ) );
        }
    }
    painter->setPen( oldPen );
}





























