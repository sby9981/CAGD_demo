#ifndef MAINGRAPHICSSCENE_H
#define MAINGRAPHICSSCENE_H

#include <QObject>
#include <QWidget>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include "myitems.h"
#include <QList>
#include <QTimer>
class MainGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:

    bool addPointEnable {true};
    QList<MyPointItem*> ctrPoints;
    QGraphicsPathItem* controlPolygon {NULL};

    explicit MainGraphicsScene(QWidget *parent = nullptr);
    MyPointItem* addPoint(QPointF pos);

    //控制点中心坐标与boundboxPos的转换
    QPointF pointRectCenter(QPointF boundingRectPos);
    QPointF pointRectPos(QPointF center);

    void drawControlPolgon();
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

    QTimer* timer;

    ControlPolygonItem* ctrPolygon {NULL};

    qreal m_R{5};    //点半径

};

#endif // MAINGRAPHICSSCENE_H
