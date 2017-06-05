#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QPoint>
#include <QTimer>
#include <vector>

struct Cell{
    int impact;
    bool state;
};

struct ModelSettings{
    int width;
    int height;
    int timerPeriod;
    int firstImpact;
    int secondImpact;
    int liveBegin;
    int liveEnd;
    int birthBegin;
    int birthEnd;
};

class Model : public QObject
{
        Q_OBJECT
public:
    Model(const ModelSettings &settings);

    void setSettings(const ModelSettings &settings);

    int getWidth();
    int getHeight();

    Cell getCellState(int x, int y);
    void setCellState(int x, int y, bool state);

    bool getSaved() const;
    void setSaved(bool value);

signals:
    void modelUpdated();
    void widthChanged(int bottom, int width);
    void heightChanged(int bottom, int height);

public slots:
    void updateState();
    void clearField();
    void turnTimer(bool);

private:
    ModelSettings settings;
    std::vector<Cell> fields[2];

    int activeField;
    QTimer timer;
    bool saved;

    float countImpact(const QPoint &point);
    void addImpact(const QPoint &point, int field, bool deleting = false);
    bool isCellAlive(int x, int y);
    void recalculateImpacts();
};

#endif // MODEL_H
