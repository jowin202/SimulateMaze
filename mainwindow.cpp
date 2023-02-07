#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cell.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->simulate();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::simulate()
{
    this->connected_points.clear();

    //delete old
    for (int i = 0; i < list.length(); i++)
    {
        for (int j = 0; j < list.at(i)->length(); j++)
            delete list.at(i)->at(j);
        delete list.at(i);
    }
    this->list.clear();

    //rand generate new
    QRandomGenerator rand(QDateTime::currentMSecsSinceEpoch());
    for (int i = 0; i < n; i++)
    {
        QList<Cell*> *row = new QList<Cell*>();
        for (int j = 0; j < n; j++)
        {
            row->append(new Cell(i,j,n,rand.generate()&1));
        }
        list.append(row);
    }

    //init image
    QImage img(n*d,n*d, QImage::Format_RGB16);
    QPainter painter(&img);
    QPen pen;
    pen.setWidth(3);
    painter.setPen(pen);
    painter.fillRect(0,0,img.width(),img.height(),Qt::white);
    painter.drawRect(0,0,img.width()-1,img.height()-1);
    int s = d/4;

    //draw grid
    for (int i = 0; i <= n; i++)
    {
        for (int j = 0; j <= n; j++)
        {
            painter.drawLine(d*i+s,d*j,d*i-s,d*j);
            painter.drawLine(d*i,d*j+s,d*i,d*j-s);
        }
    }


    //draw blocked ways in gray
    pen.setColor(Qt::gray);
    painter.setPen(pen);
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            //innen
            if (i > 0 && j > 0 && j < n-1 && (!list.at(i)->at(j)->rand_left() && !list.at(i-1)->at(j)->rand_right()))
            {
                painter.drawLine(d*i,d*j+s,d*i,d*(j+1)-s);
            }
            if (j > 0 && i > 0 && i < n-1 && (!list.at(i)->at(j)->rand_up() && !list.at(i)->at(j-1)->rand_down()))
            {
                painter.drawLine(d*i+s,d*j,d*(i+1)-s,d*j);
            }
            //aussen
            if (j != 0 && i == 0 && j%2 == 0 && !list.at(i)->at(j)->rand_up()) //links
                painter.drawLine(d*i+s,d*j,d*(i+1)-s,d*j);
            if (i != 0 && j == 0 && i%2 == 0 && !list.at(i-1)->at(j)->rand_right()) //oben
                painter.drawLine(d*i,d*j+s,d*i,d*(j+1)-s);
            if (j != 0 && i == n-1 && j%2 == 1 && !list.at(i)->at(j)->rand_up()) //rechts
                painter.drawLine(d*i+s,d*j,d*(i+1)-s,d*j);
            if (i != 0 && j == n-1 && i%2 == 1 && !list.at(i-1)->at(j)->rand_right()) //unten
                painter.drawLine(d*i,d*j+s,d*i,d*(j+1)-s);
        }
    }


    //draw arrows in red
    pen.setColor(Qt::red);
    painter.setPen(pen);
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            QPoint topleft(d*i,d*j);
            if (list.at(i)->at(j)->open_up() && list.at(i)->at(j)->rand_up())
            {
                painter.drawLine(topleft + QPoint(d/2,d/4),topleft + QPoint(d/2,0));
                painter.drawLine(topleft + QPoint(d/2,0), topleft+QPoint(d/4,d/4));
                painter.drawLine(topleft + QPoint(d/2,0), topleft+QPoint(3*d/4,d/4));
            }
            if (list.at(i)->at(j)->open_down() && list.at(i)->at(j)->rand_down())
            {
                painter.drawLine(topleft + QPoint(d/2,3*d/4),topleft + QPoint(d/2,d));
                painter.drawLine(topleft + QPoint(d/2,d), topleft+QPoint(d/4,3*d/4));
                painter.drawLine(topleft + QPoint(d/2,d), topleft+QPoint(3*d/4,3*d/4));
            }
            if (list.at(i)->at(j)->open_right() && list.at(i)->at(j)->rand_right())
            {
                painter.drawLine(topleft + QPoint(3*d/4,d/2),topleft + QPoint(d,d/2));
                painter.drawLine(topleft + QPoint(d,d/2), topleft+QPoint(3*d/4+5,  d/4));
                painter.drawLine(topleft + QPoint(d,d/2), topleft+QPoint(3*d/4+5,3*d/4));
            }
            if (list.at(i)->at(j)->open_left() && list.at(i)->at(j)->rand_left())
            {
                painter.drawLine(topleft + QPoint(d/4,d/2),topleft + QPoint(0,d/2));
                painter.drawLine(topleft + QPoint(0,d/2), topleft+QPoint(d/4-5,  d/4));
                painter.drawLine(topleft + QPoint(0,d/2), topleft+QPoint(d/4-5,3*d/4));
            }

        }
    }


    //recursive line in blue
    pen.setColor(Qt::blue);
    pen.setWidth(5);
    painter.setPen(pen);
    this->recursive_find(&painter,QPoint(0,n-1)); //start in bottom left

    painter.end();
    //img.save(QString("/tmp/sim/img%1.png").arg(++num,4,10,QChar('0')));
    this->ui->label_result->setPixmap(QPixmap::fromImage(img));
}

