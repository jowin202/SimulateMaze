#include "cell.h"

Cell::Cell(int x, int y, int n, int r)
    : QObject{}
{
    this->x = x;
    this->y = y;
    this->n = n;
    this->r = r;
}
