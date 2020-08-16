#include "glyphs.h"

#include <QGraphicsOpacityEffect>
#include <QBitmap>

void Selectable::setSelected( bool bSelected )
{
    m_bSelected = bSelected;
}

void cleanUpItem( QGraphicsItem* pItem, GlyphMap& map, const Blueprint::GlyphSpec* pSpec, QGraphicsScene*  )
{
    if( pItem )
    {
        ASSERT( pItem->childItems().isEmpty() );
        map.erase( pItem, pSpec );
        delete pItem;
    }
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
GlyphControlPoint::GlyphControlPoint( Blueprint::IGlyph::Ptr pParent, QGraphicsScene* pScene,
                                      GlyphMap map, Blueprint::ControlPoint* pControlPoint, 
                                      float fZoom, bool bShouldRender )
    :   Blueprint::GlyphControlPoint( pControlPoint, pParent ),
        Selectable( calculateDepth( pControlPoint ) ),
        Renderable( bShouldRender ),
        m_pScene( pScene ),
        m_map( map ),
        m_fSize( 8.0f / fZoom ),
        m_fWidth( 0.5f / fZoom )
{
    QGraphicsItem* pParentItem = m_map.findItem( getControlPoint()->getParent() );
    m_pItem = new QGraphicsEllipseItem( -( m_fSize / 2.0f ), -( m_fSize / 2.0f ), m_fSize, m_fSize, pParentItem );
    if( !pParentItem ) m_pScene->addItem( m_pItem );
    m_pItem->setPos( getControlPoint()->getX(), getControlPoint()->getY() );
    m_pItem->setPen( QPen( Qt::NoPen ) );
    m_pItem->setBrush( QBrush( QColor( 255, 255, 255 ) ) );
    m_pItem->setZValue( 2.0f );
    updateColours();
    m_map.insert( m_pItem, getControlPoint(), this );
}

GlyphControlPoint::~GlyphControlPoint()
{
    cleanUpItem( m_pItem, m_map, m_pGlyphSpec /*getControlPoint()*/, m_pScene );
}

void GlyphControlPoint::updateColours()
{
    m_pItem->setBrush( isSelected() ?    QBrush( Qt::green ) :
                                        QBrush( Qt::black ) );
}

void GlyphControlPoint::setSelected( bool bSelected )
{
    const bool bUpdate = isSelected() != bSelected;
    Selectable::setSelected( bSelected );
    if( bUpdate )
        updateColours();
}

void GlyphControlPoint::update()
{
    m_pItem->setPos( getControlPoint()->getX(), getControlPoint()->getY() );
    m_pItem->setRect( -( m_fSize / 2.0f ), -( m_fSize / 2.0f ), m_fSize, m_fSize );
}

void GlyphControlPoint::OnNewZoomLevel( float fZoom )
{
    m_fSize = 8.0f / fZoom;
    m_fWidth = 0.5f / fZoom;
    update();
    updateColours();
}

QGraphicsEllipseItem* GlyphControlPoint::getItem() const
{
    return m_pItem;
}

void GlyphControlPoint::setShouldRender( bool bShouldRender )
{
    m_bShouldRender = bShouldRender;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
static const QColor g_pathColor( 255,255,255,125 );
static const float g_pathWidth( 4.0f );

GlyphPath::GlyphPath( Blueprint::IGlyph::Ptr pParent, QGraphicsScene* pScene,
                      GlyphMap map, Blueprint::MarkupPath* pPath, 
                      float fZoom, bool bShouldRender )
    :   Blueprint::GlyphPath( pPath, pParent ),
        Renderable( bShouldRender ),
        m_pScene( pScene ),
        m_map( map ),
        m_fSize( g_pathWidth / fZoom ),
        m_pItem( 0u )
{
    //convert to painter path
    matchPath( m_path );
    QGraphicsItem* pParentItem = m_map.findItem( getMarkupPath()->getParent() );
    m_pItem = new QGraphicsPathItem( m_path, pParentItem );
    if( !pParentItem ) m_pScene->addItem( m_pItem );
    m_pItem->setPen( QPen( QBrush( g_pathColor ), m_fSize, Qt::DashDotLine ) );
    m_pItem->setBrush( QBrush( Qt::NoBrush ) );
    m_pItem->setZValue( 1.5f );
    m_map.insert( m_pItem, getMarkupPath(), this );
}

GlyphPath::~GlyphPath()
{
    cleanUpItem( m_pItem, m_map, getMarkupPath(), m_pScene );
}

void GlyphPath::OnNewZoomLevel( float fZoom )
{
    m_fSize = g_pathWidth / fZoom;
    update();
}

void GlyphPath::matchPath( QPainterPath& path )
{
    const Blueprint::MarkupPath::PathCmdVector& cmds =
            getMarkupPath()->getCmds();
    for( Blueprint::MarkupPath::PathCmdVector::const_iterator
         i = cmds.begin(), iEnd = cmds.end(); i!=iEnd; ++i )
    {
        switch( i->cmd )
        {
            case Blueprint::MarkupPath::Cmd::path_cmd_stop:
                break;
            case Blueprint::MarkupPath::Cmd::path_cmd_move_to:
                path.moveTo( i->x, i->y );
                break;
            case Blueprint::MarkupPath::Cmd::path_cmd_line_to:
                path.lineTo( i->x, i->y );
                break;
            case Blueprint::MarkupPath::Cmd::path_cmd_curve3:
                {
                    qDebug() << "Error attempting to render MarkupPath::Cmd::path_cmd_curve3";
                }
                break;
            case Blueprint::MarkupPath::Cmd::path_cmd_curve4:
                {
                    const float c1x = i->x, c1y = i->y;
                    ++i;
                    const float c2x = i->x, c2y = i->y;
                    ++i;
                    path.cubicTo( c1x, c1y, c2x, c2y, i->x, i->y );
                }
                break;
            case Blueprint::MarkupPath::Cmd::path_cmd_curveN:
                {
                    qDebug() << "Error attempting to render MarkupPath::Cmd::path_cmd_curveN";
                }
                break;
            case Blueprint::MarkupPath::Cmd::path_cmd_catrom:
                {
                    qDebug() << "Error attempting to render MarkupPath::Cmd::path_cmd_catrom";
                }
                break;
            case Blueprint::MarkupPath::Cmd::path_cmd_ubspline:
                {
                    qDebug() << "Error attempting to render MarkupPath::Cmd::path_cmd_ubspline";
                }
                break;
            case Blueprint::MarkupPath::Cmd::path_cmd_end_poly | Blueprint::MarkupPath::Cmd::path_flags_close:
                path.closeSubpath();
                break;
            default:
                break;
        }
    }
}

void GlyphPath::update()
{
    QPainterPath newPath;
    matchPath( newPath );
    m_path = newPath;
    m_pItem->setPath( m_path );
    m_pItem->setPen( QPen( QBrush( g_pathColor ), m_fSize, Qt::DashDotLine ) );
}

void GlyphPath::setShouldRender( bool bShouldRender )
{
    m_bShouldRender = bShouldRender;
}

QVector< QRgb > GlyphImage::m_coloursNormal;
QVector< QRgb > GlyphImage::m_coloursSelected;
QVector< QRgb > GlyphImage::m_coloursActiveContext;

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
GlyphImage::GlyphImage( Blueprint::IGlyph::Ptr pParent, QGraphicsScene* pScene,
                        GlyphMap map, Blueprint::ImageSpec* pImage, 
                        Blueprint::IEditContext*& pActiveContext, bool bShouldRender )
    :   Blueprint::GlyphImage( pImage, pParent ),
        Selectable( calculateDepth( pImage ) ),
        Renderable( bShouldRender ),
        m_pScene( pScene ),
        m_map( map ),
        m_pItem( 0u ),
        m_pActiveContext( pActiveContext ),
        m_bActiveContext( false )
{
    static bool bColourTablesInit = true;
    if( bColourTablesInit )
    {
        bColourTablesInit = false;
        m_coloursNormal.push_back( 0u );
        m_coloursNormal.push_back( 0xA03030A0 );
        m_coloursNormal.push_back( 0xA06F6FA0 );

        m_coloursSelected.push_back( 0u );
        m_coloursSelected.push_back( 0xA03030FF );
        m_coloursSelected.push_back( 0xA06F6FFF );

        m_coloursActiveContext.push_back( 0u );
        m_coloursActiveContext.push_back( 0x30A03000 );
        m_coloursActiveContext.push_back( 0x30FF6F00 );
    }

    if( Blueprint::NavBitmap::Ptr pBuffer = getImageSpec()->getBuffer() )
    {
        QImage m_image( pBuffer->get(),pBuffer->getWidth(),pBuffer->getHeight(),pBuffer->getStride(),QImage::Format_Indexed8 );
        m_image.setColorTable( m_coloursNormal );
        m_pixelMap = QPixmap::fromImage( m_image, Qt::ColorOnly );
        setOrCreateImageItem();
    }
}

GlyphImage::~GlyphImage()
{
    cleanUpItem( m_pItem, m_map, getImageSpec(), m_pScene );
}

bool GlyphImage::isActiveContext() const
{
    return ( m_pActiveContext && m_pActiveContext->getImageSpec() == getImageSpec() ) ? true : false;
}

void GlyphImage::setSelected( bool bSelected )
{
    const bool bUpdate = 
        ( isSelected() != bSelected ) || 
        ( m_bActiveContext != isActiveContext() );
    Selectable::setSelected( bSelected );
    if( bUpdate )
    {
        m_bActiveContext = isActiveContext();
        if( m_bActiveContext )
        {
            if( Blueprint::NavBitmap::Ptr pBuffer = getImageSpec()->getBuffer() )
            {
                QImage m_image( pBuffer->get(),pBuffer->getWidth(),pBuffer->getHeight(),pBuffer->getStride(),QImage::Format_Indexed8 );
                m_image.setColorTable( m_coloursActiveContext );
                m_pixelMap = QPixmap::fromImage( m_image, Qt::ColorOnly );
                setOrCreateImageItem();
            }
        }
        else if( isSelected() )
        {
            if( Blueprint::NavBitmap::Ptr pBuffer = getImageSpec()->getBuffer() )
            {
                QImage m_image( pBuffer->get(),pBuffer->getWidth(),pBuffer->getHeight(),pBuffer->getStride(),QImage::Format_Indexed8 );
                m_image.setColorTable( m_coloursSelected );
                m_pixelMap = QPixmap::fromImage( m_image, Qt::ColorOnly );
                setOrCreateImageItem();
            }
        }
        else
        {
            if( Blueprint::NavBitmap::Ptr pBuffer = getImageSpec()->getBuffer() )
            {
                QImage m_image( pBuffer->get(),pBuffer->getWidth(),pBuffer->getHeight(),pBuffer->getStride(),QImage::Format_Indexed8 );
                m_image.setColorTable( m_coloursNormal );
                m_pixelMap = QPixmap::fromImage( m_image, Qt::ColorOnly );
                setOrCreateImageItem();
            }
        }
    }
}

void GlyphImage::setOrCreateImageItem()
{
    if( !m_pItem )
    {
        QGraphicsItem* pParentItem = m_map.findItem( getImageSpec()->getParent() );
        m_pItem = new QGraphicsPixmapItem( m_pixelMap, pParentItem );
        if( !pParentItem ) m_pScene->addItem( m_pItem );
        m_map.insert( m_pItem, getImageSpec(), this );
        m_pItem->setZValue( 0.0f );
        m_pItem->setPos( getImageSpec()->getX(), getImageSpec()->getY() );
        m_pItem->setOffset( getImageSpec()->getOffsetX(), getImageSpec()->getOffsetY() );
    }
    else
        m_pItem->setPixmap( m_pixelMap );
}

void GlyphImage::update()
{
    if( Blueprint::NavBitmap::Ptr pBuffer = getImageSpec()->getBuffer() )
    {
        if( ( m_lastUpdateTick < pBuffer->getLastUpdateTick() ) || !m_pItem || 
            ( isActiveContext() != m_bActiveContext ) )
        {
            QImage m_image( pBuffer->get(), pBuffer->getWidth(),
                              pBuffer->getHeight(), pBuffer->getStride(), QImage::Format_Indexed8 );
            m_bActiveContext = isActiveContext();
            if( m_bActiveContext )
                m_image.setColorTable( m_coloursActiveContext );
            else if( isSelected() )
                m_image.setColorTable( m_coloursSelected );
            else
                m_image.setColorTable( m_coloursNormal );

            m_pixelMap.convertFromImage( m_image, Qt::ColorOnly );
            setOrCreateImageItem();
            m_lastUpdateTick.update();
        }
        m_pItem->setPos( getImageSpec()->getX(), getImageSpec()->getY() );
        m_pItem->setOffset( getImageSpec()->getOffsetX(), getImageSpec()->getOffsetY() );
    }
    else if( m_pItem )
    {
        cleanUpItem( m_pItem, m_map, getImageSpec(), m_pScene );
        m_pItem = 0u;
    }
}

void GlyphImage::setShouldRender( bool bShouldRender )
{
    m_bShouldRender = bShouldRender;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
GlyphText::GlyphText( Blueprint::IGlyph::Ptr pParent, QGraphicsScene* pScene, 
                     GlyphMap map, Blueprint::MarkupText* pText, bool bShouldRender )
    :   Blueprint::GlyphText( pText, pParent ),
        Renderable( bShouldRender ),
        m_pScene( pScene ),
        m_map( map ),
        m_pItem( 0u )
{
    //convert to painter path
    QGraphicsItem* pParentItem = m_map.findItem( getMarkupText()->getParent() );
    m_pItem = new QGraphicsSimpleTextItem( getMarkupText()->getText().c_str(), pParentItem );
    if( !pParentItem ) m_pScene->addItem( m_pItem );
    m_pItem->setZValue( 0.9f );
    m_pItem->setFlag( QGraphicsItem::ItemIgnoresTransformations );
    m_pItem->setPos( getMarkupText()->getX(), getMarkupText()->getY() );
    m_pItem->setBrush( QBrush( QColor( 255, 255, 255 ) ) );
    m_map.insert( m_pItem, pText, this );

    if( !m_bShouldRender && m_pItem->isVisible() )
        m_pItem->hide();
}

GlyphText::~GlyphText()
{
    cleanUpItem( m_pItem, m_map, getMarkupText(), m_pScene );
}

void GlyphText::update()
{
    m_pItem->setText( getMarkupText()->getText().c_str() );
    m_pItem->setPos( getMarkupText()->getX(), getMarkupText()->getY() );
}

void GlyphText::setShouldRender( bool bShouldRender )
{
    m_bShouldRender = bShouldRender;
    if( m_bShouldRender && !m_pItem->isVisible() )
        m_pItem->show();
    else if( !m_bShouldRender && m_pItem->isVisible() )
        m_pItem->hide();
}




