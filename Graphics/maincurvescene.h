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
class MainCurveScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit MainCurveScene(QWidget *parent = nullptr);

    MyPointItem* addPoint(QPointF pos);
    void removeAllPoint();

    //控制点中心坐标与boundboxPos的转换
    QPointF pointRectCenter(QPointF boundingRectPos);
    QPointF pointRectPos(QPointF center);
    QList<QPointF> generatePoints();

    void drawControlPolgon();
    void drawBsplineCurve();

public:
    bool addPointEnable {true};
    int bsplineDegree {0};
    KnotsType bsplineType {NotDefine};
    QList<MyPointItem*> ctrPoints;
    ControlPolygonItem* ctrPolygon;
    BsplineCurveItem* bsplineCurve;
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

signals:
    void mouseMove(QPointF event);
    void selectItemScenePos(QPointF pos);
    void defineKnotsVec(KnotsVector& knots);
    void textInfo(QString info);
private:
    //用于框选
    SelectRectItem* m_selectRectItem {NULL};
    bool m_selectAreaEnable {false};

    QTimer* timer;

    qreal m_R{5};    //点半径

};

#endif // MAINGRAPHICSSCENE_H
