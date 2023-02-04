#ifndef MYPOINT_H
#define MYPOINT_H

#include <QObject>
#include <QGraphicsEllipseItem>
#include <QGraphicsItem>
#include <QPainter>

class MyPointItem;
class ControlPolygonItem;
class SelectRectItem;

class MyPointItem : public QGraphicsEllipseItem
{
    enum { Type = UserType + 1 }; // Type = 65537
public:
    explicit MyPointItem(qreal d);
    explicit MyPointItem(QPointF pos, qreal d);
    explicit MyPointItem(qreal x, qreal y, qreal d);
    QPointF toRectCenter(QPointF boundingRectPos);
    QPointF toRectPos(QPointF center);
    void setRadius(qreal r);

    int type() const override;
private:
    qreal m_R;
signals:

};

class SelectRectItem : public QGraphicsRectItem
{
public:
    explicit SelectRectItem(QPointF pos);
    void begin(QPointF pos);
    void move(QPointF pos);
    bool end(QPointF pos);
    QPainterPath& path();
private:
    QRectF m_selectArea;
    QPainterPath m_selectPath;

    QPointF m_beginPos;
    QPointF m_endPos;

    void m_updateArea();
};

#endif // MYPOINT_H