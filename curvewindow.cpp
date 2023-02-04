#include "curvewindow.h"
#include "./ui_curvewindow.h"
#include <QMouseEvent>
#include <QDebug>
#include <QPainter>
#include <QString>

CurveWindow::CurveWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CurveWindow)
{
    ui->setupUi(this);

    labViewCoordinate = new QLabel("光标View坐标");
    labViewCoordinate->setMinimumWidth(150);
    ui->statusbar->addWidget(labViewCoordinate);

    labSceneCoordinate = new QLabel("光标Scene坐标");
    labSceneCoordinate->setMinimumWidth(150);
    ui->statusbar->addWidget(labSceneCoordinate);

    labItemCoordinate = new QLabel("控制点坐标");
    labItemCoordinate->setMinimumWidth(150);
    ui->statusbar->addWidget(labItemCoordinate);

    ui->mainView->setCursor(Qt::CrossCursor);
    ui->mainView->setMouseTracking(true);

    //将scene左上角与view窗口左上角重合，且大小一致
    QRectF viewR = ui->mainView->geometry();
    scene = new MainGraphicsScene();
    scene->setSceneRect(-viewR.width()/2, -viewR.height()/2,
                        viewR.width()-2, viewR.height()-2);
    ui->mainView->setScene(scene);

    connect(scene, &MainGraphicsScene::mouseMove,
            this, &CurveWindow::on_mouseMove);
    connect(scene, &MainGraphicsScene::selectItemScenePos,
            this, &CurveWindow::on_selectItem);
}

CurveWindow::~CurveWindow()
{
    delete ui;
}

void CurveWindow::on_mouseMove(QPointF point)
{
    QPointF pointView = ui->mainView->mapFromScene(point);
    labViewCoordinate->setText(
                QString::asprintf("View坐标:%.0f, %.0f",
                                  pointView.x(), pointView.y()));

    labSceneCoordinate->setText(
                QString::asprintf("Scene坐标:%.0f, %.0f",
                                  point.x(), point.y()));
}

void CurveWindow::on_selectItem(QPointF point)
{
    labItemCoordinate->setText(
                QString::asprintf("控制点Scene坐标: %.1f, %.1f",
                                  point.x(), point.y()));
    ui->editPosX->setText(QString::number(point.x()));
    ui->editPosY->setText(QString::number(point.y()));
}


void CurveWindow::on_btnAddPoint_clicked()
{
    scene->addPointEnable = true;
    ui->mainView->setCursor(Qt::CrossCursor);
}


void CurveWindow::on_btnChangePoint_clicked()
{
    scene->addPointEnable = false;
    ui->mainView->setCursor(Qt::ArrowCursor);
}


void CurveWindow::on_btnDefineEdit_clicked()
{
    if(scene->selectedItems().count() == 1)
    {
        QGraphicsItem* item = scene->selectedItems()[0];
        qreal center_x = ui->editPosX->text().toDouble();
        qreal center_y = ui->editPosY->text().toDouble();
        QPointF newBoundBoxScenePos =
                scene->pointRectPos(QPointF(center_x, center_y));
        //item的位置是以创立位置为远点的局部坐标系
        //但item->sceneBoundingRect()是基于scene坐标系的
        //做差得到位移距离，在item局部坐标系做平移
        QPointF moveDistance = newBoundBoxScenePos
                - QPointF(item->sceneBoundingRect().x(),
                          item->sceneBoundingRect().y());
        item->setPos(item->pos() + moveDistance);
    }
}

