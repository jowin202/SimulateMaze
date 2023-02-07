#ifndef CELL_H
#define CELL_H

#include <QObject>
#include <QDebug>

class Cell : public QObject
{
    Q_OBJECT
public:
    explicit Cell(int x, int y, int n, int r);

    bool open_up()
    {
        //top
        if (y==0)
            return false;

        //left
        if (x==0 && y%2==0 && y > 1)
            return true;
        else if (x==0)
            return false;

        //right
        if (x==n-1 && y%2==1)
            return true;
        else if (x==n-1)
            return false;

        //middle
        if (x%2 == 0)
            return true;

        return false;
    }
    bool open_down(){
        //bottom
        if (y==n-1)
            return false;

        //left
        if (x==0)
            return false;

        //right
        if (x==n-1)
            return false;

        //middle
        if (x%2 == 1)
            return true;

        return false;
    }
    bool open_left()
    {
        if (x==0)
            return false;


        if (y%2==1)
            return true;

        return false;
    }
    bool open_right()
    {
        if (x==n-1)
            return false;

        if (y==0 && x>0 && x%2==1)
            return true;
        else if (y==0)
            return false;


        if (y==n-1 && x%2==0)
            return true;
        else if (y==n-1)
            return false;

        if (y%2==0)
            return true;

        return false;
    }

    bool rand_up()
    {
        return this->open_up() && (r==0);
    }
    bool rand_down()
    {
        return this->open_down() && (r==0);
    }
    bool rand_left()
    {
        return this->open_left() && (r==1);
    }
    bool rand_right()
    {
        return this->open_right() && (r==1);
    }

signals:

private:
    int x = 0;
    int y = 0;
    int n = 9;
    int r = 0;

};

#endif // CELL_H
