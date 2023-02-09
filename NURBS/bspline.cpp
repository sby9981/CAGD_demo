#include "bspline.h"
#include <queue>
using std::queue;

namespace MyBSpline
{
#define MIN_POSITIVE_NUM 1e-5


//template<typename T>
double evaluateDeBoorCoeff(
        double u, vector<double> &ctrPoints, double *knots, int i, int k)
{
    queue<vector<double>> coeff;  // save d
    coeff.push(ctrPoints);
    double d;
    for(int l = 1; l < k+1; l++)
    {
        vector<double> pre_d = coeff.front(); // d(l-1, )
        vector<double> temp_d;                //d(l, )
        for(int j = i-k; j<i-l+1;j++)
        {
            double denom = knots[j+k+1] - knots[j+l];
            double alpha = 0;
            if(denom > MIN_POSITIVE_NUM)
            {
                alpha = (u - knots[j+l]) / denom;
            }
            if(l==1)
            {
                // d(l,j) = (1-alpha)*d(l-1,j) + alpha*d(l-1,j+1)
                d = (1 - alpha) * pre_d[j]
                        + alpha * pre_d[j+1];
            }
            else
            {
                //由于d的数组长度逐渐变短,需要调整角标
                //d角标均从i-k开始,调整为0, d(l, i-k) => d(l,0)
                int bias = i-k;
                d = (1 - alpha) * pre_d[j-bias]
                        + alpha * pre_d[j+1-bias];
            }
            temp_d.push_back(d);
        }
        coeff.push(temp_d);
        coeff.pop();
    }
    return d;
}
}
