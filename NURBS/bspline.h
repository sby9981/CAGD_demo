#ifndef BSPLINE_H
#define BSPLINE_H

#include <vector>
using std::vector;

namespace MyBSpline
{

struct point
{
    double x;
    double y;
};

class BSpline
{
public:
    BSpline();
};

//template<typename T>
double evaluateDeBoorCoeff(double u, vector<double> &ctrPoints, double* knots, int i, int k);

}
#endif // BSPLINE_H
