#include "myitems.h"
#include <QBrush>
#include <QtGlobal>

MyPointItem::MyPointItem(qreal r)
    : QGraphicsEllipseItem{0, 0, 2*r, 2*r}, m_R {r}
{}

MyPointItem::MyPointItem(QPointF pos, qreal r)
    : QGraphicsEllipseItem{pos.x(), pos.y(), 2*r, 2*r},
      m_R {r}
{}

MyPointItem::MyPointItem(qreal x, qreal y, qreal r)
    : QGraphicsEllipseItem{x, y, 2*r, 2*r}, m_R {r}
{}

QPointF MyPointItem::toRectCenter(QPointF boundingRectPos)
{
    //接受控制点boundingBox左上角的坐标
    //返回控制点中心坐标
    return boundingRectPos + QPointF(m_R, m_R);
}

QPointF MyPointItem::toRectPos(QPointF center)
{
    //接受控制点中心坐标
    //返回boundingBox左上角的坐标
    return center - QPointF(m_R, m_R);
}

QPointF MyPointItem::center()
{
    return sceneBoundingRect().center();
}

void MyPointItem::setRadius(qreal r)
{
    setRect(rect().x(), rect().y(), 2 * r, 2 * r);
}

int MyPointItem::type() const
{
    // Enable the use of qgraphicsitem_cast with this item.
    return Type;
}



SelectRectItem::SelectRectItem(QPointF pos)
    : m_beginPos{pos}, m_endPos{pos}
{
    m_selectArea.setTopLeft(pos);
    m_selectArea.setBottomRight(pos);
//    QGraphicsRectItem(m_selectArea);
    setRect(m_selectArea);
    setPen(QPen(Qt::blue));
}

void SelectRectItem::begin(QPointF pos)
{
    m_selectArea.setTopLeft(pos);
    m_selectArea.setBottomRight(pos);
}

void SelectRectItem::move(QPointF pos)
{
    m_endPos = pos;
    m_updateArea();
}

bool SelectRectItem::end(QPointF pos)
{
    m_selectPath.addRect(m_selectArea);
    //判断是否选择框足够大
    return (m_selectArea.width() > 10
            && m_selectArea.height() > 10);
}

QPainterPath &SelectRectItem::path()
{
    return m_selectPath;
}

void SelectRectItem::m_updateArea()
{
    qreal x = qMin(m_beginPos.x(), m_endPos.x());
    qreal y = qMin(m_beginPos.y(), m_endPos.y());
    QPointF bias = m_endPos - m_beginPos;
    m_selectArea.setRect(x, y, qAbs(bias.x()), qAbs(bias.y()));
    setRect(m_selectArea);
}

ControlPolygonItem::ControlPolygonItem(QPointF beginPos)
{
    m_path.moveTo(beginPos);
    setPen(QPen(Qt::darkGray));
}

ControlPolygonItem::ControlPolygonItem()
{
    m_path.moveTo(QPointF(0,0));
    setPen(QPen(Qt::darkGray));
}

void ControlPolygonItem::lineTo(QPointF pos)
{
    m_path.lineTo(pos);
    setPath(m_path);
}

void ControlPolygonItem::draw(QList<MyPointItem *> &ctrPoints)
{
    m_path.clear();
    if(ctrPoints.size() < 2)
    {
        setPath(m_path);
        return;
    }  
//    qDebug()<<"DRAW";
    m_path.moveTo(ctrPoints[0]->center());
    for(auto i = 1; i < ctrPoints.size(); i++)
    {
        m_path.lineTo(ctrPoints[i]->center());
    }
    setPath(m_path);
}

BsplineCurveItem::BsplineCurveItem(QGraphicsItem *parent)
    : QGraphicsPathItem(parent)
{
    QPen pen{QBrush(Qt::darkBlue), 2};
    setPen(pen);
}

bool BsplineCurveItem::draw(QList<QPointF>& ctrPoints)
{
    //绘制样条曲线
    if(!bspline.isDrawEnable())
    {
        return false;
    }
    else
    {
        QList<QPointF> points = bspline.evaluate(ctrPoints, 0.01);
        m_path.clear();
        m_path.moveTo(points[0]);
        for(int i=1; i<points.size(); i++)
        {
            m_path.lineTo(points[i]);
        }
        setPath(m_path);
        return true;
    }
}


void BsplineCurveItem::setDegree(int newDegree)
{
    bspline.setDegree(newDegree);
}

void BsplineCurveItem::setCtrPointsNum(int newCtrPointsNum)
{
    bspline.setCtrPointsNum(newCtrPointsNum);
}
