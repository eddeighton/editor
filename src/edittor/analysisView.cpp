

#include "analysisView.h"

struct PainterImpl : public Blueprint::Analysis::IPainter
{
    QPainterPath& m_path;
    
    PainterImpl( QPainterPath& path )
        :   m_path( path )
    {
    }
    
    virtual void moveTo( float x, float y )
    {
        m_path.moveTo( x, y );
    }
    
    virtual void lineTo( float x, float y )
    {
        m_path.lineTo( x, y );
    }
};

static QColor g_pathColor( 155,155,155,125 );
static float g_pathWidth( 4.0f );

AnalysisView::AnalysisView( 
            Blueprint::Analysis::Ptr pAnalysis, 
            QGraphicsScene* pScene,
            Blueprint::Toolbox::Ptr pToolBoxPtr )
   :    m_pAnalysis( pAnalysis ),
        m_pScene( pScene ),
        m_pToolBoxPtr( pToolBoxPtr )
{
    if( m_pToolBoxPtr )
    {
        m_pToolBoxPtr->getConfigValue( ".visibility.paths.colour", g_pathColor );
        m_pToolBoxPtr->getConfigValue( ".visibility.paths.width", g_pathWidth );
    }
    
    PainterImpl painter( m_path );
    m_pAnalysis->renderFloor( painter );
    
    //Qt::NoPen	            0	no line at all. For example, QPainter::drawRect() fills but does not draw any boundary line.
    //Qt::SolidLine	        1	A plain line.
    //Qt::DashLine	        2	Dashes separated by a few pixels.
    //Qt::DotLine	        3	Dots separated by a few pixels.
    //Qt::DashDotLine	    4	Alternate dots and dashes.
    //Qt::DashDotDotLine    5	One dash, two dots, one dash, two dots.

    m_pItem = new QGraphicsPathItem( m_path, nullptr );
    m_pItem->setPen( QPen( QBrush( g_pathColor ), g_pathWidth, Qt::SolidLine ) );
    m_pItem->setBrush( QBrush( Qt::NoBrush ) );
    m_pItem->setZValue( 0.1f );
    
    m_pScene->addItem( m_pItem );
}

AnalysisView::~AnalysisView()
{
    delete m_pItem;
}
    
void AnalysisView::setVisible( bool bVisible )
{
    m_pItem->setVisible( bVisible );
}