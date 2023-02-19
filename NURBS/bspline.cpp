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

#if USE_QTPOINT
vector<vector<QVector3D>> BsplineSurface::evaluate(
        vector<vector<QVector3D>> &ctrpoints, double uInterval, double vInterval)
/*
    INPUT:
    ctrpoints:  内层为u向，及固定v，外层为v向

    OUTPUT:
    surf:       内层为u向，及固定v取不同U，外层为v向
*/
{

    vector<vector<QVector3D>> tempCtrPoints;
    for(int i{0}; i < ctrpoints[0].size(); i++)
    {
        BSpline vSpline(m_vDegree, m_vCtrPointsNum);

        //先做转置，取v向的控制点
        vector<QVector3D> vCtps;
        for(int j{0}; j < ctrpoints.size(); j++)
        {
            vCtps.push_back(QVector3D(ctrpoints[j][i]));
        }

        //根据控制点，计算v向曲线，作为u向控制点
        vSpline.setType(m_vType, vCtps);
        vector<QVector3D> tempVCtrPolygon = vSpline.evaluate(vCtps, vInterval);

        //tempCtrPoints内层的每个数组为U向的控制点
        tempCtrPoints.push_back(tempVCtrPolygon);
    }

    vector<vector<QVector3D>> surf;
    for(int i=0; i<tempCtrPoints.size(); i++)
    {
        BSpline uSpline(m_uDegree, m_uCtrPointsNum);
        uSpline.setType(m_uType, tempCtrPoints[i]);
        vector<QVector3D> tempPoints = uSpline.evaluate(tempCtrPoints[i], uInterval);
        surf.push_back(tempPoints);
    }
    return surf;
}
#endif


void BsplineSurface::setUDegree(int newUDegree)
{
    m_uDegree = newUDegree;
}

void BsplineSurface::setVDegree(int newVDegree)
{
    m_vDegree = newVDegree;
}

void BsplineSurface::setUCtrPointsNum(int newUCtrPointsNum)
{
    m_uCtrPointsNum = newUCtrPointsNum;
}

void BsplineSurface::setVCtrPointsNum(int newVCtrPointsNum)
{
    m_vCtrPointsNum = newVCtrPointsNum;
}

void BsplineSurface::setUType(KnotsType newUType)
{
    m_uType = newUType;
}

void BsplineSurface::setVType(KnotsType newVType)
{
    m_vType = newVType;
}
