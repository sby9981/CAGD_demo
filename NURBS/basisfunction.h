#ifndef BASISFUNCTION_H
#define BASISFUNCTION_H

#include <vector>
using std::vector;

enum KnotsType
{
    null,
    uniform,
    quasi_uniform,
};

class BasisFunction
{

public:
    BasisFunction(int degree, int controlPointsNum);
    void reset(int degree, int controlPointsNum);
    void setType(KnotsType newType);
    KnotsType type() const;
    vector<vector<double>> evaluate(double interval=0.01);
    vector<vector<double>> evaluate(vector<double>* interp);
private:
    int m_degree;
    int m_ctrPointsNum;
    KnotsType m_type {null};
    vector<double> m_knots;

    void setUnifromKnots();
    void setQuasiUniformKnots();
};

double getBasisFunVal(double u, double* knots, int i, int k);

#endif // BASISFUNCTION_H
