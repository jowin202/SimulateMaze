#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QRandomGenerator>
#include <QList>
#include <QDateTime>

class Cell;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void simulate();
    void recursive_find(QPainter *painter, QPoint point);

    bool always_up(QPoint p)
    {
        if (p.x() == 0 && p.y()%2 ==1 ) return true;
        if (p.x() == n-1 && p.y()%2 == 0 && p.y() > 0 ) return true;
        return false;
    }
    bool always_down(QPoint p)
    {
        if (p.x() == 0 && p.y()%2 ==0 ) return true;
        if (p.x() == n-1 && p.y()%2 == 1 ) return true;
        return false;
    }
    bool always_right(QPoint p)
    {
        if (p.y() == 0 && p.x()%2 ==0 ) return true;
        if (p.y() == n-1 && p.x()%2 == 1 ) return true;
        return false;
    }
    bool always_left(QPoint p)
    {
        if (p.y() == 0 && p.x()%2 ==1 ) return true;
        if (p.y() == n-1 && p.x()%2 == 0 && p.x() > 0 ) return true;
        return false;
    }

private slots:
    void on_button_simulate_clicked();

private:
    Ui::MainWindow *ui;
    int d = 70;
    int n = 5;
    QList< QList<Cell*> *> list;
    QList<QPoint> connected_points;

    int num = 0;
};
#endif // MAINWINDOW_H
