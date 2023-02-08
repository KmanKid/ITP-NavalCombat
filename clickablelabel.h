#ifndef QCLICKABLELABEL_H
#define QCLICKABLELABEL_H

#include <Qt>
#include <QWidget>
#include <QLabel>

class ClickableLabel : public QLabel
{
    Q_OBJECT
public:
    ClickableLabel(QWidget * parent = 0 );
    ~ClickableLabel(){}

signals:
    void clicked();

public slots:
    void slotClicked();

protected:
    void mousePressEvent ( QMouseEvent * event ) ;

};
#endif
