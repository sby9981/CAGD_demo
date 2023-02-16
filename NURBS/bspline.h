#ifndef BSPLINE_H
#define BSPLINE_H

#include "basis.h"

class BSpline
{
public:
    KnotsVector knots;

    explicit BSpline() {};
    explicit BSpline(int degree, int controlPointsNum);

    void reset(int degree, int controlPointsNum);
    bool isDrawEnable();
    static bool isDrawEnable(int degree, int controlPointsNum, KnotsType type);

    template<typename T_point>
    vector<T_point> evaluate(vector<T_point> &ctrpoints, double interval=0.01)
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
    QList<QPointF> evaluate(QList<QPointF> &ctrpoints, double interval=0.01);
#endif

    void setDegree(int newDegree);
    void setCtrPointsNum(int newCtrPointsNum);

private:
    int m_degree {0};
    int m_ctrPointsNum {0};
};

#endif // BSPLINE_H
