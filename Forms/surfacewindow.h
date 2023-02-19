#ifndef SURFACEWINDOW_H
#define SURFACEWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include "bspline.h"
#include <Q3DSurface>
#include <Q3DScatter>

namespace Ui {
class SurfaceWindow;
}

class SurfaceWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SurfaceWindow(QWidget *parent = nullptr);
    ~SurfaceWindow();

private slots:


    void on_pbtReadFile_clicked();

    void on_pbtClear_clicked();

private:
    Ui::SurfaceWindow *ui;


    QList<QList<QPointF>> ctrPoints;
    QWidget *container;
    Q3DSurface *graphSurf;
    QSurface3DSeries *serieSurf;

};

#endif // SURFACEWINDOW_H
