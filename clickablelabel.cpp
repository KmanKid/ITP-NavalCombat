#include "clickablelabel.h"
#include <QDebug>

ClickableLabel::ClickableLabel(QWidget * parent )
:QLabel(parent)
{
    connect( this, SIGNAL( clicked() ), this, SLOT( slotClicked() ) );
}

void ClickableLabel::slotClicked()
{
    qDebug()<<"Clicked";
}

void ClickableLabel::mousePressEvent ( QMouseEvent * event )
{
    emit clicked();
}
