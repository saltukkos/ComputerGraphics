#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QWidget>

class ImageViewer : public QWidget
{
    Q_OBJECT
public:
    explicit ImageViewer(QString name, QWidget *parent = 0);
    virtual ~ImageViewer() = default;

    void paintEvent(QPaintEvent *) override;

    void setImage(const QImage &value);
    const QImage &getImage();

private:
    QString name;

protected:
    QImage image;
    float scaleFactor;
};

#endif // IMAGEVIEWER_H
