#include "maingraphicsscene.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <QPainter>
#include <typeinfo>
#include <QGraphicsView>

MainGraphicsScene::MainGraphicsScene(QWidget *parent)
    : QGraphicsScene{parent}
{
    ctrPolygon = new ControlPolygonItem();
    addItem(ctrPolygon);

    timer = new QTimer();
    timer->setInterval(30);
    connect(timer, &QTimer::timeout, this,
            &MainGraphicsScene::drawControlPolgon);
    timer->start();
}

MyPointItem *MainGraphicsScene::addPoint(QPointF pos)
{
    auto newPoint = new MyPointItem{pos, m_R};

    newPoint->setBrush(Qt::black);
    newPoint->setFlags(QGraphicsItem::ItemIsFocusable |
                       QGraphicsItem::ItemIsSelectable |
                       QGraphicsItem::ItemIsMovable);
    addItem(newPoint);
    ctrPoints.append(newPoint);
    return newPoint;
}

QPointF MainGraphicsScene::pointRectCenter(QPointF boundingRectPos)
{
    //接受控制点boundingBox左上角的坐标
    //返回控制点中心坐标
    return boundingRectPos + QPointF(m_R, m_R);
}

QPointF MainGraphicsScene::pointRectPos(QPointF center)
{
    //接受控制点中心坐标
    //返回boundingBox左上角的坐标
    return center - QPointF(m_R, m_R);
}

void MainGraphicsScene::drawControlPolgon()
{
    ctrPolygon->draw(ctrPoints);
}

void MainGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
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

void MainGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
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

void MainGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
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

void MainGraphicsScene::keyPressEvent(QKeyEvent *event)
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
    }
}


