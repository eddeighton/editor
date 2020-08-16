#ifndef BLUEPRINTSCENE_H
#define BLUEPRINTSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneWheelEvent>

class BlueprintScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit BlueprintScene(QObject *parent = 0);

protected:
signals:

public slots:

private:
    float m_fZoomLevel;

};

#endif // BLUEPRINTSCENE_H
