#include "gridcell.h"

GridCell::GridCell( QWidget* parent,int x,int y)
    : QPushButton(parent)
{   //Initialisieren der Attribute
    this->x = x;
    this->y = y;
    this->state = 0;
}

GridCell::~GridCell()
{

}
