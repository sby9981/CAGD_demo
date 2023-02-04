#ifndef MAINGRAPHICSSCENE_H
#define MAINGRAPHICSSCENE_H

#include <QObject>
#include <QWidget>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include "myitems.h"
#include <QList>

class MainGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:

    bool addPointEnable {true};
    QGraphicsPathItem* controlPolygon {NULL};

    explicit MainGraphicsScene(QWidget *parent = nullptr);
    MyPointItem* addPoint(QPointF pos);
    QPointF pointRectCenter(QPointF boundingRectPos);
    QPointF pointRectPos(QPointF center);
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
signals:
    void mouseMove(QPointF event);
    void selectItemScenePos(QPointF pos);

private:
    //用于框选
    SelectRectItem* m_selectRectItem {NULL};
    bool m_selectAreaEnable {false};

//    ControlPolylineItem ctrPolyline;

    qreal m_R{5};    //点半径

};

#endif // MAINGRAPHICSSCENE_H
