#ifndef SURFACEWINDOW_H
#define SURFACEWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include "bspline.h"
#include <Q3DSurface>

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


    QWidget *container;
    Q3DSurface *graphSurf;
    QSurface3DSeries *serieCtrPoints;
    QSurface3DSeries *serieSurf;

    vector<vector<QVector3D>> ctrPoints;

};

#endif // SURFACEWINDOW_H
