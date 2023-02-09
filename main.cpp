#include "curvewindow.h"

#include <QApplication>
#include <QDebug>
#include "bspline.h"

int main(int argc, char *argv[])
{
    qDebug()<<"helloworld";
    double px[] {300, 300, 500, 500, 700, 700};
    double py[] {400, 200, 200, 400, 400, 200};
    vector<double> vx{300, 300, 500, 500, 700, 700};
    vector<double> vy{400, 200, 200, 400, 400, 200};
    double knots[] {-1., -2./3., -1./3., 0, 0.3333, 0.6667, 1, 1.3333, 1.6667, 2};
    auto p = MyBSpline::evaluateDeBoorCoeff(0.5, vx, knots, 4, 3);
    qDebug()<<p;

//    QApplication a(argc, argv);
//    CurveWindow w;
//    w.show();
//    return a.exec();
    return 0;
}
