#include "curvewindow.h"

#include <QApplication>
#include <QDebug>
#include "bspline.h"
#include "basis.h"
#include <queue>
#include <numeric>
#include <QString>

using std::accumulate;

QString showP(point3 p)
{
    return QString("(%1,%2,%3)")
            .arg(p.x).arg(p.y).arg(p.z);
}

int main(int argc, char *argv[])
{
    qDebug()<<"helloworld";
    double px[] {300, 300, 500, 500, 700, 700};
    double py[] {400, 200, 200, 400, 400, 200};
    vector<double> vx{300, 300, 500, 500, 700, 700};
    vector<double> vy{400, 200, 200, 400, 400, 200};
    double knots[] {-1., -2./3., -1./3., 0, 0.3333, 0.6667, 1, 1.3333, 1.6667, 2};
    double p = evaluateDeBoorCoeff<double>(0.5, vy, knots, 4, 3);
    KnotsVector knotsVec(2, 5);
    knotsVec.setUnifrom();
    qDebug()<<knotsVec.vec();
    knotsVec.setQuasiUniform();
    qDebug()<<knotsVec.vec();

    vector<double> l {100, 120, 130, 140};
    knotsVec.setRiesenfeld(l);
    qDebug()<<knotsVec.vec();

    knotsVec.setHartley_Judd(l);
    qDebug() << knotsVec.vec();
    QList<QPointF> plist{QPointF(300,400), QPointF(300,200),
                         QPointF(500,200), QPointF(500,400),
                         QPointF(700,400), QPointF(700,200)};
    QPointF pp = evaluateDeBoorCoeff(0.5, plist, knots, 4, 3);
    qDebug() << pp;

//    QApplication a(argc, argv);
//    CurveWindow w;
//    w.show();
//    return a.exec();
    return 0;
}
