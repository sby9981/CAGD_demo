#include "basis.h"
#include <QDebug>
#include <numeric>
#include <queue>

KnotsVector::KnotsVector()
{}

KnotsVector::KnotsVector(int degree, int controlPointsNum)
    :m_degree(degree), m_ctrPointsNum(controlPointsNum)
{
    m_knots.resize(m_degree + m_ctrPointsNum + 1);
}

void KnotsVector::reset(int degree, int controlPointsNum)
{
    if(degree < 0 || controlPointsNum < 1)
    {
        return;
    }
    m_degree = degree;
    m_ctrPointsNum = controlPointsNum;
    m_knots.resize(m_degree + m_ctrPointsNum + 1);
}

double KnotsVector::at(int i) const
{
    return m_knots[i];
}

double *KnotsVector::data()
{
    return m_knots.data();
}

void KnotsVector::setUnifrom()
{
    m_type = uniform;
    double interval = 1.0 / (m_degree + m_ctrPointsNum);
    double val = 0;
    for(int i=0; i < m_knots.size(); i++)
    {
        m_knots[i] = val;
        val += interval;
    }
}

void KnotsVector::setQuasiUniform()
{
    m_type = quasi_uniform;
    double interval = 1.0 / ( -m_degree + m_ctrPointsNum);
    double val = interval;

    m_setEdgeRepeatKnot();
    for(int i = m_degree + 1; i < m_knots.size()-m_degree-1; i++)
    {
        m_knots[i] = val;
        val += interval;
    }
}

void KnotsVector::setRiesenfeld(vector<double>& polylength)
{
    if(polylength.size() != m_ctrPointsNum-1)
    {
        return;
    }

    //分母 L
    double totalLength = std::accumulate(
                polylength.begin(), polylength.end(), 0.0);

    m_setEdgeRepeatKnot();

    if(m_degree % 2 == 0)
    {
        int control_j = m_degree/2;
        for(int i = m_degree + 1; i < m_knots.size()-m_degree-1; i++)
        {
            double val = 0;
            for(int j=0; j < control_j; j++)
            {
                val += polylength[j];
            }
            val += polylength[control_j] / 2;
            val /= totalLength;
            m_knots[i] = val;

            control_j++;
        }
    }
    else
    {
        int control_j = (m_degree+1)/2;
        for(int i = m_degree + 1; i < m_knots.size()-m_degree-1; i++)
        {
            double val = 0;
            for(int j=0; j < control_j; j++)
            {
                val += polylength[j];
            }
            val /= totalLength;
            m_knots[i] = val;

            control_j++;
        }
    }
}

void KnotsVector::setHartley_Judd(vector<double> &polylength)
{
    if(polylength.size() != m_ctrPointsNum-1)
    {
        return;
    }
    //分母 L
    double denominator {0};
    for(int i = m_degree+1; i < m_ctrPointsNum; i++)
    {
        for(int j = i-m_degree-1; j < i; j++)
        {
            denominator += polylength[j];
        }
    }

    m_setEdgeRepeatKnot();

    double pre_val {0};
    for(int i = m_degree + 1; i < m_knots.size()-m_degree-1; i++)
    {
        for(int j = i-m_degree-1; j < i-1; j++)
        {
            pre_val += polylength[j];
        }
        m_knots[i] = pre_val / denominator;
    }
}

KnotsType KnotsVector::type() const
{
    return m_type;
}

vector<double> &KnotsVector::vec()
{
    return m_knots;
}

int KnotsVector::getParamRangeId(double u)
{
    for(int i=0; i<m_knots.size(); i++)
    {
        if(u < m_knots[i])
        {
            return i-1;
        }
    }
    return m_knots.size();
}

bool KnotsVector::isInDomain(double u)
{
    if(u < m_knots[m_degree] || u > m_knots[m_ctrPointsNum])
    {
        return false;
    }
    return true;
}

void KnotsVector::m_setEdgeRepeatKnot()
{
    for(int i = 0; i < m_degree+1; i++)
    {
        m_knots[i] = 0.0;
        m_knots.end()[-i-1] = 1.0;
    }
}

BasisFunction::BasisFunction(int degree, int controlPointsNum)
    :m_degree(degree), m_ctrPointsNum(controlPointsNum)
{
    knots.reset(degree, controlPointsNum);
}

void BasisFunction::reset(int degree, int controlPointsNum)
{
    m_degree = degree;
    m_ctrPointsNum = controlPointsNum;
    knots.reset(degree, controlPointsNum);
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
                        u, knots.data(), i, m_degree);
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
                        interp->at(j), knots.data(), i, m_degree);
        }
    }
    return output;
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


//template<typename T>
//T evaluateDeBoorCoeff(double u, vector<T> &ctrPoints, double *knots, int i, int k)
//{
///*
//    计算B样条曲线上点的德布尔算法
//    CAGD p242  7.6b
//    INPUT:
//        u:          指定的参数值  knots[i] < u < knots[i+1]
//        ctrPoints:  控制点，自定义类型必须定义 *,+,=运算
//        knots:      节点向量
//        i:          u所在的节点区间标号
//                    knots[k] <= knots[i] < u < knots[i+1] <= knots[ctrPointsNum]
//        k:          次数 degree
//    OUTPUT:
//        Val:    = p(u) 曲线在该点的坐标
//*/

//    std::queue<vector<T>> coeff;  // save d
//    coeff.push(ctrPoints);
//    T d;
//    for(int l = 1; l < k+1; l++)
//    {
//        vector<T> pre_d = coeff.front(); // d(l-1, )
//        vector<T> temp_d;                //d(l, )
//        for(int j = i-k; j<i-l+1;j++)
//        {
//            double denominator = knots[j+k+1] - knots[j+l];
//            double alpha = 0;
//            if(denominator > MIN_POSITIVE_NUM)
//            {
//                alpha = (u - knots[j+l]) / denominator;
//            }
//            if(l==1)
//            {
//                // d(l,j) = (1-alpha)*d(l-1,j) + alpha*d(l-1,j+1)
//                d = (1 - alpha) * pre_d[j]
//                        + alpha * pre_d[j+1];
//            }
//            else
//            {
//                //由于d的数组长度逐渐变短,需要调整角标
//                //d角标均从i-k开始,调整为0, d(l, i-k) => d(l,0)
//                int bias = i-k;
//                d = (1 - alpha) * pre_d[j-bias]
//                        + alpha * pre_d[j+1-bias];
//            }
//            temp_d.push_back(d);
//        }
//        coeff.push(temp_d);
//        coeff.pop();
//    }
//    return static_cast<T>(d);
//}

//template double evaluateDeBoorCoeff<double>(double u, vector<double> &ctrPoints, double *knots, int i, int k);
//template point3 evaluateDeBoorCoeff<point3>(double u, vector<point3> &ctrPoints, double *knots, int i, int k);

#if USE_QTPOINT
QPointF evaluateDeBoorCoeff(
        double u, QList<QPointF> &ctrPoints, double* knots, int i, int k)
{
    std::queue<QList<QPointF>> coeff;  // save d
    coeff.push(ctrPoints);
    QPointF d;
    for(int l = 1; l < k+1; l++)
    {
        QList<QPointF> pre_d = coeff.front(); // d(l-1, )
        QList<QPointF> temp_d;                //d(l, )
        for(int j = i-k; j<i-l+1;j++)
        {
            double denominator = knots[j+k+1] - knots[j+l];
            double alpha = 0;
            if(denominator > MIN_POSITIVE_NUM)
            {
                alpha = (u - knots[j+l]) / denominator;
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
#endif
