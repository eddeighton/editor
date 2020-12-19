#include "blueprinttoolbox.h"

#include <functional>

#include <QLabel>
#include <QMenu>

#ifndef Q_MOC_RUN
#include "common/assert_verify.hpp"
#include "common/stl.hpp"
#include "common/compose.hpp"
#include "common/math.hpp"
#include "common/rounding.hpp"
#endif

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
ClipScene::ClipScene( QWidget *parent) :
    QGraphicsScene( parent ),
    m_pNullContext( 0u )
{
}

void ClipScene::setSite( Blueprint::Site::Ptr pSite, Blueprint::Toolbox::Ptr pToolBox )
{
    m_pToolBox = pToolBox;
    
    float fScaling = 1.0f;
    if( m_pToolBox )
    {
        m_pToolBox->getConfigValue( ".toolbox.item.width", m_fDeviceWidth );
        m_pToolBox->getConfigValue( ".toolbox.item.scaling", fScaling );
    }
    
    m_fDeviceWidth *= fScaling; //artificially increase so that control points are smaller
    
    if( pSite )
    {
        m_pBlueprintEdit.reset();
        VERIFY_RTE( m_itemMap.empty() );
        VERIFY_RTE( m_specMap.empty() );

        m_pBlueprint = pSite;
        m_pBlueprint->init();
        m_pBlueprintEdit = Blueprint::EditMain::create( *this, m_pBlueprint, true, false, false );
    }
}

void ClipScene::calculateSceneRect()
{
    QRectF rect;
    for( ItemMap::const_iterator i = m_itemMap.begin(),
         iEnd = m_itemMap.end(); i!=iEnd; ++i )
    {
        rect = rect.united( i->first->sceneBoundingRect() );
    }
    rect = rect.marginsAdded( QMargins( 2, 2, 2, 2 ) );
    setSceneRect( rect );
}
    
//glyph factory interface
Blueprint::IGlyph::Ptr ClipScene::createControlPoint( Blueprint::ControlPoint* pControlPoint, Blueprint::IGlyph::Ptr pParent )
{
    Blueprint::IGlyph::Ptr pNewGlyph( new GlyphControlPoint( pParent, this, 
        GlyphMap( m_itemMap, m_specMap ), pControlPoint, m_fDeviceWidth / this->sceneRect().height(), m_pToolBox ) );
    return pNewGlyph;
}

Blueprint::IGlyph::Ptr ClipScene::createOrigin( Blueprint::Origin* pOrigin, Blueprint::IGlyph::Ptr pParent )
{
    Blueprint::IGlyph::Ptr pNewGlyph( new GlyphOrigin( pParent, this, 
        GlyphMap( m_itemMap, m_specMap ), pOrigin, m_pNullContext, m_pToolBox ) );
    return pNewGlyph;
}

Blueprint::IGlyph::Ptr ClipScene::createMarkupPolygonGroup( Blueprint::MarkupPolygonGroup* pMarkupPolygonGroup, Blueprint::IGlyph::Ptr pParent )
{
    Blueprint::IGlyph::Ptr pNewGlyph( new GlyphPolygonGroup( pParent, this, 
        GlyphMap( m_itemMap, m_specMap ), pMarkupPolygonGroup, m_fDeviceWidth / this->sceneRect().height(), m_pToolBox ) );
    return pNewGlyph;
}

Blueprint::IGlyph::Ptr ClipScene::createMarkupText( Blueprint::MarkupText* pMarkupText, Blueprint::IGlyph::Ptr pParent )
{
    Blueprint::IGlyph::Ptr pNewGlyph( new GlyphText( pParent, this, 
        GlyphMap( m_itemMap, m_specMap ), pMarkupText, m_pToolBox ) );
    return pNewGlyph;
}

