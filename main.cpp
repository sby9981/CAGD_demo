#include "curvewindow.h"
#include <QApplication>
#include <QDebug>
#include <queue>
#include <numeric>
#include <QString>
#include <QtDataVisualization>
#include <QHBoxLayout>


int main(int argc, char *argv[])
{
    qputenv("QSG_RHI_BACKEND", "opengl");
    QApplication a(argc, argv);
    CurveWindow w;
    w.show();

    return a.exec();
}
