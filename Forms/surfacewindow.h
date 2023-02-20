#ifndef SURFACEWINDOW_H
#define SURFACEWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <Q3DSurface>
#include <Q3DScatter>
#include <vector>
using std::vector;

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

    void on_pbtDrawSurf_clicked();

private:
    Ui::SurfaceWindow *ui;


    QWidget *container;
    Q3DSurface *graphSurf;
    QSurface3DSeries *serieSurf;

//    Q3DScatter *graphScatter;
//    QScatter3DSeries *seriesScatter;
    vector<vector<QVector3D>> ctrPoints;

};

#endif // SURFACEWINDOW_H
