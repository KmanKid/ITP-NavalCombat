#ifndef MYLABEL_H
#define MYLABEL_H

#import <QMouseEvent>
#import <QLabel>

class MyLabel
{
public:
    MyLabel();
public signals:
    void clicked();
public slots:
    void slotClicked();
private:
    void mousePressEvent ( QMouseEvent * event );
};

#endif // MYLABEL_H
