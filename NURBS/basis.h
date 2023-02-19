#ifndef BASIS_H
#define BASIS_H

#include <vector>
#include <iostream>
#include <queue>
using std::vector;

#define MIN_POSITIVE_NUM 1e-5
#define USE_QTPOINT true

#if USE_QTPOINT
#include <QPointF>
#include <QList>
#include <QVector3D>
#endif

enum KnotsType
{
    NotDefine,
    Uniform,
    Quasi_uniform,
    Riesenfeld,
    Hartley_Judd
};


class KnotsVector
{
public:
    KnotsVector();
    KnotsVector(int degree, int controlPointsNum);

    void reset(int degree, int controlPointsNum);
    void setDegree(int newDegree);
    void setCtrPointsNum(int newCtrPointsNum);
    void operator=(const KnotsVector& other);

    double at(int i) const;
    double* data();
    KnotsType type() const;
    vector<double>& vec();

    //对u的判断
    int getParamRangeId(double u);  //判断u所在的区间序号
    bool isInDomain(double u);      //判断u是否在定义域内

    //设置类型
    void setNotDefine();
    void setUnifrom();
    void setQuasiUniform();
    void setRiesenfeld(vector<double>& polylength);
    void setHartley_Judd(vector<double>& polylength);


    int degree() const;

    int ctrPointsNum() const;

private:
    int m_degree {0};
    int m_ctrPointsNum {0};
    KnotsType m_type {NotDefine};
    vector<double> m_knots;

    //设置首尾节点重复度为 degree+1
    void m_setEdgeRepeatKnot();
};

class BasisFunction
{

public:
    KnotsVector knots;
public:
    BasisFunction(){};
    BasisFunction(int degree, int controlPointsNum);
    void reset(int degree, int controlPointsNum);
    void reset(KnotsVector& otherKnots);
    vector<vector<double>> evaluate(double interval=0.01);
    vector<vector<double>> evaluate(vector<double>* interp);
    bool isDrawEnable();
private:
    int m_degree {0};
    int m_ctrPointsNum {0};
};

double getBasisFunVal(double u, double* knots, int i, int k);


#if USE_QTPOINT
QPointF evaluateDeBoorCoeff(
        double u, QList<QPointF> &ctrPoints, double* knots, int i, int k);
vector<double> getPolyLength(QList<QPointF> points_list);
vector<double> getPolyLength(vector<QPointF> points_list);
vector<double> getPolyLength(vector<QVector3D> points_list);
#endif

template<typename T>
T evaluateDeBoorCoeff(double u, vector<T> &ctrPoints, double* knots, int i, int k)
{
    /*
    计算B样条曲线上点的德布尔算法
    CAGD p242  7.6b
    INPUT:
        u:          指定的参数值  knots[i] < u < knots[i+1]
        ctrPoints:  控制点，自定义类型必须定义 *,+,=运算
        knots:      节点向量
        i:          u所在的节点区间标号
                    knots[k] <= knots[i] < u < knots[i+1] <= knots[ctrPointsNum]
        k:          次数 degree
    OUTPUT:
        Val:    = p(u) 曲线在该点的坐标
*/

    std::queue<vector<T>> coeff;  // save d
    coeff.push(ctrPoints);
    T d;
    for(int l = 1; l < k+1; l++)
    {
        vector<T> pre_d = coeff.front(); // d(l-1, )
        vector<T> temp_d;                //d(l, )
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
    return static_cast<T>(d);
}


#endif // BASIS_H
