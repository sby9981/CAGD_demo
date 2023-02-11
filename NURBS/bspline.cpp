#include "bspline.h"

#define MIN_POSITIVE_NUM 1e-5

BSpline::BSpline(int degree, int controlPointsNum)
    :m_degree(degree), m_ctrPointsNum(controlPointsNum)
{
    knots.reset(degree, controlPointsNum);
}

//template<typename T_point>
//vector<T_point> BSpline::evaluate(
//        vector<T_point> &ctrpoints, double interval)
//{
//    vector<T_point> curve;
//    for(double u = 0; u < 1.0; u += interval)
//    {
//        if(knots.isInDomain(u))
//        {
//            T_point pu = evaluateDeBoorCoeff<T_point>(
//                        u, ctrpoints, knots.data(),
//                        knots.getParamRangeId(u), m_degree
//                        );

//            curve.pop_back(pu);
//        }
//    }
//    return curve;
//}

#if USE_QTPOINT
QList<QPointF> BSpline::evaluate(
        QList<QPointF> &ctrpoints, double interval)
{
    QList<QPointF> curve;
    for(double u = 0; u < 1.0; u += interval)
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

