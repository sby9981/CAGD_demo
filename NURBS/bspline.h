#ifndef BSPLINE_H
#define BSPLINE_H

#include "basis.h"

class BSpline
{
public:
    KnotsVector knots;


    BSpline() {};
    BSpline(int degree, int controlPointsNum);

    void reset(int degree, int controlPointsNum);

    template<typename T_point>
    vector<T_point> evaluate(vector<T_point> &ctrpoints, double interval)
    {
        vector<T_point> curve;
        for(double u = 0; u < 1.0; u += interval)
        {
            if(knots.isInDomain(u))
            {
                T_point pu = evaluateDeBoorCoeff<T_point>(
                            u, ctrpoints, knots.data(),
                            knots.getParamRangeId(u), m_degree
                            );

                curve.pop_back(pu);
            }
        }
        return curve;
    }

#if USE_QTPOINT
    QList<QPointF> evaluate(QList<QPointF> &ctrpoints, double interval);
#endif

private:
    int m_degree;
    int m_ctrPointsNum;
};

#endif // BSPLINE_H