void MainWindow::recursive_find(QPainter *painter, QPoint point)
{
    //if point already found... stop
    if (this->connected_points.contains(point))
        return;

    //invalid point .. stop
    if (point.x() < 0 || point.y() < 0 || point.x() >= n || point.y() >= n)
        return;

    //append point ...
    this->connected_points.append(point);

    //go in all directions
    if ((list.at(point.x())->at(point.y())->open_up() && list.at(point.x())->at(point.y())->rand_up()) || this->always_up(point) )
    {
        painter->drawLine(point.x()*d + d/2, point.y()*d + d/2, point.x()*d +d/2, point.y()*d - d/2);
        this->recursive_find(painter, QPoint(point.x(), point.y()-1));
    }
    if ((list.at(point.x())->at(point.y())->open_down() && list.at(point.x())->at(point.y())->rand_down()) || this->always_down(point) )
    {
        painter->drawLine(point.x()*d + d/2, point.y()*d + d/2, point.x()*d +d/2, point.y()*d + 3*d/2);
        this->recursive_find(painter, QPoint(point.x(), point.y()+1));
    }
    if ((list.at(point.x())->at(point.y())->open_left() && list.at(point.x())->at(point.y())->rand_left()) || this->always_left(point) )
    {
        painter->drawLine(point.x()*d + d/2, point.y()*d + d/2, point.x()*d -d/2, point.y()*d + d/2);
        this->recursive_find(painter, QPoint(point.x()-1, point.y()));
    }
    if ((list.at(point.x())->at(point.y())->open_right() && list.at(point.x())->at(point.y())->rand_right()) || this->always_right(point) )
    {
        painter->drawLine(point.x()*d + d/2, point.y()*d + d/2, point.x()*d +3*d/2, point.y()*d + d/2);
        this->recursive_find(painter, QPoint(point.x()+1, point.y()));
    }


    //check if point is reachable from direction
    if (point.y() < n-1 && list.at(point.x())->at(point.y()+1)->open_up() && list.at(point.x())->at(point.y()+1)->rand_up())
        this->recursive_find(painter, QPoint(point.x(), point.y()+1));
    if (point.x() < n-1 && list.at(point.x()+1)->at(point.y())->open_left() && list.at(point.x()+1)->at(point.y())->rand_left() )
        this->recursive_find(painter, QPoint(point.x()+1, point.y()));
    if (point.y() > 0 && list.at(point.x())->at(point.y()-1)->open_down() && list.at(point.x())->at(point.y()-1)->rand_down())
        this->recursive_find(painter, QPoint(point.x(), point.y()-1));
    if (point.x() > 0 && list.at(point.x()-1)->at(point.y())->open_right() && list.at(point.x()-1)->at(point.y())->rand_right())
        this->recursive_find(painter, QPoint(point.x()-1, point.y()));
}


void MainWindow::on_button_simulate_clicked()
{
    this->simulate();
}

