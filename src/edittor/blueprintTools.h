#ifndef BLUEPRINTTOOLS_H
#define BLUEPRINTTOOLS_H

#include <set>
#include <memory>

#include <QPainterPath>
#include <QGraphicsPathItem>
#include <QGraphicsRectItem>
#include <QWheelEvent>

#ifndef Q_MOC_RUN
#include "blueprint/glyph.h"
#include "blueprint/edit.h"
#endif

#include "glyphs.h"

class BlueprintView;
class BlueprintScene;

typedef std::set< Blueprint::IGlyph* > SelectionSet;

class Selection
{
public:
    enum Mode
    {
        eSet,
        eOR,
        eXAND,
        eXOR
    };

    static const Selectable* glyphToSelectable( const Blueprint::IGlyph* pGlyph );
    static Selectable* glyphToSelectable( Blueprint::IGlyph* pGlyph );
    static Mode getSelectionMode( QMouseEvent *event );

public:
    Selection( BlueprintView& view, Mode mode, const SelectionSet& interactiveSelection );
    void update( const SelectionSet& interactiveSelection );
    SelectionSet& getInitial() { return m_initialSelection; }
    const SelectionSet& getInitial() const { return m_initialSelection; }
private:
    BlueprintView& m_view;
    const Mode m_selectionMode;
    SelectionSet m_initialSelection;
};

class Tool
{
public:
    virtual void mouseHover( QMouseEvent* event )=0;
    virtual void mousePressEvent( QMouseEvent *event )=0;
    virtual void mouseMoveEvent( QMouseEvent *event )=0;
    virtual void mouseReleaseEvent( QMouseEvent *event )=0;
    virtual void keyPressEvent( QKeyEvent *event )=0;
    virtual void reset()=0;
    virtual unsigned int getToolType() const = 0;
};

class Selection_Interaction
{
public:
    virtual ~Selection_Interaction(){}
    virtual void update( const QPointF& pos )=0;
};

class Selection_Rect : public Selection_Interaction
{
public:
    Selection_Rect( BlueprintView& view, Selection::Mode mode, float fZoomLevel, const QPointF& downPos, const QColor& colour = QColor( 0, 0, 200, 50 ) );
    ~Selection_Rect();
    void update( const QPointF& pos );
private:
    BlueprintView& m_view;
    QRectF m_selectionRect;
    Selection m_selection;
    QPointF m_downPos;
    QPen m_selectionPen;
    QGraphicsRectItem* m_pSelectionRectItem;
};

class Selection_Path : public Selection_Interaction
{
public:
    Selection_Path( BlueprintView& view, Selection::Mode mode, float fZoomLevel, const QPointF& downPos, const QColor& colour = QColor( 0, 0, 200, 50 ) );
    ~Selection_Path();
    void update( const QPointF& pos );
    unsigned int getDepth() const { return m_uiDepth; }
private:
    BlueprintView& m_view;
    QPainterPath m_path;
    Selection m_selection;
    QPointF m_downPos;
    QPen m_selectionPen;
    QGraphicsPathItem* m_pSelectionPath;
    unsigned int m_uiDepth;
};

class SelectTool : public Tool
{
    enum ToolMode
    {
        eSelectNodes,
        eCmd,
        eDragCopyCmd
    };

public:
    SelectTool( BlueprintView& view );

    virtual void mouseHover( QMouseEvent* event );
    virtual void mousePressEvent( QMouseEvent *event );
    virtual void mouseMoveEvent( QMouseEvent *event );
    virtual void mouseReleaseEvent( QMouseEvent *event );
    virtual void keyPressEvent( QKeyEvent *event );
    virtual void reset();
    
    virtual unsigned int getToolType() const { return Blueprint::IEditContext::eSelect; }
private:
    BlueprintView& m_view;
    std::auto_ptr< Selection_Interaction > m_pSelection;
    Blueprint::IInteraction::Ptr m_pInteraction;
    ToolMode m_toolMode;
};

class PenTool : public Tool
{
protected:
    enum ToolMode
    {
        eDraw,
        eCmd,
        eDragCopy,
        eDelete
    };

public:
    PenTool( BlueprintView& view );

    virtual void mouseHover( QMouseEvent* event );
    virtual void mousePressEvent( QMouseEvent *event );
    virtual void mouseMoveEvent( QMouseEvent *event );
    virtual void mouseReleaseEvent( QMouseEvent *event );
    virtual void keyPressEvent( QKeyEvent *event );
    virtual void reset();
    
    virtual unsigned int getToolType() const { return Blueprint::IEditContext::eDraw; }
protected:
    BlueprintView& m_view;
    std::auto_ptr< Selection_Interaction > m_pSelection;
    Blueprint::IInteraction::Ptr m_pInteraction;
    ToolMode m_toolMode;
};

class ContextTool : public PenTool
{
public:
    ContextTool( BlueprintView& view );
    void setToolID( unsigned int uiToolID ) { m_toolID = uiToolID; }
    
    virtual unsigned int getToolType() const { return m_toolID; }
    virtual void mousePressEvent( QMouseEvent *event );
    /*
    virtual void mouseHover( QMouseEvent* event );
    virtual void mouseMoveEvent( QMouseEvent *event );
    virtual void mouseReleaseEvent( QMouseEvent *event );
    virtual void keyPressEvent( QKeyEvent *event );
    virtual void reset();
    */

private:
    unsigned int m_toolID;
};

#endif // BLUEPRINTTOOLS_H
