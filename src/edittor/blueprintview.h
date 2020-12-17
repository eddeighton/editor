#ifndef BLUEPRINTVIEW_H
#define BLUEPRINTVIEW_H

#include <map>
#include <set>
#include <memory>

#ifndef Q_MOC_RUN
#include <boost/shared_ptr.hpp>
#endif

#include <QDebug>
#include <QGraphicsView>
#include <QWheelEvent>
#include <QGraphicsScene>
#include <QItemSelection>
#include <QPainterPath>

#ifndef Q_MOC_RUN
#include "blueprint/site.h"
#include "blueprint/editMain.h"
#include "blueprint/toolbox.h"
#endif

#include "blueprintscene.h"
#include "glyphs.h"
#include "messages.h"
#include "blueprintTools.h"

class BlueprintSelectionModel;
class BlueprintItemModel;

namespace Ui {
class MainWindow;
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
class BlueprintView : public QGraphicsView, public Blueprint::GlyphFactory
{
    Q_OBJECT

    friend class SelectTool;
    friend class PenTool;
    friend class ContextTool;
    friend class PropertyTool;

    enum InteractionMode
    {
        eNone,
        eScroll,
        eTool
    };

    struct ScrollData
    {
        QPointF m_downPos, m_downScrollPos;
        ScrollData( const QPointF& downPos, const QPointF& downScrollPos )
            : m_downPos( downPos ), m_downScrollPos( downScrollPos )
        {}
    };
    std::auto_ptr< ScrollData > m_pScrollData;

public:
    explicit BlueprintView(QWidget *parent = 0);
    ~BlueprintView();

    void setUI( Ui::MainWindow* pUI ) { m_pQTUI = pUI; }
    void setModel( BlueprintSelectionModel* pSelection, BlueprintItemModel* pModel ) { m_pSelectionModel = pSelection; m_pModel = pModel; }
    void setToolbox( Blueprint::Toolbox::Ptr pToolbox ) { m_pToolBox = pToolbox; }

    QGraphicsItem* findActiveContextItem() const;
    QPointF mapToActiveContext( const QPointF& scenePos ) const;
    Blueprint::IGlyph* findGlyph( QGraphicsItem* pItem ) const;
    Blueprint::IGlyph* findSelectableTopmostGlyph( const QPointF& pos ) const;
    Selectable* selectableFromNode( Blueprint::Node::PtrCst pNode ) const;

    //glyph factory interface
    virtual Blueprint::IGlyph::Ptr createControlPoint( Blueprint::ControlPoint* pControlPoint, Blueprint::IGlyph::Ptr pParent );
    virtual Blueprint::IGlyph::Ptr createOrigin( Blueprint::Origin* pOrigin, Blueprint::IGlyph::Ptr pParent );
    virtual Blueprint::IGlyph::Ptr createMarkupPath( Blueprint::MarkupPath* pMarkupPath, Blueprint::IGlyph::Ptr pParent );
    virtual Blueprint::IGlyph::Ptr createMarkupPolygonGroup( Blueprint::MarkupPolygonGroup* pMarkupPolygonGroup, Blueprint::IGlyph::Ptr pParent );
    virtual Blueprint::IGlyph::Ptr createMarkupText( Blueprint::MarkupText* pMarkupText, Blueprint::IGlyph::Ptr pParent );

    void SelectContext( Blueprint::IEditContext* pNewContext );

    SelectionSet getSelection() const;
    SelectionSet getSelectedByRect( const QRectF& rect ) const;
    SelectionSet getSelectedByPath( const QPainterPath& path ) const;
    void setSelected( const SelectionSet& selection );
    Blueprint::Site::Ptr getCurrentPaletteItem() const;

    void DoZoom( float fAmt );
    void SetZoom( QVector2D v2NewZoomLevel );
    void CalculateOversizedSceneRect();
    void CalculateRulerItems();
    QVector2D BlueprintView::calculateQuantisation() const;
    Blueprint::IEditContext::ToolMode getMode() const { return m_toolMode; }

signals:
    void OnBlueprintModified();
    void OnBlueprintSelected( BlueprintMsg msg );
    void OnEditContextChanged( BlueprintContext context );
    void OnClipboardAction();
    void OnWindowTitleModified( QString strTitle );

public slots:
    void OnNewBlueprint();
    void OnLoadBlueprint();
    void OnSaveBlueprint();
    void OnSaveAsBlueprint();

    //cmds
    void OnCmd_Delete();
    void OnCmd_Cut();
    void OnCmd_Copy();
    void OnCmd_Paste();
    void OnCmd_SelectAll();
    void OnCmd_ZoomToAll();
    void OnCmd_TabOut();
    void OnCmd_Extrude();
    
    //menu commands
    void OnCmd_AddProperty();
    void OnCmd_DelProperty();
    void OnCmd_EditProperty();

    //selection
    void OnCurrentSelectionItemChanged( const QModelIndex& current, const QModelIndex& previous );
    void OnSelectionChanged( const QItemSelection& selected, const QItemSelection& deselected );

    //view modes
    void setViewMode( bool bArrangement, bool bCellComplex, bool bClearance );

    //tools
    void OnSelectTool_Selector();
    void OnSelectTool_Pen();
    void OnSelectTool_Property();
    
    //modes
    void OnSelectMode_Area();
    void OnSelectMode_Contour();
    //void OnSelectMode_Connection();
    
    //rotate and flip
    void OnRotateLeft();
    void OnRotateRight();
    void OnFlipHorizontally();
    void OnFlipVeritcally();

    void OnSetQuantise( int iQuantisation );
    
    void showEvent( QShowEvent* pEvent );
protected:
    void mouseHover( QMouseEvent* event );//fake

    void mouseDoubleClickEvent(QMouseEvent * event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void scrollContentsBy(int dx, int dy );
    void wheelEvent(QWheelEvent *event);
    void drawBackground(QPainter *painter, const QRectF &rect);
private:
    Ui::MainWindow* m_pQTUI;

    BlueprintScene* m_pBlueprintScene;
    BlueprintSelectionModel* m_pSelectionModel;
    BlueprintItemModel* m_pModel;
    ItemMap m_itemMap;
    SpecMap m_specMap;

    Blueprint::EditMain::Ptr m_pBlueprintEdit;
    Blueprint::Site::Ptr m_pBlueprint;
    Blueprint::IEditContext* m_pActiveContext;

    InteractionMode m_interactionMode;

    SelectTool m_selectTool;
    PenTool m_penTool;
    PropertyTool m_propertyTool;
    Tool* m_pActiveTool;
    Blueprint::Toolbox::Ptr m_pToolBox;
    Blueprint::IEditContext::ToolMode m_toolMode = Blueprint::IEditContext::eArea;

    typedef std::vector< QGraphicsSimpleTextItem* > TextItemVector;
    TextItemVector m_rulerVertItems, m_rulerHoriItems;

    QVector2D m_v2ZoomLevel;
    int m_iQuantisation;
    static const float m_fZoomRate;
    float m_fDefaultZoom;
    bool m_bInitialising;
};

#endif // BLUEPRINTVIEW_H
