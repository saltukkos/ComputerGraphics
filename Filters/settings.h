#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialogButtonBox>
#include <QLabel>
#include <QScrollArea>
#include <QWidget>

class Settings : public QWidget
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = 0);
    ~Settings();

    void setParametersWidget(QWidget *w);
    void setFilterName(QString n);
signals:
    void buttonPressed(int code);

public slots:
    void setButtonsEnable(bool enable);

private:
    QLabel *name;
    QScrollArea *area;
    QDialogButtonBox *buttons;
};

#endif // SETTINGS_H
