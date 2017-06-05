#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QColor>
#include <QPointF>
#include <QSize>

class Settings : public QDialog
{
    Q_OBJECT
public:
    explicit Settings(QPointF *functionFieldFrom, QPointF *functionFieldTo, QSize *gridSize, QColor *lineColor, QWidget *parent = 0);
};

#endif // SETTINGS_H
