#ifndef GRIDCELL_H
#define GRIDCELL_H
#include <QPushButton>
#include <QTcpServer>

class GridCell : public QPushButton
{
    Q_OBJECT
public:
    GridCell(QWidget* parent = 0, int x = 0, int y = 0);
    virtual ~GridCell();
    int x;
    int y;
    int state;
    //property for field status
};

#endif // GRIDCELL_H
