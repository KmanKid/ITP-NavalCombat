#ifndef GRIDCELL_H
#define GRIDCELL_H
#include <QPushButton>

class GridCell : public QPushButton
{
    Q_OBJECT
public:
    GridCell(QWidget* parent = 0, int x = 0, int y = 0);
    virtual ~GridCell();
    int x;
    int y;
};

#endif // GRIDCELL_H
