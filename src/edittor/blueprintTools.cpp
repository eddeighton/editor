#include "blueprintTools.h"

#include "blueprintview.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//selection handling
const Selectable* Selection::glyphToSelectable( const Blueprint::IGlyph* pGlyph )
{
    return dynamic_cast< const Selectable* >( pGlyph );
}

Selectable* Selection::glyphToSelectable( Blueprint::IGlyph* pGlyph )
{
    return dynamic_cast< Selectable* >( pGlyph );
}

Selection::Mode Selection::getSelectionMode( QMouseEvent *event )
{
    Selection::Mode mode = Selection::eSet;
    if( event->modifiers() & Qt::ControlModifier )      mode = Selection::eXAND;
    else if( event->modifiers() & Qt::ShiftModifier )   mode = Selection::eOR;
    else if( event->modifiers() & Qt::AltModifier )     mode = Selection::eXOR;
    return mode;
}


Selection::Selection( BlueprintView& view, Mode mode, const SelectionSet& interactiveSelection )
    :   m_view( view ),
        m_selectionMode( mode ),
        m_initialSelection( m_view.getSelection() )
{
    update( interactiveSelection );
}

void Selection::update( const SelectionSet& interactive )
{
    //calculate the new selection state
    SelectionSet selection;
    switch( m_selectionMode )
    {
        case Selection::eSet:
            selection = interactive;
            break;
        case Selection::eOR:
            selection.insert( interactive.begin(), interactive.end() );
            selection.insert( m_initialSelection.begin(), m_initialSelection.end() );
            break;
        case Selection::eXAND:
            selection = m_initialSelection;
            for( SelectionSet::iterator i = interactive.begin(),
                 iEnd = interactive.end(); i!=iEnd; ++i )
                selection.erase( *i );
            break;
        case Selection::eXOR:
            selection = m_initialSelection;
            for( SelectionSet::iterator i = interactive.begin(),
                 iEnd = interactive.end(); i!=iEnd; ++i )
            {
                if( selection.find( *i ) == selection.end() )
                    selection.insert( *i );
                else
                    selection.erase( *i );
            }
            break;
    }

    //apply the new result selection to the control points...
    m_view.setSelected( selection );
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
Selection_Rect::Selection_Rect( BlueprintView& view, Selection::Mode mode, float fZoomLevel, const QPointF& downPos, const QColor& colour )
    :   m_view( view ),
        m_selectionRect( downPos, downPos ),
        m_selection( view, mode, view.getSelectedByRect( m_selectionRect ) ),
        m_downPos( downPos )
{
    m_selectionPen.setStyle(        Qt::SolidLine);
    m_selectionPen.setWidth(        2.0f / fZoomLevel );
    m_selectionPen.setBrush(        QColor( colour.red(), colour.green(), colour.blue() ) );
    m_selectionPen.setCapStyle(     Qt::RoundCap);
    m_selectionPen.setJoinStyle(    Qt::RoundJoin);
    m_pSelectionRectItem = m_view.scene()->addRect( m_selectionRect, m_selectionPen, QBrush( colour ) );
}

void Selection_Rect::update( const QPointF& pos )
{
    m_selectionRect = QRectF( m_downPos, pos ).normalized();
    m_pSelectionRectItem->setRect( m_selectionRect );
    m_selection.update( m_view.getSelectedByRect( m_selectionRect ) );
}

Selection_Rect::~Selection_Rect()
{
    m_view.scene()->removeItem( m_pSelectionRectItem );
    delete m_pSelectionRectItem;
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void filterSelectionByDepth( SelectionSet& selection, unsigned int uiDepth )
{
    for( SelectionSet::iterator 
        i = selection.begin(),
        iEnd = selection.end(); i!=iEnd; )
    {
        if( Selection::glyphToSelectable( *i )->getDepth() != uiDepth )
            i = selection.erase( i );
        else
            ++i;
    }
}

unsigned int findLowestDepth( const SelectionSet& selection )
{
    unsigned int uiLowest = 100u;
    for( SelectionSet::iterator 
        i = selection.begin(),
        iEnd = selection.end(); i!=iEnd; ++i )
    {
        Selectable* pSelectable = Selection::glyphToSelectable( *i );
        ASSERT( pSelectable );
        if( pSelectable )
        {
            if( uiLowest > pSelectable->getDepth() )
                uiLowest = pSelectable->getDepth();
        }
    }
    return uiLowest;
}

Selection_Path::Selection_Path( BlueprintView& view, Selection::Mode mode, float fZoomLevel, const QPointF& downPos, const QColor& colour )
    :   m_view( view ),
        m_path( downPos ),
        m_selection( view, mode, view.getSelectedByPath( m_path ) ),
        m_downPos( downPos ),
        m_uiDepth( findLowestDepth( m_selection.getInitial() ) )
{
    m_path.setFillRule( Qt::WindingFill );

    m_selectionPen.setStyle(        Qt::SolidLine );
    m_selectionPen.setWidth(        2.0f / fZoomLevel );
    m_selectionPen.setBrush(        QColor( colour.red(), colour.green(), colour.blue() ) );
    m_selectionPen.setCapStyle(     Qt::RoundCap);
    m_selectionPen.setJoinStyle(    Qt::RoundJoin);

    m_pSelectionPath = m_view.scene()->addPath( m_path, m_selectionPen, QBrush( colour ) );

    filterSelectionByDepth( m_selection.getInitial(), m_uiDepth );   
}

void Selection_Path::update( const QPointF& pos )
{
    m_path.lineTo( pos );
    QPainterPath p = m_path;
    p.closeSubpath();
    m_pSelectionPath->setPath( p );
    
    SelectionSet selection = m_view.getSelectedByPath( m_path );
    if( !selection.empty() )
    {
        SelectionSet actualSelection = m_view.getSelection();
        if( actualSelection.empty() )
        {
            //if the actual selection is empty then can the lowest depth
            m_uiDepth = findLowestDepth( selection );
            filterSelectionByDepth( selection, m_uiDepth );  
            m_selection.getInitial() = selection;
            m_selection.update( selection );
        }
        else
        {
            //otherwise stick to it
            filterSelectionByDepth( selection, m_uiDepth );   
            m_selection.update( selection );
        }
    }
}

Selection_Path::~Selection_Path()
{
    m_view.scene()->removeItem( m_pSelectionPath );
    delete m_pSelectionPath;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
SelectTool::SelectTool( BlueprintView& view )
    :   m_view( view ),
        m_toolMode( eCmd )
{
}

void SelectTool::mouseHover( QMouseEvent* event )
{
    if( m_view.findSelectableTopmostGlyph( event->pos() ) )
        m_view.setCursor( Qt::SizeAllCursor );
    else
        m_view.setCursor( Qt::ArrowCursor );
}

void SelectTool::mousePressEvent(QMouseEvent *event)
{
    ASSERT( m_view.m_pActiveContext );
    
    const QVector2D q = m_view.calculateQuantisation();
    if( event->button() == Qt::RightButton )
    {
        m_view.setCursor( Qt::ArrowCursor );
        m_toolMode = eSelectNodes;
        m_pSelection.reset( new Selection_Path( 
            m_view,
            Selection::getSelectionMode( event ), 
            m_view.m_v2ZoomLevel.y(), 
            m_view.mapToScene( event->pos() ) ) );
    }
    else if( event->button() == Qt::LeftButton )
    {
        if( Blueprint::IGlyph* pGlyph = m_view.findSelectableTopmostGlyph( event->pos() ) )
        {
            SelectionSet currentSelection = m_view.getSelection();
            if( currentSelection.find( pGlyph ) == currentSelection.end() )
            {
                currentSelection.clear();
                currentSelection.insert( pGlyph );
                m_view.setSelected( currentSelection );
            }
            const QPointF pos = m_view.mapToActiveContext( m_view.mapToScene( event->pos() ) );

            if( event->modifiers() & Qt::ControlModifier )
            {
                if( m_pInteraction = m_view.m_pActiveContext->cmd_paste( currentSelection, pos.x(), pos.y(), q.x(), q.y() ) )
                {
                    m_view.setCursor( Qt::DragCopyCursor );
                    m_toolMode = eDragCopyCmd;
                }
            }
            if( !m_pInteraction )
            {
                m_pInteraction = m_view.m_pActiveContext->interaction_start( m_view.getMode(), pos.x(), pos.y(), q.x(), q.y(), pGlyph, currentSelection );
                m_toolMode = eCmd;
                m_view.setCursor( Qt::SizeAllCursor );
            }
        }
        else
        {
            m_view.setCursor( Qt::ArrowCursor );
            m_toolMode = eSelectNodes;
            m_pSelection.reset( new Selection_Rect( 
                m_view, 
                Selection::getSelectionMode( event ), 
                m_view.m_v2ZoomLevel.y(),
                m_view.mapToScene( event->pos() ) ) );
        }
    }
}
void SelectTool::mouseMoveEvent(QMouseEvent *event)
{
    switch( m_toolMode )
    {
        case eSelectNodes:
            if( m_pSelection.get() )
            {
                m_pSelection->update( m_view.mapToScene( event->pos() ) );
                m_view.setCursor( Qt::ArrowCursor );
            }
            break;
        case eCmd:
            m_view.setCursor( Qt::SizeAllCursor );
            if( m_pInteraction )
            {
                QPointF pos = m_view.mapToActiveContext( m_view.mapToScene( event->pos() ) );
                m_pInteraction->OnMove( pos.x(), pos.y() );
                m_view.CalculateOversizedSceneRect();
            }
            break;
        case eDragCopyCmd:
            m_view.setCursor( Qt::DragCopyCursor );
            if( m_pInteraction )
            {
                QPointF pos = m_view.mapToActiveContext( m_view.mapToScene( event->pos() ) );
                m_pInteraction->OnMove( pos.x(), pos.y() );
                m_view.CalculateOversizedSceneRect();
            }
            break;
    }
}
void SelectTool::mouseReleaseEvent(QMouseEvent*)
{
    reset();
}

void SelectTool::keyPressEvent(QKeyEvent*)
{
}
void SelectTool::reset()
{
    m_pInteraction.reset();
    m_pSelection.reset();
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
PenTool::PenTool( BlueprintView& view )
    :   m_view( view ),
        m_toolMode( eDraw )
{
}

void PenTool::mouseHover( QMouseEvent* event )
{
    if( m_view.findSelectableTopmostGlyph( event->pos() ) )
        m_view.setCursor( Qt::SizeAllCursor );
    else
        m_view.setCursor( Qt::CrossCursor );
}
void PenTool::mousePressEvent(QMouseEvent *event)
{
    ASSERT( m_view.m_pActiveContext );
    
    const QVector2D q = m_view.calculateQuantisation();
    if( event->button() == Qt::LeftButton )
    {
        const QPointF pos = m_view.mapToActiveContext( m_view.mapToScene( event->pos() ) );
        if( Blueprint::IGlyph* pGlyph = m_view.findSelectableTopmostGlyph( event->pos() ) )
        {
            SelectionSet selection;
            if( event->modifiers() & Qt::ControlModifier )
            {
                if( m_pInteraction = m_view.m_pActiveContext->cmd_paste( pGlyph, pos.x(), pos.y(), q.x(), q.y() ) )
                {
                    m_toolMode = eDragCopy;
                    m_view.setCursor( Qt::DragCopyCursor );
                }
            }
            if( !m_pInteraction )
            {
                m_pInteraction = m_view.m_pActiveContext->interaction_start( m_view.getMode(), pos.x(), pos.y(), q.x(), q.y(), pGlyph, selection );
                m_toolMode = eCmd;
                m_view.setCursor( Qt::SizeAllCursor );
            }
            m_view.CalculateOversizedSceneRect();
        }
        else
        {
            //draw new space
            m_toolMode = eDraw;
            m_view.setCursor( Qt::CrossCursor );
            m_pInteraction = m_view.m_pActiveContext->interaction_draw( m_view.getMode(), pos.x(), pos.y(), q.x(), q.y(), m_view.getCurrentPaletteItem() );

            m_view.CalculateOversizedSceneRect();
        }
    }
    else if( event->button() == Qt::RightButton )
    {
        m_view.setCursor( Qt::ArrowCursor );
        m_toolMode = eDelete;
        m_pSelection.reset( new Selection_Rect( m_view, Selection::getSelectionMode( event ), m_view.m_v2ZoomLevel.y(),
            m_view.mapToScene( event->pos() ), QColor( 200, 0, 0, 100) ) );
    }
}
void PenTool::mouseMoveEvent(QMouseEvent *event)
{
    switch( m_toolMode )
    {
        case eDraw:
            m_view.setCursor( Qt::CrossCursor );
            if( m_pInteraction )
            {
                QPointF pos = m_view.mapToActiveContext( m_view.mapToScene( event->pos() ) );
                m_pInteraction->OnMove( pos.x(), pos.y() );
                m_view.CalculateOversizedSceneRect();
            }
            break;
        case eCmd:
            if( m_pInteraction )
            {
                m_view.setCursor( Qt::SizeAllCursor );
                QPointF pos = m_view.mapToActiveContext( m_view.mapToScene( event->pos() ) );
                m_pInteraction->OnMove( pos.x(), pos.y() );
                m_view.CalculateOversizedSceneRect();
            }
            break;
        case eDragCopy:
            if( m_pInteraction )
            {
                m_view.setCursor( Qt::DragCopyCursor );
                QPointF pos = m_view.mapToActiveContext( m_view.mapToScene( event->pos() ) );
                m_pInteraction->OnMove( pos.x(), pos.y() );
                m_view.CalculateOversizedSceneRect();
            }
            break;
        case eDelete:
            if( m_pSelection.get() )
            {
                m_pSelection->update( m_view.mapToScene( event->pos() ) );
                m_view.setCursor( Qt::ArrowCursor );
            }
            break;
    }
}
void PenTool::mouseReleaseEvent(QMouseEvent*)
{
    switch( m_toolMode )
    {
        case eDelete:
            m_view.OnCmd_Delete();
            break;
        default:
            break;
    }
    reset();
}
void PenTool::keyPressEvent(QKeyEvent*)
{
}
void PenTool::reset()
{
    m_pInteraction.reset();
    m_pSelection.reset();
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//ContextTool::ContextTool( BlueprintView& view )
//    :   PenTool( view )
//{
//}
//
//void ContextTool::mousePressEvent(QMouseEvent *event)
//{
//    ASSERT( m_view.m_pActiveContext );
//    
//    const QVector2D q = m_view.calculateQuantisation();
//    if( event->button() == Qt::LeftButton )
//    {
//        const QPointF pos = m_view.mapToActiveContext( m_view.mapToScene( event->pos() ) );
//        if( Blueprint::IGlyph* pGlyph = m_view.findSelectableTopmostGlyph( event->pos() ) )
//        {
//            SelectionSet selection;
//            if( event->modifiers() & Qt::ControlModifier )
//            {
//                if( m_pInteraction = m_view.m_pActiveContext->cmd_paste( pGlyph, pos.x(), pos.y(), q.x(), q.y() ) )
//                {
//                    m_toolMode = eDragCopy;
//                    m_view.setCursor( Qt::DragCopyCursor );
//                }
//            }
//            if( !m_pInteraction )
//            {
//                m_pInteraction = m_view.m_pActiveContext->interaction_tool( pos.x(), pos.y(), q.x(), q.y(), pGlyph, selection, m_toolID );
//                m_toolMode = eCmd;
//                m_view.setCursor( Qt::SizeAllCursor );
//            }
//            m_view.CalculateOversizedSceneRect();
//        }
//        else
//        {
//            //draw new space
//            m_toolMode = eDraw;
//            m_view.setCursor( Qt::CrossCursor );
//            m_pInteraction = m_view.m_pActiveContext->interaction_tool_draw( pos.x(), pos.y(), q.x(), q.y(), m_view.getCurrentPaletteItem(), m_toolID );
//
//            m_view.CalculateOversizedSceneRect();
//        }
//    }
//    else if( event->button() == Qt::RightButton )
//    {
//        m_view.setCursor( Qt::ArrowCursor );
//        m_toolMode = eDelete;
//        m_pSelection.reset( new Selection_Rect( m_view, Selection::getSelectionMode( event ), m_view.m_v2ZoomLevel.y(),
//            m_view.mapToScene( event->pos() ), QColor( 200, 0, 0, 100) ) );
//    }
//}
/*
void ContextTool::mouseHover( QMouseEvent*  )
{
    m_view.setCursor( Qt::ArrowCursor );
}
void ContextTool::mousePressEvent(QMouseEvent* event)
{
    ASSERT( m_view.m_pActiveContext );

    if( event->button() == Qt::LeftButton )
    {
        const QPointF pos = m_view.mapToActiveContext( m_view.mapToScene( event->pos() ) );
        const QVector2D q = m_view.calculateQuantisation();
        m_view.setCursor( Qt::ArrowCursor );
        m_pInteraction = m_view.m_pActiveContext->interaction_tool( pos.x(), pos.y(), q.x(), q.y(), m_toolID );
        m_view.CalculateOversizedSceneRect();
    }
}
void ContextTool::mouseMoveEvent(QMouseEvent* event)
{
    m_view.setCursor( Qt::ArrowCursor );
    if( m_pInteraction )
    {
        QPointF pos = m_view.mapToActiveContext( m_view.mapToScene( event->pos() ) );
        m_pInteraction->OnMove( pos.x(), pos.y() );
        m_view.CalculateOversizedSceneRect();
    }
}
void ContextTool::mouseReleaseEvent(QMouseEvent*)
{
    reset();
}
void ContextTool::keyPressEvent(QKeyEvent*)
{
}
void ContextTool::reset()
{
    m_pInteraction.reset();
}

*/





