void ClipScene::drawBackground(QPainter* , const QRectF& )
{
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
FlowView::FlowItem::FlowItem( FlowView& view, Blueprint::Site::Ptr pSite, Blueprint::Toolbox::Palette::Ptr pPalette )
    :   m_view( view ),
        m_pSite( pSite ),
        m_pPalette( pPalette )
{
    
    QColor bkgrnd( 255, 255, 255 );
    int itemWidth   = 256;
    int itemHeight  = 256;
    QColor textColour( 0,0,0 );
    std::string strFont = "Times";
    int fontSize = 8;
    
    Blueprint::Toolbox::Ptr pToolBox = view.getToolbox();
    if( pToolBox )
    {
        pToolBox->getConfigValue( ".toolbox.item.background.colour", bkgrnd );
        pToolBox->getConfigValue( ".toolbox.item.width", itemWidth );
        pToolBox->getConfigValue( ".toolbox.item.height", itemHeight );
        pToolBox->getConfigValue( ".toolbox.item.text.colour", textColour );
        pToolBox->getConfigValue( ".toolbox.item.text.font", strFont );
        pToolBox->getConfigValue( ".toolbox.item.text.fontsize", fontSize );
    }
    
    QPixmap buffer( itemWidth, itemHeight );
    {
        ClipScene tempScene;
        tempScene.setSite( pSite, m_view.m_toolBox.getToolbox() );
        
        QPainter painter;
        //painter.setRenderHint( QPainter::HighQualityAntialiasing, true );
        
        painter.begin( &buffer );
        {
            //fill the square
            {
                painter.fillRect( 0, 0, itemWidth, itemHeight, bkgrnd );
            }
        
            //paint the clip scene
            {
                tempScene.calculateSceneRect();
                tempScene.render( &painter );
            }
            
            //render useful text over the top
            {
                painter.setPen( textColour );
               
                static QFont newFont( strFont.c_str(), fontSize, QFont::Bold, false );
                painter.setFont( newFont );
                
                std::string strName = pSite->Blueprint::Node::getName();
                for( auto& c : strName )
                {
                    if( c == '_' )
                        c = ' ';
                }
                
                QRectF bounds( 0, 0, itemWidth, itemHeight );
                painter.drawText( bounds, Qt::AlignCenter | Qt::TextWordWrap, strName.c_str() ); //
            }
        }
        
        painter.end();
    }

    m_pImageItem = new QGraphicsPixmapItem( buffer );
    m_pImageItem->setZValue( 0.0f );
    m_view.scene()->addItem( m_pImageItem );
}
bool FlowView::FlowItem::onHit( QGraphicsItem* pItem )
{
    bool bHit = false;
    if( m_pImageItem == pItem )
    {
        m_pPalette->select( m_pSite );
        m_view.OnClipboardAction();
        bHit = true;
    }
    return bHit;
}
void FlowView::FlowItem::updatePosition( int x, int y, float fWidth, float fHeight, float fSpacing )
{
    QRectF bounds = m_pImageItem->boundingRect();
    if( bounds.isNull() || bounds.isEmpty() )
        bounds = QRectF( 0, 0, 1, 1 );

    m_pImageItem->setPos( x * ( fWidth + fSpacing ), y * ( fHeight + fSpacing ) );
    m_pImageItem->setTransform( QTransform::fromScale( 
        fWidth / bounds.width(), fHeight / bounds.height() ) );
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
static float g_fItemWidth = 256.0f;
static float g_fItemHeight = 256.0f;
static float g_fItemSpacing = 8.0f;
static float g_fSelectionWidth = 2.0f;
static QColor g_selectionColour( 0,0,255 );
static QColor g_selectionBackground( 0,0,150,100 );
FlowView::FlowView( BlueprintToolbox& toolbox, Blueprint::Toolbox::Palette::Ptr pPalette ) 
    :   QGraphicsView( &toolbox ),
        m_toolBox( toolbox ),
        m_pPalette( pPalette )
{
    setScene( new QGraphicsScene( this ) );
    setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    setTransformationAnchor( QGraphicsView::AnchorViewCenter );
    
    Blueprint::Toolbox::Ptr pToolBox = toolbox.getToolbox();
    if( pToolBox )
    {
        pToolBox->getConfigValue( ".toolbox.item.width", g_fItemWidth );
        pToolBox->getConfigValue( ".toolbox.item.height", g_fItemHeight );
        pToolBox->getConfigValue( ".toolbox.item.spacing", g_fItemSpacing );
        pToolBox->getConfigValue( ".toolbox.selection.width", g_fSelectionWidth );
        pToolBox->getConfigValue( ".toolbox.selection.colour", g_selectionColour );
        pToolBox->getConfigValue( ".toolbox.selection.background", g_selectionBackground );
    }
    
    QPen selectionPen( Qt::SolidLine );
    selectionPen.setWidth( g_fSelectionWidth );
    selectionPen.setBrush( g_selectionColour );
    m_pSelectionRectItem = scene()->addRect( QRect(0,0,0,0), selectionPen, QBrush( g_selectionBackground ) );
    m_pSelectionRectItem->setZValue( 1.0f );
    m_pSelectionRectItem->setVisible( false );

    updateClips();
}

FlowView::~FlowView()
{
}

Blueprint::Toolbox::Ptr FlowView::getToolbox() const 
{ 
    return m_toolBox.getToolbox(); 
}

void FlowView::updateClips()
{
    Blueprint::Site::PtrSet sites( m_pPalette->get().begin(), m_pPalette->get().end() );

    ItemMap removals;
    Blueprint::Site::PtrSet additions;
    generics::match( m_items.begin(), m_items.end(),
        sites.begin(), sites.end(),
        generics::lessthan( generics::first< ItemMap::const_iterator >(), 
                            generics::deref< Blueprint::Site::PtrSet::const_iterator >() ),
        generics::collect( removals, generics::deref< ItemMap::const_iterator >() ),
        generics::collect( additions, generics::deref< Blueprint::Site::PtrSet::const_iterator >() ) );

    for( ItemMap::iterator i = removals.begin(),
        iEnd = removals.end(); i!=iEnd; ++i )
        m_items.erase( i->first );
    removals.clear();
    for( Blueprint::Site::PtrSet::iterator i = additions.begin(),
        iEnd = additions.end(); i!=iEnd; ++i )
        m_items.insert( std::make_pair( *i, FlowItem::Ptr( new FlowItem( *this, *i, m_pPalette ) ) ) );
    calculateLayout();
}

void FlowView::clear()
{
    m_items.clear();
}

int FlowView::findIndex( Blueprint::Site::Ptr pSite )
{
    Blueprint::Site::PtrList::const_iterator iFind = 
       std::find( m_pPalette->get().begin(), m_pPalette->get().end(), pSite );
    if( iFind != m_pPalette->get().end() )
        return std::distance( m_pPalette->get().begin(), iFind );
    else
        return -1;
}

void FlowView::calculateLayout()
{
    //determine the indices for each element
    FlowItem::Ptr pSelectedItem;
    FlowItem::PtrVector items( m_items.size() );
    for( ItemMap::iterator i = m_items.begin(),
        iEnd = m_items.end(); i!=iEnd; ++i )
    {
        const int iIndex = findIndex( i->first );
        if( iIndex >= 0 )
        {
            items[ iIndex ] = i->second;
            if( i->second->getSite() == m_pPalette->getSelection() )
                pSelectedItem = i->second;
        }
    }
    

    //decide the stride
    int iStride = std::max( 1.0f, viewport()->rect().width() / g_fItemWidth );

    //now calculate the positions and sizes
    FlowItem::PtrVector::iterator i = items.begin();
    int y = 0;
    for( ; i != items.end(); ++y)
    {
        for( int x = 0; x < iStride && i != items.end(); ++x, ++i )
            (*i)->updatePosition( x, y, g_fItemWidth, g_fItemHeight, g_fItemSpacing );
    }

    setSceneRect( 0.0f, 0.0f, iStride * ( g_fItemWidth + g_fItemSpacing ), y * ( g_fItemHeight + g_fItemSpacing ) );

    if( pSelectedItem )
    {
        m_pSelectionRectItem->setRect( pSelectedItem->getRect() );
        m_pSelectionRectItem->setVisible( true );
    }
    else
    {
        m_pSelectionRectItem->setVisible( false );
    }
    
}

void FlowView::resizeEvent(QResizeEvent * event)
{
    QGraphicsView::resizeEvent( event );
    calculateLayout();
}

void FlowView::mouseDoubleClickEvent(QMouseEvent * event)
{
    QGraphicsView::mouseDoubleClickEvent( event );
}

void FlowView::mousePressEvent(QMouseEvent *event)
{
    qDebug() << "At start of FlowView::mousePressEvent";
    QGraphicsView::mousePressEvent( event );
    
    QGraphicsPixmapItem* pImageItem = 0u;
    QList< QGraphicsItem* > stack = items( event->pos() );
    for( QList< QGraphicsItem* >::iterator i = stack.begin(),
         iEnd = stack.end(); i!=iEnd; ++i )
    {
        if( pImageItem = dynamic_cast< QGraphicsPixmapItem* >( *i ) )
            break;
    }

    if( pImageItem )
    {
        Blueprint::Site::Ptr pSite;
        for( ItemMap::iterator i = m_items.begin(),
            iEnd = m_items.end(); i!=iEnd; ++i )
        {
            if( i->second->onHit( pImageItem ) )
                pSite = i->second->getSite();
        }

        if( pSite )
        {
            if( event->button() == Qt::RightButton )
            {
                OnMenu( ClipboardMsg( pSite, m_pPalette ) );
            }
        }
    }
    qDebug() << "At end of FlowView::mousePressEvent";
}

void FlowView::mouseMoveEvent(QMouseEvent *event)
{
    QGraphicsView::mouseMoveEvent( event );
}

void FlowView::mouseReleaseEvent(QMouseEvent *event)
{
    QGraphicsView::mouseReleaseEvent( event );
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
BlueprintToolbox::BlueprintToolbox(QWidget *parent) 
    :   QToolBox(parent)
{
}

void BlueprintToolbox::setToolbox( Blueprint::Toolbox::Ptr pToolbox )
{
    while( count() )
        removeItem( 0 );

    m_pToolBox = pToolbox;
    updateToolbox();
    
    onCurrentPaletteChanged( 0 );
}

BlueprintToolbox::ToolboxPanel::ToolboxPanel( BlueprintToolbox& toolbox, 
            Blueprint::Toolbox::Palette::Ptr pPalette, unsigned int uiIndex )
    :   m_toolbox( toolbox ),
        m_pPalette( pPalette ),
        m_strName( pPalette->getName() )
{
    m_pView = new FlowView( m_toolbox, pPalette );
    unsigned int uiActualIndex = m_toolbox.insertItem( uiIndex, m_pView, m_strName.c_str() );
    m_toolbox.setItemEnabled( uiActualIndex, true );
    updateClips();

    QObject::connect( m_pView, &FlowView::OnClipboardAction, &m_toolbox, &BlueprintToolbox::updateToolbox );
    qRegisterMetaType< ClipboardMsg >();
    QObject::connect( m_pView, &FlowView::OnMenu, &m_toolbox, &BlueprintToolbox::OnPaletteMenu, Qt::QueuedConnection );
}
BlueprintToolbox::ToolboxPanel::~ToolboxPanel()
{
    m_pView->clear();
    const int iIndex = m_toolbox.indexOf( m_pView );
    m_toolbox.removeItem( iIndex );
    delete m_pView;
}

void BlueprintToolbox::ToolboxPanel::updateClips()
{
    m_pView->updateClips();
    m_updateTick.update();
}

struct CompareUpdateTimes
{
    inline bool operator()( BlueprintToolbox::PanelMap::const_iterator i1, 
                            Blueprint::Toolbox::Palette::PtrMap::const_iterator i2 ) const
    {
        return i1->second->getUpdateTick() < i2->second->getLastModifiedTick();
    }
};

void BlueprintToolbox::updateToolbox()
{
    ASSERT( m_pToolBox );

    if( m_pToolBox )
    {
        typedef Blueprint::Toolbox::Palette::PtrMap ClipMap;
        const ClipMap& tools = m_pToolBox->get();

        PanelMap removals, updates;
        ClipMap additions;
        generics::matchGetUpdates( m_panels.begin(), m_panels.end(), tools.begin(), tools.end(),
            generics::lessthan( generics::first< PanelMap::const_iterator >(), generics::first< ClipMap::const_iterator >() ),
            CompareUpdateTimes(),
            generics::collect( removals, generics::deref< PanelMap::const_iterator >() ),
            generics::collect( additions, generics::deref< ClipMap::const_iterator >() ),
            generics::collect( updates, generics::deref< PanelMap::const_iterator >() ) );

        for( PanelMap::iterator i = removals.begin(),
            iEnd = removals.end(); i!=iEnd; ++i )
        {
            //remove the panel
            m_panels.erase( i->first );
        }
        removals.clear();
        for( ClipMap::iterator i = additions.begin(),
            iEnd = additions.end(); i!=iEnd; ++i )
        {
            //add new panel
            unsigned int uiIndex = 0u;
            for( ClipMap::const_iterator j = tools.begin(),
                jEnd = tools.end(); j!=jEnd; ++j, ++uiIndex )
            {
                if( j->first == i->first )
                    break;
            }

            ToolboxPanel::Ptr pNewPanel( new ToolboxPanel( *this, i->second, uiIndex ) );
            m_panels.insert( std::make_pair( i->first, pNewPanel ) );
        }
        
        //typedef std::map< std::string, ToolboxPanel::Ptr > PanelMap;
        for( auto& i : updates )
        {
            i.second->updateClips();
        }
    }
}

void BlueprintToolbox::onCurrentPaletteChanged( int index )
{
    if( FlowView* pView = dynamic_cast< FlowView* >( widget( index ) ) )
    {
        if( m_pToolBox ) 
            m_pToolBox->selectPalette( pView->getPalette() );
    }
}

void BlueprintToolbox::OnPaletteMenu( ClipboardMsg msg )
{
    qDebug() << "BlueprintToolbox::OnPaletteMenu()";
    
    QMenu menu( this );

    QObject::connect( menu.addAction( "Delete" ), &QAction::triggered, 
        std::function< void() >( std::bind( &BlueprintToolbox::On_DeleteClip, this, msg ) ));
    QObject::connect( menu.addAction( "Rescan Toolbox" ), SIGNAL(triggered()), this, SLOT(On_RescanToolBoxDir() ) );

    menu.exec( QCursor::pos() );
}

void BlueprintToolbox::On_DeleteClip( ClipboardMsg msg )
{
    msg.pPalette->remove( msg.pSite );
    updateToolbox();
}

void BlueprintToolbox::On_RescanToolBoxDir()
{
    m_panels.clear();
    m_pToolBox->reload();
    updateToolbox();
}
