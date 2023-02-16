#include "bspline.h"
#include <QDebug>
#define MIN_POSITIVE_NUM 1e-5

BSpline::BSpline(int degree, int controlPointsNum)
    :m_degree(degree), m_ctrPointsNum(controlPointsNum)
{
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

