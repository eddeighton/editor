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
    if( pSite )
    {
        m_pBlueprintEdit.reset();
        VERIFY_RTE( m_itemMap.empty() );
        VERIFY_RTE( m_specMap.empty() );

        m_pBlueprint = pSite;
        m_pBlueprint->init();
        m_pBlueprintEdit = Blueprint::Edit::create( *this, m_pBlueprint );
        setSceneRect( QRectF() );
    }
}

//glyph factory interface
Blueprint::IGlyph::Ptr ClipScene::createControlPoint( Blueprint::ControlPoint* pControlPoint, Blueprint::IGlyph::Ptr pParent )
{
    Blueprint::IGlyph::Ptr pNewGlyph( new GlyphControlPoint( pParent, this, 
        GlyphMap( m_itemMap, m_specMap ), pControlPoint, 128.0f / this->sceneRect().height(), true, m_pToolBox ) );
    return pNewGlyph;
}
/*
Blueprint::IGlyph::Ptr ClipScene::createImage( Blueprint::ImageSpec* pImage, Blueprint::IGlyph::Ptr pParent )
{
    Blueprint::IGlyph::Ptr pNewGlyph( new GlyphImage( pParent, this, 
        GlyphMap( m_itemMap, m_specMap ), pImage, m_pNullContext, true, m_pToolBox ) );
    return pNewGlyph;
}
*/
Blueprint::IGlyph::Ptr ClipScene::createOrigin( Blueprint::Origin* pOrigin, Blueprint::IGlyph::Ptr pParent )
{
    Blueprint::IGlyph::Ptr pNewGlyph( new GlyphOrigin( pParent, this, 
        GlyphMap( m_itemMap, m_specMap ), pOrigin, m_pNullContext, true, m_pToolBox ) );
    return pNewGlyph;
}

Blueprint::IGlyph::Ptr ClipScene::createMarkupPath( Blueprint::MarkupPath* pMarkupPath, Blueprint::IGlyph::Ptr pParent )
{
    Blueprint::IGlyph::Ptr pNewGlyph( new GlyphPath( pParent, this, 
        GlyphMap( m_itemMap, m_specMap ), pMarkupPath, 128.0f / this->sceneRect().height(), true, m_pToolBox ) );
    return pNewGlyph;
}

Blueprint::IGlyph::Ptr ClipScene::createMarkupText( Blueprint::MarkupText* pMarkupText, Blueprint::IGlyph::Ptr pParent )
{
    Blueprint::IGlyph::Ptr pNewGlyph( new GlyphText( pParent, this, 
        GlyphMap( m_itemMap, m_specMap ), pMarkupText, false, m_pToolBox ) );
    return pNewGlyph;
}

void ClipScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    float fXStep = 1.0f;
    while( fXStep / 16.0f < ( 1 ) )
        fXStep *= 2.0f;
    float fYStep = 1.0f;
    while( fYStep / 16.0f < ( 1 ) )
        fYStep *= 2.0f;

    painter->fillRect( rect, QColor( 25, 25, 75 ) );

    static const QColor clr1( 100, 100, 100, 125 );
    static const QColor clr2( 225, 225, 225, 125 );

    //grid lines
    QPen oldPen = painter->pen();
    {
        QPen gridPen;
        gridPen.setStyle( Qt::DotLine );
        gridPen.setWidth( 0.5f );
        painter->setPen( gridPen );

        const float fQuantLeft    =   Math::quantize< float >( rect.left(),      fXStep * 4.0f );
        const float fQuantRight   =   Math::quantize< float >( rect.right(),     fXStep ) + fXStep * 4.0f;
        const float fQuantTop     =   Math::quantize< float >( rect.top(),       fYStep * 4.0f );
        const float fQuantBottom  =   Math::quantize< float >( rect.bottom(),    fYStep ) + fYStep * 4.0f;

        unsigned int uiX = 0u;
        for( float x = fQuantLeft; x <= fQuantRight; x += fXStep, ++uiX )
        {
            if( uiX % 4 )
                gridPen.setColor( clr1 );
            else
                gridPen.setColor( clr2 );
            painter->setPen( gridPen );
            painter->drawLine( QPoint( x, fQuantTop ), QPoint( x, fQuantBottom ) );
        }
        unsigned int uiY = 0u;
        for( float y = fQuantTop; y <= fQuantBottom; y += fYStep, ++uiY )
        {
            if( uiY % 4 )
                gridPen.setColor( clr1 );
            else
                gridPen.setColor( clr2 );
            painter->setPen( gridPen );
            painter->drawLine( QPoint( fQuantLeft, y ), QPoint( fQuantRight, y ) );
        }
    }
    painter->setPen( oldPen );
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
FlowView::FlowItem::FlowItem( FlowView& view, Blueprint::Site::Ptr pSite, Blueprint::Toolbox::Palette::Ptr pPalette )
    :   m_view( view ),
        m_pSite( pSite ),
        m_pPalette( pPalette )
{
    QPixmap buffer( 128, 128 );
    {
        ClipScene tempScene;
        tempScene.setSite( pSite, m_view.m_toolBox.getToolbox() );
        QPainter painter;
        //painter.setRenderHint( QPainter::HighQualityAntialiasing, true );
        painter.begin( &buffer );
        painter.fillRect( 0, 0, 128, 128, QColor( 0,0,0) );
        tempScene.render(&painter );
        painter.resetTransform();
        painter.setPen( QColor( 255, 255, 255 ) );
        
        std::string strName = pSite->Blueprint::Node::getName();
        {
            const std::vector< std::string > prefixes =
            {
                std::string( "room_shape_" ),
                std::string( "room_" ),
                std::string( "object_" ),
                std::string( "ship_" ),
            };
            for( const std::string& prefix : prefixes )
            {
                std::string::const_iterator iSearch =
                    std::search( strName.begin(), strName.end(), 
                                  prefix.begin(), prefix.end() );
                if( iSearch == strName.begin() )
                    strName.erase( 0, prefix.size() );
            }
        }
        
        painter.drawText( 16, 32, 100, 90, Qt::TextWordWrap, strName.c_str() );
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
FlowView::FlowView( BlueprintToolbox& toolbox, Blueprint::Toolbox::Palette::Ptr pPalette ) 
    :   QGraphicsView( &toolbox ),
        m_toolBox( toolbox ),
        m_pPalette( pPalette )
{
    setScene( new QGraphicsScene( this ) );
    setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    setTransformationAnchor( QGraphicsView::AnchorViewCenter );
    
    QPen selectionPen( Qt::SolidLine );
    selectionPen.setWidth( 2.0f );
    selectionPen.setBrush( QColor( 0,0,255 ) );
    m_pSelectionRectItem = scene()->addRect( QRect(0,0,0,0), selectionPen, QBrush( QColor( 0,0,150,100 ) ) );
    m_pSelectionRectItem->setZValue( 1.0f );
    m_pSelectionRectItem->setVisible( false );

    updateClips();
}

FlowView::~FlowView()
{
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
    float fItemWidth = 128.0f;
    float fItemHeight = 128.0f;
    float fItemSpacing = 8.0f;
    int iStride = std::max( 1.0f, viewport()->rect().width() / fItemWidth );

    //now calculate the positions and sizes
    FlowItem::PtrVector::iterator i = items.begin();
    int y = 0;
    for( ; i != items.end(); ++y)
    {
        for( int x = 0; x < iStride && i != items.end(); ++x, ++i )
            (*i)->updatePosition( x, y, fItemWidth, fItemHeight, fItemSpacing );
    }

    setSceneRect( 0.0f, 0.0f, iStride * ( fItemWidth + fItemSpacing ), y * ( fItemHeight + fItemSpacing ) );

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
        std::function< void() >( std::bind( &BlueprintToolbox::On_DeleteClip,       this, msg ) ));
    //QObject::connect( menu.addAction( "Delete All" ),       &QAction::triggered,
    //    std::function< void() >( std::bind( &BlueprintToolbox::On_DeleteAllClips,   this, msg ) ) );
    //QObject::connect( menu.addAction( "Copy To Clipboard" ),&QAction::triggered,
    //    std::function< void() >( std::bind( &BlueprintToolbox::On_CopyToClipboard,  this, msg ) ) );
    //QObject::connect( menu.addAction( "Save Clip As" ),     &QAction::triggered,
    //    std::function< void() >( std::bind( &BlueprintToolbox::On_SaveClipAs,       this, msg ) ) );
    //                
    //QObject::connect( menu.addAction( "Set Toolbox Dir" ),  SIGNAL(triggered()), this, SLOT(On_SetToolboxDir() )        );
    //QObject::connect( menu.addAction( "Rescan Toolbox" ),   SIGNAL(triggered()), this, SLOT(On_RescanToolBoxDir() )     );

    menu.exec( QCursor::pos() );
}

void BlueprintToolbox::On_DeleteClip( ClipboardMsg msg )
{
    msg.pPalette->remove( msg.pSite );
    updateToolbox();
}
/*
void BlueprintToolbox::On_DeleteAllClips( ClipboardMsg msg )
{
    m_pToolBox->remove( msg.pPalette );
    updateToolbox();
}

void BlueprintToolbox::On_CopyToClipboard( ClipboardMsg msg )
{
    updateToolbox();
}
void BlueprintToolbox::On_SaveClipAs( ClipboardMsg msg )
{
    updateToolbox();
}
void BlueprintToolbox::On_SetToolboxDir()
{
    updateToolbox();
}
void BlueprintToolbox::On_RescanToolBoxDir()
{
    updateToolbox();
}*/