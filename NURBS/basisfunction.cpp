#include "basisfunction.h"
#include <QDebug>

#define MIN_POSITIVE_NUM 1e-7

BasisFunction::BasisFunction(int degree, int controlPointsNum)
    :m_degree(degree), m_ctrPointsNum(controlPointsNum)
{
    m_knots.resize(m_degree + m_ctrPointsNum + 1);
}

void BasisFunction::reset(int degree, int controlPointsNum)
{
    m_degree = degree;
    m_ctrPointsNum = controlPointsNum;
    m_knots.resize(m_degree + m_ctrPointsNum + 1);
}

void BasisFunction::setType(KnotsType newType)
{
    m_type = newType;
    switch (m_type) {
    case uniform:
        setUnifromKnots();
        break;
    case quasi_uniform:
        setQuasiUniformKnots();
        break;
    default:
        break;
    }
}

KnotsType BasisFunction::type() const
{
    return m_type;
}

vector<vector<double>> BasisFunction::evaluate(double interval)
{
    int length = 1.0 / interval;
    vector<vector<double>> output(m_ctrPointsNum, vector<double>(length));

    for (int i=0; i<m_ctrPointsNum; i++)
    {
        for(double u=0, j=0; u <= 1.0; u += interval, j++)
        {
            output[i][j] = getBasisFunVal(
                        u, m_knots.data(), i, m_degree);
        }
    }
    return output;
}

vector<vector<double> > BasisFunction::evaluate(vector<double> *interp)
{
    vector<vector<double>> output(
                m_ctrPointsNum, vector<double>(interp->size()));

    for (int i=0; i<m_ctrPointsNum; i++)
    {
        for(int j=0; j < interp->size(); j++)
        {
            output[i][j] = getBasisFunVal(
                        interp->at(j), m_knots.data(), i, m_degree);
        }
    }
    return output;
}

void BasisFunction::setUnifromKnots()
{
    double interval = 1.0 / (m_degree + m_ctrPointsNum);
    double val = 0;
    for(int i=0; i < m_knots.size(); i++)
    {
        m_knots[i] = val;
        val += interval;
    }
}

void BasisFunction::setQuasiUniformKnots()
{
    double interval = 1.0 / ( -m_degree + m_ctrPointsNum);
    double val = 0;
    for(int i = 0; i < m_degree+1; i++)
    {
        m_knots[i] = 0;
        m_knots.end()[-i-1] = 0;
    }
    for(int i = m_degree; i < m_knots.size()-m_degree-1; i++)
    {
        m_knots[i] = val;
        val += interval;
    }
}

double getBasisFunVal(double u, double *knots, int i, int k)
/*
    CAGD p220
    在指定的参数值处计算基函数值，采用递归的方法
    INPUT:
        u:      指定的参数值
        knots:  节点向量
        i:      基函数下标，标记节点区间，序号
        k:      次数
    OUTPUT:
        Val:    =N(i,k)(u)
*/
{

    double Val=0.0;
    double val1=0.0;
    double val2=0.0;
    if(k == 0 )
    {
        if((u <= knots[i] && knots[i] != 0) || u > knots[i+1])
        {
            return 0;
        }
        else
        {
            return 1;
        }
    }
    if(k > 0)
    {

        if(u < knots[i] || u > knots[i+k+1])
        {
            return Val;
        }
        else
        {
            double alpha = 0.0;
            double beta = 0.0;
            double dTemp = 0.0;
            dTemp = knots[i+k] - knots[i];

            if(dTemp < MIN_POSITIVE_NUM)
            {
                alpha = 0.0;
            }
            else
            {
                alpha = (u - knots[i]) / dTemp;
            }
            dTemp = knots[i+k+1] - knots[i+1];

            if(dTemp < MIN_POSITIVE_NUM)
            {
                beta = 0.0;
            }
            else
            {
                beta = (knots[i+k+1]-u) / dTemp;
            }

            val1 = alpha*getBasisFunVal(u, knots, i,k-1);
            val2 = beta*getBasisFunVal(u, knots, i+1,k-1);
            Val = val1+val2;
        }
    }
    return Val;
}

