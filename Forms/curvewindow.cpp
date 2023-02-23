#include "curvewindow.h"
#include "./ui_curvewindow.h"
#include <QMouseEvent>
#include <QDebug>
#include <QPainter>
#include <QString>
#include "surfacewindow.h"
#include <QtDataVisualization>
CurveWindow::CurveWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CurveWindow)
{
    ui->setupUi(this);

    //底部显示鼠标位置
    labViewCoordinate = new QLabel("光标View坐标");
    labViewCoordinate->setMinimumWidth(150);
    ui->statusbar->addWidget(labViewCoordinate);

    labSceneCoordinate = new QLabel("光标Scene坐标");
    labSceneCoordinate->setMinimumWidth(150);
    ui->statusbar->addWidget(labSceneCoordinate);

    labItemCoordinate = new QLabel("控制点坐标");
    labItemCoordinate->setMinimumWidth(150);
    ui->statusbar->addWidget(labItemCoordinate);

    //选择曲线类型
    ui->cboCurveType->addItem("无", static_cast<KnotsType>(NotDefine));
    ui->cboCurveType->addItem("均匀B样条", static_cast<KnotsType>(Uniform));
    ui->cboCurveType->addItem("准均匀B样条", static_cast<KnotsType>(Quasi_uniform));
    ui->cboCurveType->addItem("非均匀Riesenfeld方法", static_cast<KnotsType>(Riesenfeld));
    ui->cboCurveType->addItem("非均匀Hartley_Judd方法", static_cast<KnotsType>(Hartley_Judd));
    ui->cboCurveType->addItem("Bezier曲线", static_cast<KnotsType>(Bezier));

    ui->textCurveInterval->setText("0.01");

    //Graphics setting
    ui->mainView->setCursor(Qt::CrossCursor);
    ui->mainView->setMouseTracking(true);

    //将scene左上角与view窗口左上角重合，且大小一致
    QRectF viewR = ui->mainView->geometry();
    scene = new MainCurveScene();
    scene->setSceneRect(-viewR.width()/2, -viewR.height()/2,
                        viewR.width()-2, viewR.height()-2);


    ui->mainView->setScene(scene);

    connect(scene, &MainCurveScene::mouseMove, this, &CurveWindow::on_mouseMove);
    connect(scene, &MainCurveScene::selectItemScenePos, this, &CurveWindow::on_selectItem);

    connect(scene, &MainCurveScene::defineKnotsVec,
            ui->basisFunWidget, &BasisFunWidget::on_defineKnotsVec);

    connect(scene, &MainCurveScene::textInfo, this, &CurveWindow::on_textInfo);

    //定时刷新绘图
    timer = new QTimer();
    timer->setInterval(30);
    connect(timer, &QTimer::timeout, ui->basisFunWidget, &BasisFunWidget::on_timeout);
    timer->start();
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
    if(scene->selectedItems().count()==1 &&
            !ui->editPosX->text().isEmpty() &&
            !ui->editPosY->text().isEmpty())
    {
        bool isNum1, isNum2;
        QGraphicsItem* item = scene->selectedItems()[0];
        qreal center_x = ui->editPosX->text().toDouble(&isNum1);
        qreal center_y = ui->editPosY->text().toDouble(&isNum2);
        if(!isNum1 || !isNum2)
        {
            return;
        }
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


void CurveWindow::on_btnClear_clicked()
{
    scene->removeAllPoint();
}


void CurveWindow::on_spbDegree_valueChanged(int arg1)
{
    scene->bsplineDegree = arg1;
    scene->bsplineCurve->setDegree(arg1);
}


void CurveWindow::on_cboCurveType_activated(int index)
{
    scene->bsplineType = static_cast<KnotsType>(index);
}


void CurveWindow::on_pushButton_clicked()
{
    SurfaceWindow *surfW = new SurfaceWindow();
    surfW->setWindowTitle("曲面绘制");
    surfW->show();
}

void CurveWindow::on_textInfo(QString info)
{
    ui->textBrowser->setText(info);
}


void CurveWindow::on_textCurveInterval_returnPressed()
{
    bool isNum;
    double interval = ui->textCurveInterval->text().toDouble(&isNum);
    if(isNum)
        scene->curveInterval = interval;
}

