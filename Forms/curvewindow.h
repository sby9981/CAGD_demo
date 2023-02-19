#ifndef CURVEWINDOW_H
#define CURVEWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include "maincurvescene.h"

QT_BEGIN_NAMESPACE
namespace Ui { class CurveWindow; }
QT_END_NAMESPACE

class CurveWindow : public QMainWindow
{
    Q_OBJECT

public:
    CurveWindow(QWidget *parent = nullptr);
    ~CurveWindow();
private slots:
    void on_mouseMove(QPointF point);
    void on_selectItem(QPointF point);

    void on_btnAddPoint_clicked();

    void on_btnChangePoint_clicked();

    void on_btnDefineEdit_clicked();

    void on_btnClear_clicked();

    void on_spbDegree_valueChanged(int arg1);

    void on_cboCurveType_activated(int index);

    void on_pushButton_clicked();

private:
    Ui::CurveWindow *ui;
    QLabel *labViewCoordinate;
    QLabel *labSceneCoordinate;
    QLabel *labItemCoordinate;

    MainCurveScene* scene;

    QTimer* timer;
};
#endif // CURVEWINDOW_H
