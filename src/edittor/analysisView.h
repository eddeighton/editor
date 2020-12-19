
#ifndef VISIBILITY_VIEW_19_DEC_2020
#define VISIBILITY_VIEW_19_DEC_2020

#include <map>
#include <memory>

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
#include "blueprint/visibility.h"
#endif

class AnalysisView
{
public:
    using Ptr = std::shared_ptr< AnalysisView >;

    AnalysisView(    Blueprint::Analysis::Ptr pAnalysis, 
                        QGraphicsScene* pScene,
                        Blueprint::Toolbox::Ptr pToolBoxPtr );
    ~AnalysisView();

    void setVisible( bool bVisible );
private:
    Blueprint::Analysis::Ptr m_pAnalysis;
    QGraphicsScene* m_pScene;
    Blueprint::Toolbox::Ptr m_pToolBoxPtr;
    
    
    QPainterPath m_path;
    QGraphicsPathItem* m_pItem;
};


#endif //VISIBILITY_VIEW_19_DEC_2020