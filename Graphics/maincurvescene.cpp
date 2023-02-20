#include "maincurvescene.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <QPainter>
#include <typeinfo>
#include <QGraphicsView>

MainCurveScene::MainCurveScene(QWidget *parent)
    : QGraphicsScene{parent}
{
    ctrPolygon = new ControlPolygonItem();
    addItem(ctrPolygon);

    bsplineCurve = new BsplineCurveItem();

    addItem(bsplineCurve);

    timer = new QTimer();
    timer->setInterval(30);
    connect(timer, &QTimer::timeout, this,
            &MainCurveScene::drawControlPolgon);
    connect(timer, &QTimer::timeout, this,
            &MainCurveScene::drawBsplineCurve);
    timer->start();
}

MyPointItem *MainCurveScene::addPoint(QPointF pos)
{
    auto newPoint = new MyPointItem{pos, m_R};

    newPoint->setBrush(Qt::black);
    newPoint->setFlags(QGraphicsItem::ItemIsFocusable |
                       QGraphicsItem::ItemIsSelectable |
                       QGraphicsItem::ItemIsMovable);
    addItem(newPoint);
    ctrPoints.append(newPoint);
    bsplineCurve->setCtrPointsNum(ctrPoints.size());

    return newPoint;
}

void MainCurveScene::removeAllPoint()
{
    for(auto item : items())
    {
        if(item->type() == 65537)
        {
            removeItem(item);
            ctrPoints.removeOne(item);
            delete item;
        }
    }
}

QPointF MainCurveScene::pointRectCenter(QPointF boundingRectPos)
{
    //接受控制点boundingBox左上角的坐标
    //返回控制点中心坐标
    return boundingRectPos + QPointF(m_R, m_R);
}

QPointF MainCurveScene::pointRectPos(QPointF center)
{
    //接受控制点中心坐标
    //返回boundingBox左上角的坐标
    return center - QPointF(m_R, m_R);
}

void MainCurveScene::drawControlPolgon()
{
    ctrPolygon->draw(ctrPoints);
}

void MainCurveScene::drawBsplineCurve()
{
    bsplineCurve->clear();
    if(bsplineType==NotDefine)
    {
        emit textInfo(QString("未定义曲线类型"));

        return;
    }

    if(!bsplineCurve->bspline.isDrawEnable(
                bsplineDegree, ctrPoints.size(), bsplineType))
    {
        emit textInfo(QString("错误，次数与控制点数不匹配！"));
        return;
    }

    QList<QPointF> points_list = generatePoints();
    vector<double> polylength;
    switch (bsplineType)
    {
    case Uniform:
        bsplineCurve->bspline.knots.setUnifrom();
        break;
    case Quasi_uniform:
        bsplineCurve->bspline.knots.setQuasiUniform();
        break;
    case Riesenfeld:
        polylength = getPolyLength(points_list);
        bsplineCurve->bspline.knots.setRiesenfeld(polylength);
        break;
    case Hartley_Judd:
        polylength = getPolyLength(points_list);
        bsplineCurve->bspline.knots.setHartley_Judd(polylength);
        break;
    case Bezier:
        bsplineCurve->bspline.knots.setBezier();
        break;
    default:
        emit textInfo(QString("未定义曲线类型"));
        return;
    }
    emit textInfo(QString(" "));
    //将knots传递给basisfun窗口
    emit defineKnotsVec(bsplineCurve->bspline.knots);
    bsplineCurve->draw(points_list);
}


QList<QPointF> MainCurveScene::generatePoints()
{
    QList<QPointF> points_list;
    for (auto p : ctrPoints)
    {
        points_list.append(QPointF(p->center()));
    }
    return points_list;
}

void MainCurveScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
/*
    鼠标移动，需要考虑：
    实时将鼠标位置传给主窗口显示
    绘制变化中的选择框
*/
{
    QPointF mouseScenePos = event->scenePos();
    if(m_selectAreaEnable)
    {
        //选择框
        m_selectRectItem->move(mouseScenePos);
    }
    emit mouseMove(mouseScenePos);  //传输到主窗口显示鼠标位置
    QGraphicsScene::mouseMoveEvent(event);
}

void MainCurveScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
/*
    鼠标点击，需要考虑：
    添加点、选择点、框选的起始位置
*/
{
    QPointF pressScenePos = event->scenePos();

    QGraphicsItem *item = NULL; //点击处的Item
    item = itemAt(pressScenePos, views()[0]->transform());

    if(event->modifiers() != Qt::CTRL)
    {
        clearSelection();
    }

    if(item!=NULL && item->type() == 65537)
    {
        //点击选择点
        item->setSelected(true);
        QRectF rect = item->sceneBoundingRect();
//        qDebug()<< rect << views()[0]->mapFromScene(rect.x(), rect.y());
        emit selectItemScenePos(QPointF(rect.x()+m_R, rect.y()+m_R));
    }
    else if(addPointEnable)
    {
        //添加点
        auto newPoint = addPoint(pressScenePos - QPointF(m_R, m_R));
        newPoint->setSelected(true);
    }
    else
    {
        //开始框选
        m_selectAreaEnable = true;
        m_selectRectItem = new SelectRectItem(pressScenePos);
        addItem(m_selectRectItem);
    }
//    qDebug()<<items();
    QGraphicsScene::mousePressEvent(event);
}

void MainCurveScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
/*
    鼠标松开，需要考虑：
    框选结束，将选择框中的item设为选中，删除选择框
*/
{
    if (!addPointEnable && m_selectAreaEnable)
    {
        if(m_selectRectItem->end(event->scenePos()))
        {
            setSelectionArea(m_selectRectItem->path());
        }
        //删除选择框
        m_selectAreaEnable = false;
        removeItem(m_selectRectItem);
        delete m_selectRectItem;
    }
    QGraphicsScene::mouseReleaseEvent(event);
}

void MainCurveScene::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Backspace &&
            !selectedItems().isEmpty())
    {
        for(auto item : selectedItems())
        {
            removeItem(item);
            ctrPoints.removeOne(item);
            delete item;
        }
        bsplineCurve->setCtrPointsNum(ctrPoints.size());
    }
}


