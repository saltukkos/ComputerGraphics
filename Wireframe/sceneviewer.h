#ifndef SCENEVIEWER_H
#define SCENEVIEWER_H

#include <QWidget>

#include <QImage>
#include <QGenericMatrix>

#include <vector>

#include "object3d.h"

class SceneViewer : public QWidget
{
    Q_OBJECT

public:
    explicit SceneViewer(QWidget *parent = 0);

    void setObjects(const std::vector<Object3D*> &value);

    int heightForWidth(int) const override; //doesn't work :(

    double getZ() const;

    double getZNear() const;

    double getZFar() const;

    double getWScale() const;

    double getHScale() const;

    QGenericMatrix<4, 4, double> getTransformScene() const;
    void setTransformScene(const QGenericMatrix<4, 4, double> &value);

    QColor getBackground() const;
    void setBackground(const QColor &value);

public slots:
    void setZ(double val);

    void setZNear(double val);

    void setZFar(double val);

    void setWScale(double val);

    void setHScale(double val);

    void resetScene();

protected:
    void paintEvent(QPaintEvent *) override;

    void resizeEvent(QResizeEvent *) override;

    void mouseMoveEvent(QMouseEvent *) override;

    void mousePressEvent(QMouseEvent *) override;

private:
    void drawObject(Object3D *object, const QGenericMatrix<4, 4, double> &transform);

    void drawAspectLines();

    double findMaxLen();

private:
    QGenericMatrix<4, 4, double> transformScene;

    QGenericMatrix<4, 4, double> transformCamera;

    QPoint lastPos;

    std::vector<Object3D*> objects;

    QImage canvas;
    QColor background;

    int cameraSize;

    double zNear;
    double zFar;

    double wScale;
    double hScale;
};

#endif // SCENEVIEWER_H
