#include "basisfunwidget.h"
#include <QPainterPath>
#include <numeric>

constexpr double interval {0.01};

BasisFunWidget::BasisFunWidget(QWidget *parent)
    : QWidget{parent}
{
    this->setAttribute(Qt::WA_StyledBackground, true);

    for(double u = 0; u < 1.0; u += interval)
    {
        m_interp.push_back(u);
    }
    basisFun.knots.setQuasiUniform();
}

void BasisFunWidget::paintEvent(QPaintEvent *event)
{
//    绘制
//    static const QPoint points[4] = {
//        QPoint(10, 80),
//        QPoint(20, 10),
//        QPoint(80, 30),
//        QPoint(90, 70)
//    };
//    QRect rect{10, 20, 80, 60};
    //创建画家对象
    //参数为绘图设备, this表示在当前窗口绘制
    QPainter painter(this);
    painter.setPen(m_axPen);
    painter.setRenderHint(QPainter::Antialiasing, true);
    drawAxis(&painter);

    painter.setPen(m_curvePen);
    drawBasisFunCurve(&painter);
}

void BasisFunWidget::drawAxis(QPainter *painter)
{
    // x axis
    painter->drawLine(
                QPoint(zero_x, zero_y),
                QPoint(zero_x + axis_length_x, zero_y));
    //y axis
    painter->drawLine(
                QPoint(zero_x, zero_y),
                QPoint(zero_x, zero_y - axis_length_y));
}

void BasisFunWidget::drawBasisFunCurve(QPainter *painter)
{
    int length = 1.0 / interval;
    vector<vector<double>> curves = basisFun.evaluate(&m_interp);
    for(int i = 0; i < curves.size(); i++)
    {
        QPainterPath path;
        bool moved {false};     //控制path移动到初始点 再开始连线
        for(int j = 0; j< curves[i].size(); j++)
        {
            double x = zero_x + m_interp[j] * axis_length_x;
            double y = zero_y - curves[i][j] * axis_length_y;
            if(curves[i][j] > 0.001)
            {
                //忽略函数值为0的部分
                if(moved)
                {
                    path.lineTo(QPoint(x, y));
                }
                else
                {
                    moved = true;
                    path.moveTo(QPoint(x, y));
                }
            }
        }
        painter->drawPath(path);
    }
}
