#include "surfacewindow.h"
#include "ui_surfacewindow.h"
#include <QFile>
#include <QFileDialog>
#include "bspline.h"

SurfaceWindow::SurfaceWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SurfaceWindow)
{
    ui->setupUi(this);

    //选择曲线类型
    ui->cboUType->addItem("无", static_cast<KnotsType>(NotDefine));
    ui->cboUType->addItem("均匀B样条", static_cast<KnotsType>(Uniform));
    ui->cboUType->addItem("准均匀B样条", static_cast<KnotsType>(Quasi_uniform));
    ui->cboUType->addItem("非均匀Riesenfeld方法", static_cast<KnotsType>(Riesenfeld));
    ui->cboUType->addItem("非均匀Hartley_Judd方法", static_cast<KnotsType>(Hartley_Judd));
    ui->cboUType->addItem("Bezier", static_cast<KnotsType>(Bezier));
    ui->cboVType->addItem("无", static_cast<KnotsType>(NotDefine));
    ui->cboVType->addItem("均匀B样条", static_cast<KnotsType>(Uniform));
    ui->cboVType->addItem("准均匀B样条", static_cast<KnotsType>(Quasi_uniform));
    ui->cboVType->addItem("非均匀Riesenfeld方法", static_cast<KnotsType>(Riesenfeld));
    ui->cboVType->addItem("非均匀Hartley_Judd方法", static_cast<KnotsType>(Hartley_Judd));
    ui->cboVType->addItem("Bezier", static_cast<KnotsType>(Bezier));

    graphSurf = new Q3DSurface;
//    graphScatter = new Q3DScatter;
    container = QWidget::createWindowContainer(graphSurf, ui->centralwidget);
    container->setGeometry(QRect(0, 160, 900, 900));
    serieSurf = new QSurface3DSeries;
    serieSurf->setDrawMode(QSurface3DSeries::DrawSurfaceAndWireframe);
    serieSurf = new QSurface3DSeries;
    graphSurf->addSeries(serieSurf);

//    seriesScatter = new QScatter3DSeries;
//    graphScatter->addSeries(seriesScatter);

//    graphSurf->axisX()->setRange(0.0f, 400.0f);
//    graphSurf->axisY()->setRange(0.0f, 400.0f);
//    graphSurf->axisZ()->setRange(0.0f, 400.0f);
//    graphSurf->setAxisX()
}

SurfaceWindow::~SurfaceWindow()
{
    delete ui;
    delete container;
    delete graphSurf;
    delete serieSurf;
}



void SurfaceWindow::on_pbtReadFile_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,
                                                    tr("Open Image"),
                                                    "E:/Projects/Cpp/Qt/CAGD/demo3/data",
                                                    tr("Image Files (*.txt *.cpp *.h)")
                                                    );

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    int u_degree = ui->textUdegree->text().toInt();
    int v_degree = ui->textVdegree->text().toInt();
    //    int u_ctrPointsNum = ui->textUctrPointsNum->text().toInt();
    //    int v_ctrPointsNum = ui->textVctrPointsNum->text().toInt();

    //文件第一行为控制点数
    auto info = file.readLine().split(' ');
    int u_ctrPointsNum = info[0].toInt();
    int v_ctrPointsNum = info[1].toInt();
    ui->textUctrPointsNum->setText(QString::number(u_ctrPointsNum));
    ui->textVctrPointsNum->setText(QString::number(v_ctrPointsNum));

    ctrPoints.clear();
    QSurfaceDataArray *dataArray = new QSurfaceDataArray;
    for(int i=0; i< v_ctrPointsNum; i++)
    {
        QSurfaceDataRow *newRow = new QSurfaceDataRow(u_ctrPointsNum);

        vector<QVector3D> tempUCtps;
        for(int j=0; j<u_ctrPointsNum; j++)
        {
            if(file.atEnd())
            {
                break;
            }
            QByteArray line = file.readLine();

            auto numberList = line.split(' ');
            QVector3D p(numberList[0].toDouble(),
                        numberList[1].toDouble(),
                        numberList[2].toDouble());
//            qDebug() << p;
            tempUCtps.push_back(p);
            (*newRow)[j].setPosition(p);
        }
        *dataArray << newRow;
        ctrPoints.push_back(tempUCtps);
//        qDebug()<<"";
    }

    serieSurf->dataProxy()->resetArray(dataArray);
}


void SurfaceWindow::on_pbtClear_clicked()
{
    QSurfaceDataArray *dataArray = new QSurfaceDataArray;
    serieSurf->dataProxy()->resetArray(dataArray);
}


void SurfaceWindow::on_pbtDrawSurf_clicked()
{
    if(ctrPoints.empty())
    {
        ui->textBrowser->setText("无控制点");
        return;
    }

    int u_degree = ui->textUdegree->text().toInt();
    int v_degree = ui->textVdegree->text().toInt();
//    int u_ctrPointsNum = ui->textUctrPointsNum->text().toInt();
//    int v_ctrPointsNum = ui->textVctrPointsNum->text().toInt();
    int u_ctrPointsNum = ctrPoints[0].size();
    int v_ctrPointsNum = ctrPoints.size();
//    qDebug()<<u_ctrPointsNum<<v_ctrPointsNum;
    KnotsType u_type = static_cast<KnotsType>(ui->cboUType->currentIndex());
    KnotsType v_type = static_cast<KnotsType>(ui->cboVType->currentIndex());

    if(u_type==NotDefine || v_type==NotDefine)
    {
        ui->textBrowser->setText("请设置曲线类型");
        return;
    }

    BsplineSurface surf(u_degree, v_degree, u_ctrPointsNum, v_ctrPointsNum);
    surf.setUType(u_type);
    surf.setVType(v_type);

    if(!surf.isU_DrawEnable())
    {
        ui->textBrowser->setText("U向次数和控制点数不匹配");
        return;
    }
    if(!surf.isV_DrawEnable())
    {
        ui->textBrowser->setText("V向次数和控制点数不匹配");
        return;
    }

    QSurfaceDataArray *dataArray = new QSurfaceDataArray;
    for(double v{0.0}; v<1.0; v+=0.01)
    {
        QSurfaceDataRow *newRow = new QSurfaceDataRow;
        int i=0;
        for(double u{0.0}; u<1.0; u+=0.01)
        {
            QVector3D puv;
            if(surf.calSurfPos(ctrPoints, puv, u, v))
            {
//                qDebug() << puv;
                *newRow<<puv;
                i++;
            }
        }
        if(i>1)
        {
            *dataArray << newRow;
//            qDebug()<<"";
        }
        i=0;
    }
    serieSurf->dataProxy()->resetArray(dataArray);
    ui->textBrowser->setText("");
}

