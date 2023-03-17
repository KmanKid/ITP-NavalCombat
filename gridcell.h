#ifndef GRIDCELL_H
#define GRIDCELL_H
#include <QPushButton>
#include <QTcpServer>

// Die Klasse erweitert den Pushbutton um eine x und y coordinate und einen state
class GridCell : public QPushButton
{
    Q_OBJECT
public:
    GridCell(QWidget* parent = 0, int x = 0, int y = 0);
    virtual ~GridCell();
    int x;
    int y;
    int state;
};

#endif // GRIDCELL_H
