#ifndef FILTER
#define FILTER

#include <QIcon>
#include <QKeySequence>
#include <QScopedPointer>
#include <QString>
#include <QWidget>

#include <functional>

class Filter : public QObject
{
    Q_OBJECT

signals:
    void progressChanged(float progress);
    void settingsChanged();

public:

    Filter(QObject *parent = 0);

    virtual QIcon getIcon();

    virtual QKeySequence getShortcut();

    virtual int getCategory();

    virtual QString getText();

    virtual QString getDescription();

    virtual QWidget* getSettingsWidget();

    virtual void doFilter(const QImage *input, QImage *output) = 0;

protected:
    void perPixelLoop(const QImage *input, QImage *output, std::function<QColor(const QImage*, int x, int y)> proc,
                      float percentFrom = 0, float percentTo = 1);

private:
    QScopedPointer<QWidget> settingsWidget;


};


#endif // FILTER
