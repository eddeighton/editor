#ifndef GLYPHS_H
#define GLYPHS_H

#include <map>

#include <QDebug>
#include <QPixmap>
#include <QImage>
#include <QGraphicsPixmapItem>
#include <QGraphicsEffect>
#include <QGraphicsSimpleTextItem>
#include <QPainter>

#include "blueprintscene.h"

#ifndef Q_MOC_RUN
#include "common/assert_verify.hpp"
#include "common/tick.hpp"

#include "blueprint/site.h"
#include "blueprint/glyph.h"
#include "blueprint/editMain.h"
#include "blueprint/toolbox.h"
#endif

typedef std::map< QGraphicsItem*, Blueprint::IGlyph* > ItemMap;
typedef std::map< const Blueprint::GlyphSpec*, QGraphicsItem* > SpecMap;

struct GlyphMap
{
    ItemMap& itemMap;
    SpecMap& specMap;
    GlyphMap( ItemMap& _itemMap, SpecMap& _specMap )
        : itemMap( _itemMap ),
          specMap( _specMap )
    {

    }

    QGraphicsItem* findItem( const Blueprint::GlyphSpec* pSpec ) const
    {
        QGraphicsItem* pItem = 0u;
        if( pSpec )
        {
            SpecMap::const_iterator iFind = specMap.find( pSpec );
            if( iFind != specMap.end() )
                pItem = iFind->second;
            //only the root can have no graphics item
            if( pSpec->getParent() )
            {
                ASSERT_MSG( pItem, "Failed to locate QGraphicsItem*" );
            }
        }
        return pItem;
    }

    void insert( QGraphicsItem* pGraphicsItem, const Blueprint::GlyphSpec* pSpec, Blueprint::IGlyph* pGlyph)
    {
        itemMap.insert( std::make_pair( pGraphicsItem, pGlyph ) );
        specMap.insert( std::make_pair( pSpec, pGraphicsItem ) );
    }
    void erase( QGraphicsItem* pGraphicsItem, const Blueprint::GlyphSpec* pSpec )
    {
        itemMap.erase( pGraphicsItem );
        specMap.erase( pSpec );
    }
};

class Selectable
{
public:
    Selectable( unsigned int uiDepth )
        :   m_bSelected( false ),
            m_uiDepth( uiDepth )
    {
    }
    bool isSelected() const { return m_bSelected; }
    unsigned int getDepth() const { return m_uiDepth; }
    virtual void setSelected( bool bSelected );
    virtual bool isImage() const = 0;
private:
    bool m_bSelected;
    const unsigned int m_uiDepth;
};

class Renderable
{
public:
    Renderable( bool bShouldRender ) : m_bShouldRender( bShouldRender )
    {
    }
    virtual bool isRendering() const { return m_bShouldRender; }
    virtual void setShouldRender( bool bShouldRender )=0;
protected:
    bool m_bShouldRender;
};
class ZoomDependent
{
public:
    virtual void OnNewZoomLevel( float fZoom ) = 0;
};

