#ifndef CURVEWINDOW_H
#define CURVEWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include "maingraphicsscene.h"

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

private:
    Ui::CurveWindow *ui;
    QLabel *labViewCoordinate;
    QLabel *labSceneCoordinate;
    QLabel *labItemCoordinate;

    MainGraphicsScene* scene;
};
#endif // CURVEWINDOW_H
