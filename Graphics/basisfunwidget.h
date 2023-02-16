#ifndef BASISFUNWIDGET_H
#define BASISFUNWIDGET_H

#include <QWidget>
#include <QList>
#include <QPainter>
#include "basis.h"

class BasisFunWidget : public QWidget
{
    Q_OBJECT
public:
    BasisFunction basisFun;
public:
    explicit BasisFunWidget(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
public slots:
    void on_defineKnotsVec(KnotsVector& knots);
    void on_timeout();

private:
    QPen m_axPen{QBrush(Qt::darkBlue), 3};
    QPen m_curvePen{QBrush(Qt::black), 1};
    QPen m_knotPen{QBrush(Qt::darkRed), 2};

    //用于绘制坐标轴
    int zero_y {180};
    int zero_x {100};
    int one_x {900};
    int axis_length_x {800};
    int axis_length_y {170};

    //将坐标系上的坐标转换为窗口中的位置，用于绘制
    double transformX(double x);
    double transformY(double y);

    vector<double> m_interp;  //基函数的取值

    void drawAxis(QPainter *painter);
    void drawBasisFunCurve(QPainter *painter);
    void drawKnots(QPainter *painter);
};

#endif // BASISFUNWIDGET_H