static unsigned int calculateDepth( const Blueprint::GlyphSpec* pGlyphSpec )
{
    unsigned int uiDepth = 0u;
    while( const Blueprint::GlyphSpec* pParent = pGlyphSpec->getParent() )
    {
        pGlyphSpec = pParent;
        ++uiDepth;
    }
    return uiDepth;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
class GlyphControlPoint : public Blueprint::GlyphControlPoint, public Selectable, public ZoomDependent, public Renderable
{
public:
    GlyphControlPoint( Blueprint::IGlyph::Ptr pParent, QGraphicsScene* pScene,
                       GlyphMap map, Blueprint::ControlPoint* pControlPoint, float fZoom, bool bShouldRender,
                       Blueprint::Toolbox::Ptr pToolboxPtr );
    ~GlyphControlPoint();

    //Selectable
    virtual void setSelected( bool bSelected );
    virtual bool isImage() const { return false; }

    //ZoomDependent
    virtual void OnNewZoomLevel( float fZoom );

    //Renderable
    virtual void setShouldRender( bool bShouldRender );

    //Blueprint::GlyphControlPoint
    virtual void update();

    void updateColours();
    QGraphicsEllipseItem* getItem() const;

private:
    float m_fSizeScaling;
    float m_fSize;
    QGraphicsScene* m_pScene;
    GlyphMap m_map;
    QGraphicsEllipseItem* m_pItem;
    Blueprint::Toolbox::Ptr m_pToolBoxPtr;
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/*
class GlyphPath : public Blueprint::GlyphPath, public ZoomDependent, public Renderable
{
public:
    GlyphPath( Blueprint::IGlyph::Ptr pParent, QGraphicsScene* pScene,
               GlyphMap map, Blueprint::MarkupPath* pPath, float fZoom, bool bShouldRender,
               Blueprint::Toolbox::Ptr pToolBoxPtr );
    ~GlyphPath();

    //ZoomDependent
    virtual void OnNewZoomLevel( float fZoom );

    //Renderable
    virtual void setShouldRender( bool bShouldRender );

    //Blueprint::GlyphPath
    virtual void update();

private:
    float m_fSize;
    QPainterPath m_path;
    QGraphicsScene* m_pScene;
    GlyphMap m_map;
    QGraphicsPathItem* m_pItem;
    Blueprint::Toolbox::Ptr m_pToolBoxPtr;
};
*/
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
class GlyphPolygonGroup : public Blueprint::GlyphPolygonGroup, public ZoomDependent, public Renderable
{
public:
    GlyphPolygonGroup( Blueprint::IGlyph::Ptr pParent, QGraphicsScene* pScene,
               GlyphMap map, Blueprint::MarkupPolygonGroup* pPolygonGroup, float fZoom, bool bShouldRender,
               Blueprint::Toolbox::Ptr pToolBoxPtr );
    ~GlyphPolygonGroup();

    //ZoomDependent
    virtual void OnNewZoomLevel( float fZoom );

    //Renderable
    virtual void setShouldRender( bool bShouldRender );

    //Blueprint::GlyphPath
    virtual void update();

private:
    float m_fSize;
    QPainterPath m_path;
    QGraphicsScene* m_pScene;
    GlyphMap m_map;
    QGraphicsPathItem* m_pItem;
    Blueprint::Toolbox::Ptr m_pToolBoxPtr;
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
class GlyphOrigin : public Blueprint::GlyphOrigin, public Selectable, public Renderable
{
public:
    GlyphOrigin( Blueprint::IGlyph::Ptr pParent, QGraphicsScene* pScene,
                GlyphMap map, Blueprint::Origin* pOrigin, 
                Blueprint::IEditContext*& pActiveContext, bool bShouldRender,
                Blueprint::Toolbox::Ptr pToolBoxPtr );
    ~GlyphOrigin();

    //Selectable
    virtual void setSelected( bool bSelected );
    virtual bool isImage() const { return true; }

    //Renderable
    virtual void setShouldRender( bool bShouldRender );

    bool isActiveContext() const;

    //Blueprint::GlyphOrigin
    virtual void update();

    void updateColours();
private:
    void setOrCreateImageItem();
private:
    QGraphicsScene* m_pScene;
    GlyphMap m_map;
    
    QPainterPath m_path;
    QGraphicsPathItem* m_pPathItem;
    QGraphicsLineItem* m_pItemX;
    QGraphicsLineItem* m_pItemY;
    
    Blueprint::IEditContext*& m_pActiveContext;
    Timing::UpdateTick m_lastUpdateTick;
    bool m_bActiveContext;
    Blueprint::Toolbox::Ptr m_pToolBoxPtr;
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/*
class GlyphImage : public Blueprint::GlyphImage, public Selectable, public Renderable
{
public:
    GlyphImage( Blueprint::IGlyph::Ptr pParent, QGraphicsScene* pScene,
                GlyphMap map, Blueprint::ImageSpec* pImage, 
                Blueprint::IEditContext*& pActiveContext, bool bShouldRender,
                Blueprint::Toolbox::Ptr pToolBoxPtr );
    ~GlyphImage();

    //Selectable
    virtual void setSelected( bool bSelected );
    virtual bool isImage() const { return true; }

    //Renderable
    virtual void setShouldRender( bool bShouldRender );

    bool isActiveContext() const;

    //Blueprint::GlyphImage
    virtual void update();

private:
    void setOrCreateImageItem();
private:
    static QVector< QRgb > m_coloursNormal, m_coloursSelected, m_coloursActiveContext;
    QPixmap m_pixelMap;
    QGraphicsScene* m_pScene;
    GlyphMap m_map;
    QGraphicsPixmapItem* m_pItem;
    Blueprint::IEditContext*& m_pActiveContext;
    Timing::UpdateTick m_lastUpdateTick;
    bool m_bActiveContext;
    Blueprint::Toolbox::Ptr m_pToolBoxPtr;
};
*/

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
class GlyphText : public Blueprint::GlyphText, public Renderable
{
public:
    GlyphText( Blueprint::IGlyph::Ptr pParent, QGraphicsScene* pScene,
               GlyphMap map, Blueprint::MarkupText* pText, bool bShouldRender,
               Blueprint::Toolbox::Ptr pToolBoxPtr );
    ~GlyphText();

    //Renderable
    virtual void setShouldRender( bool bShouldRender );

    //Blueprint::GlyphText
    virtual void update();

private:
    QGraphicsScene* m_pScene;
    GlyphMap m_map;
    QGraphicsSimpleTextItem* m_pItem;
    Blueprint::Toolbox::Ptr m_pToolBoxPtr;
};


#endif // GLYPHS_H
