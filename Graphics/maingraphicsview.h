#ifndef MAINGRAPHICSVIEW_H
#define MAINGRAPHICSVIEW_H

#include <QObject>
#include <QGraphicsView>

class MainGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit MainGraphicsView(QWidget *parent = nullptr);
protected:

signals:

private:

};

#endif // MAINGRAPHICSVIEW_H
