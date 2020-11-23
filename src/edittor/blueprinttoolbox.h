#ifndef BLUEPRINTTOOLBOX_H
#define BLUEPRINTTOOLBOX_H

#include <QToolBox>
#include <QAbstractListModel>
#include <QListView>
#include <qgraphicsscene.h>

#include "blueprintview.h"

#ifndef Q_MOC_RUN
#include <string>
#include <map>
#include <boost/shared_ptr.hpp>
#include "common/tick.hpp"
#include "blueprint/node.h"
#include "blueprint/toolbox.h"
#endif


////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
class ClipScene : public QGraphicsScene, public Blueprint::GlyphFactory
{
    Q_OBJECT

public:
    explicit ClipScene(QWidget *parent = 0);

    void setSite( Blueprint::Site::Ptr pSite, Blueprint::Toolbox::Ptr pToolBox );

    //glyph factory interface
    virtual Blueprint::IGlyph::Ptr createControlPoint( Blueprint::ControlPoint* pControlPoint, Blueprint::IGlyph::Ptr pParent );
    virtual Blueprint::IGlyph::Ptr createOrigin( Blueprint::Origin* pOrigin, Blueprint::IGlyph::Ptr pParent );
    virtual Blueprint::IGlyph::Ptr createMarkupPath( Blueprint::MarkupPath* pMarkupPath, Blueprint::IGlyph::Ptr pParent );
    virtual Blueprint::IGlyph::Ptr createMarkupPolygonGroup( Blueprint::MarkupPolygonGroup* pMarkupPolygonGroup, Blueprint::IGlyph::Ptr pParent );
    virtual Blueprint::IGlyph::Ptr createMarkupText( Blueprint::MarkupText* pMarkupText, Blueprint::IGlyph::Ptr pParent );

signals:

public slots:

protected:
    virtual void drawBackground(QPainter *painter, const QRectF &rect);

private:
    ItemMap m_itemMap;
    SpecMap m_specMap;
    Blueprint::Edit::Ptr m_pBlueprintEdit;
    Blueprint::Site::Ptr m_pBlueprint;
    Blueprint::IEditContext* m_pNullContext;
    Blueprint::Toolbox::Ptr m_pToolBox;
};

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
class BlueprintToolbox;
class FlowView : public QGraphicsView
{
    Q_OBJECT

    class FlowItem
    {
    public:
        typedef boost::shared_ptr< FlowItem > Ptr;
        typedef std::vector< Ptr > PtrVector;
        FlowItem( FlowView& view, Blueprint::Site::Ptr pSite, Blueprint::Toolbox::Palette::Ptr pPalette );
        ~FlowItem()
        {
            if( m_view.scene() )
            {
                m_view.scene()->removeItem( m_pImageItem );
                delete m_pImageItem;
            }
        }
        Blueprint::Site::Ptr getSite() const { return m_pSite; }
        QRectF getRect() const { return m_pImageItem->sceneBoundingRect(); }
        bool onHit( QGraphicsItem* pItem );
        void updatePosition( int x, int y, float fWidth, float fHeight, float fSpacing );
    private:
        FlowView& m_view;
        Blueprint::Site::Ptr m_pSite;
        QGraphicsItem* m_pImageItem;
        Blueprint::Toolbox::Palette::Ptr m_pPalette;
    };
    typedef std::map< Blueprint::Site::Ptr, FlowItem::Ptr > ItemMap;

public:
    explicit FlowView( BlueprintToolbox& toolbox, Blueprint::Toolbox::Palette::Ptr pPalette );
    ~FlowView();

    Blueprint::Toolbox::Palette::Ptr getPalette() const { return m_pPalette; }

    void updateClips();
    void calculateLayout();
    void clear();
    
signals:
    void OnClipboardAction();
    void OnMenu( ClipboardMsg msg );

protected:
    void mouseDoubleClickEvent(QMouseEvent * event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    virtual void resizeEvent(QResizeEvent * event);
private:
    int findIndex( Blueprint::Site::Ptr pSite );

private:
    BlueprintToolbox& m_toolBox;
    Blueprint::Toolbox::Palette::Ptr m_pPalette;
    ItemMap m_items;
    QGraphicsRectItem* m_pSelectionRectItem;
};

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
class BlueprintToolbox : public QToolBox
{
    Q_OBJECT
private:
    friend struct CompareUpdateTimes;
    class ToolboxPanel
    {
    public:
        typedef boost::shared_ptr< ToolboxPanel > Ptr;

        ToolboxPanel( BlueprintToolbox& toolbox, 
            Blueprint::Toolbox::Palette::Ptr pPalette, unsigned int uiIndex );
        ~ToolboxPanel();
        void updateClips();
        const Timing::UpdateTick& getUpdateTick() const { return m_updateTick; }
    private:
        std::string m_strName;
        BlueprintToolbox& m_toolbox;
        Blueprint::Toolbox::Palette::Ptr m_pPalette;
        FlowView* m_pView;
        Timing::UpdateTick m_updateTick;
    };
    typedef std::map< std::string, ToolboxPanel::Ptr > PanelMap;
    
public:
    explicit BlueprintToolbox(QWidget *parent = 0);

    void setToolbox( Blueprint::Toolbox::Ptr pToolbox );
    
    Blueprint::Toolbox::Ptr getToolbox() const { return m_pToolBox; }
    
public slots:
    void updateToolbox();
    void onCurrentPaletteChanged( int index );

    void OnPaletteMenu( ClipboardMsg msg );
    void On_DeleteClip( ClipboardMsg msg );
    void On_RescanToolBoxDir();

private:
    Blueprint::Toolbox::Ptr m_pToolBox;
    PanelMap m_panels;
};

#endif // BLUEPRINTTOOLBOX_H
