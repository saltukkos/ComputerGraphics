#ifndef DRAGINPUT_H
#define DRAGINPUT_H

#include <QWidget>

#include <QPoint>
#include <QString>

class DragInput : public QWidget
{
    Q_OBJECT
public:
    explicit DragInput(QString text, QWidget *parent = 0, int gridSize = 10);

protected:
    void paintEvent(QPaintEvent *) override;

    void mouseMoveEvent(QMouseEvent *) override;

    void mousePressEvent(QMouseEvent *) override;

signals:
    void dragged(QPoint shift);

private:
    QPoint clickPos;

    QString text;
    int xShift;
    int yShift;
    int gridSize;

};

#endif // DRAGINPUT_H
