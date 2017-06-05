#include "about.h"

#include <QGridLayout>
#include <QtWidgets>

About::About(QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle("About");
    QGridLayout *mainLayout = new QGridLayout(this);
    QLabel *photo = new QLabel(this);
    photo->setPixmap(QPixmap(":/images/saltuk.jpg").scaledToHeight(180));

    QLabel *description = new QLabel("Control: \n\
    drag mouse with pressed right button to move \n\
    wheel(with shift/ctrl) for scroll(horisontal scroll/scale)\n\n\
Created by Saltuk Konstantin\n\
NSU, 2017 ", this);
    description->setWordWrap(true);

    QDialogButtonBox *buttons = new QDialogButtonBox(this);
    buttons->setStandardButtons(QDialogButtonBox::Ok);
    QObject::connect(buttons, SIGNAL(accepted()), this, SLOT(accept()));

    mainLayout->setContentsMargins(15, 15, 15, 15);
    mainLayout->setSpacing(20);
    mainLayout->addWidget(photo, 0, 0, 2, 1);
    mainLayout->addWidget(description, 0, 1);
    mainLayout->addWidget(buttons, 1, 1);
}
