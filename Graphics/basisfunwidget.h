#ifndef BASISFUNWIDGET_H
#define BASISFUNWIDGET_H

#include <QWidget>
#include <QList>
#include <QPainter>
#include "NURBS/basisfunction.h"

class BasisFunWidget : public QWidget
{
    Q_OBJECT
public:
    BasisFunction basisFun{3, 5};
    explicit BasisFunWidget(QWidget *parent = nullptr);
protected:
    void paintEvent(QPaintEvent *event) override;
signals:

private:
    QPen m_axPen{QBrush(Qt::darkBlue), 3};
    QPen m_curvePen{QBrush(Qt::black), 1};

    //用于绘制坐标轴
    int zero_y {180};
    int zero_x {100};
    int one_x {900};
    int axis_length_x {800};
    int axis_length_y {170};

    vector<double> m_interp;  //基函数的取值

    void drawAxis(QPainter *painter);
    void drawBasisFunCurve(QPainter *painter);
};

#endif // BASISFUNWIDGET_H
