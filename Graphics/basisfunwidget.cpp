#include "basisfunwidget.h"
#include <QPainterPath>
#include <numeric>

constexpr double interval {0.01};

BasisFunWidget::BasisFunWidget(QWidget *parent)
    : QWidget{parent}
{
    this->setAttribute(Qt::WA_StyledBackground, true);

    for(double u = 0.; u < 1.0; u += interval)
    {
        m_interp.push_back(u);
    }
}

void BasisFunWidget::paintEvent(QPaintEvent *event)
{
    //创建画家对象
    //参数为绘图设备, this表示在当前窗口绘制
    QPainter painter(this);
    painter.setPen(m_axPen);
    painter.setRenderHint(QPainter::Antialiasing, true);
    drawAxis(&painter);

    if(basisFun.isDrawEnable())
    {
        painter.setPen(m_curvePen);
        drawBasisFunCurve(&painter);

        painter.setPen(m_knotPen);
        drawKnots(&painter);
    }
}

void BasisFunWidget::on_defineKnotsVec(KnotsVector &knots)
{
    basisFun.reset(knots);
}

void BasisFunWidget::on_timeout()
{
    update();
}

double BasisFunWidget::transformX(double x)
{
    return zero_x + x * axis_length_x;
}

double BasisFunWidget::transformY(double y)
{
    return zero_y - y * axis_length_y;
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
            double x = transformX(m_interp[j]);
            double y = transformY(curves[i][j]);
            if(curves[i][j] > MIN_POSITIVE_NUM)
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

void BasisFunWidget::drawKnots(QPainter *painter)
{
    double half_height {10.0};
    struct counter{
        double knot{-1};
        int num{0};
        void reset(double newKnot)
        {
            knot = newKnot;
            num = 1;
        }
    }count;

    for(auto knot : basisFun.knots.vec())
    {
        if(knot - count.knot < MIN_POSITIVE_NUM)
        {
            count.num++;
        }
        else
        {
            if(count.num > 0)
            {
                double x {transformX(count.knot)};
                painter->drawLine(QPointF(x, zero_y-half_height),
                                  QPointF(x, zero_y+half_height));
                painter->drawText(x, zero_y + 2*half_height,
                                  QString::number(count.knot));
                painter->drawText(x, zero_y + 4*half_height,
                                  QString::number(count.num));
            }
            count.reset(knot);
        }
    }
    double x {transformX(count.knot)};
    painter->drawLine(QPointF(x, zero_y-half_height),
                      QPointF(x, zero_y+half_height));
    painter->drawText(x, zero_y + 2*half_height,
                      QString::number(count.knot));
    painter->drawText(x, zero_y + 4*half_height,
                      QString::number(count.num));
}
