#include "surfacewindow.h"
#include "ui_surfacewindow.h"
#include <QFile>
#include <QFileDialog>

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
    ui->cboVType->addItem("无", static_cast<KnotsType>(NotDefine));
    ui->cboVType->addItem("均匀B样条", static_cast<KnotsType>(Uniform));
    ui->cboVType->addItem("准均匀B样条", static_cast<KnotsType>(Quasi_uniform));
    ui->cboVType->addItem("非均匀Riesenfeld方法", static_cast<KnotsType>(Riesenfeld));
    ui->cboVType->addItem("非均匀Hartley_Judd方法", static_cast<KnotsType>(Hartley_Judd));

    graphSurf = new Q3DSurface;
    container = QWidget::createWindowContainer(graphSurf, ui->centralwidget);
    container->setGeometry(QRect(0, 200, 800, 800));
    serieCtrPoints = new QSurface3DSeries;
    serieSurf = new QSurface3DSeries;
    graphSurf->addSeries(serieCtrPoints);

}

SurfaceWindow::~SurfaceWindow()
{
    delete ui;
    delete container;
    delete graphSurf;
    delete serieCtrPoints;
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
            qDebug() << p;
            tempUCtps.push_back(p);
            (*newRow)[j].setPosition(p);
        }
        *dataArray << newRow;
        ctrPoints.push_back(tempUCtps);
        qDebug()<<"";
    }

    serieCtrPoints->dataProxy()->resetArray(dataArray);
}


void SurfaceWindow::on_pbtClear_clicked()
{
    QSurfaceDataArray *dataArray = new QSurfaceDataArray;
    serieCtrPoints->dataProxy()->resetArray(dataArray);
}

