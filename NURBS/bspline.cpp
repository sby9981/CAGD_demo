#include "bspline.h"
#include <QDebug>
#define MIN_POSITIVE_NUM 1e-5

BSpline::BSpline(int degree, int controlPointsNum)
    :m_degree(degree), m_ctrPointsNum(controlPointsNum)
{
    knots.reset(degree, controlPointsNum);
}

void BSpline::reset(int degree, int controlPointsNum)
{
    m_degree = degree;
    m_ctrPointsNum = controlPointsNum;
    knots.reset(degree, controlPointsNum);
}

bool BSpline::isDrawEnable()
{
    return isDrawEnable(
                m_degree, m_ctrPointsNum, knots.type());
}

bool BSpline::isDrawEnable(
        int degree, int controlPointsNum, KnotsType type)
{
    if(degree < 1)
    {
        return false;
    }
    switch (type)
    {
    case NotDefine:
        return false;
    case Uniform:
        if(controlPointsNum > degree)
        {
            return true;
        }
        else
        {
            return false;
        }
    case Quasi_uniform:
        if(controlPointsNum > 2 &&
               degree < controlPointsNum)
        {
            return true;
        }
        else
        {
            return false;
        }
    case Riesenfeld:
        if(controlPointsNum > degree)
        {
            return true;
        }
        else
        {
            return false;
        }
    case Hartley_Judd:
        if(controlPointsNum > degree)
        {
            return true;
        }
        else
        {
            return false;
        }
    default:
        break;
    }
    return false;
}

void BSpline::setDegree(int newDegree)
{
    m_degree = newDegree;
    knots.setDegree(newDegree);
}

void BSpline::setCtrPointsNum(int newCtrPointsNum)
{
    m_ctrPointsNum = newCtrPointsNum;
    knots.setCtrPointsNum(newCtrPointsNum);
}


#if USE_QTPOINT
QList<QPointF> BSpline::evaluate(
        QList<QPointF> &ctrpoints, double interval)
{
    //生成样条曲线上的采样点
    QList<QPointF> curve;
    for(double u = 0.0; u < 1.0+interval; u += interval)
    {
        if(knots.isInDomain(u))
        {
            QPointF pu = evaluateDeBoorCoeff(
                        u, ctrpoints, knots.data(),
                        knots.getParamRangeId(u), m_degree
                        );

            curve.append(pu);
        }
    }
    return curve;
}
#endif

//#if USE_QTPOINT
//QList<QList<QVector3D>> BsplineSurface::evaluate(
//        QList<QList<QVector3D>> &ctrpoints, double uInterval, double vInterval)
///*
//    INPUT:
//    ctrpoints:  内层为u向，及固定v，外层为v向

//    OUTPUT:
//    surf:       内层为u向，及固定v取不同U，外层为v向
//*/
//{

//    QList<QList<QVector3D>> tempCtrPoints;
//    for(int i{0}; i < ctrpoints[0].size(); i++)
//    {
//        //先做转置，取v向的控制点
//        QList<QVector3D> vCtps;
//        for(int j{0}; j < ctrpoints.size(); j++)
//        {
//            vCtps.append(QVector3D(ctrpoints[j][i]));
//        }
//        //根据控制点，计算v向曲线，作为u向控制点
//        QList<QVector3D> tempVCtrPolygon = vSpline.evaluate(vCtps, vInterval);

//        //tempCtrPoints内层的每个数组为U向的控制点
//        tempCtrPoints.append(tempVCtrPolygon);
//    }

//    QList<QList<QVector3D>> surf;
//    for(int i=0; i<tempCtrPoints.size(); i++)
//    {
//        QList<QVector3D> tempPoints = uSpline.evaluate(tempCtrPoints[i], uInterval);
//        surf.append(tempCtrPoints);
//    }
//    return surf;
//}
//#endif

void BsplineSurface::reset(int uDegree, int vDegree, int uCtrPointsNum, int vCtrPointsNum)
{
    m_uDegree = uDegree;
    m_uCtrPointsNum = uCtrPointsNum;
    uSpline.reset(uDegree, uCtrPointsNum);

    m_vDegree = vDegree;
    m_vCtrPointsNum = vCtrPointsNum;
    vSpline.reset(vDegree, vCtrPointsNum);
}
