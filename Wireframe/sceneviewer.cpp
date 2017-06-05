#include "sceneviewer.h"

#include <QColor>
#include <QMouseEvent>
#include <QPainter>

#include "cuboid.h"
#include "transformmatrix.h"
#include "utils.h"

SceneViewer::SceneViewer(QWidget *parent) :
    QWidget(parent),
    transformScene(TransformMatrix::identity4()),
    transformCamera(TransformMatrix::shift(0, 0, 1000)),
    canvas(size(), QImage::Format_RGB32),
    background(Qt::black),
    zNear(100),
    zFar(10000),
    wScale(1),
    hScale(1)
{}

void SceneViewer::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    if (objects.size() == 0){
        painter.drawText(rect(), Qt::AlignCenter ,"Add objects to show");
        return;
    }

    Utils::fillRectangle(background, canvas, {0,0, width(), height()});

    double scale = findMaxLen();

    auto sceneTransform = TransformMatrix::perspective(zNear) * transformCamera * transformScene * TransformMatrix::scale(1/scale, 1/scale, 1/scale);
    cameraSize = qMin(width()/wScale, height()/hScale);

    drawAspectLines();

    for (Object3D *obj : objects){
        drawObject(obj, sceneTransform);
    }
    Cuboid c(scale * 2, Qt::red, objects.size() > 1);
    drawObject(&c, sceneTransform);

    painter.drawImage(0, 0, canvas);
}

void SceneViewer::resizeEvent(QResizeEvent *)
{
    if (size().width() > canvas.width() ||
        size().height() > canvas.height()){
        canvas = QImage(size(), QImage::Format_RGB32);
    }
}

void SceneViewer::mouseMoveEvent(QMouseEvent *e)
{
    QPoint delta = e->pos() - lastPos;
    lastPos = e->pos();

    static const double mouseSpeed = 0.003;

    transformScene = TransformMatrix::rotationMatrixY4(-delta.x()*mouseSpeed) * transformScene;
    transformScene = TransformMatrix::rotationMatrixX4(-delta.y()*mouseSpeed) * transformScene;

    update();
}

void SceneViewer::mousePressEvent(QMouseEvent *e)
{
    lastPos = e->pos();
}

void SceneViewer::drawObject(Object3D *object, const QGenericMatrix<4, 4, double> &transform)
{
    QPoint center(width()/2, height()/2);

    auto objTransform = transform * object->getTransform();

    for (Segment segment : object->getSegments()){
        auto p1Transformed = objTransform * segment.getP1().toMatrix4();
        auto p2Transformed = objTransform * segment.getP2().toMatrix4();

        auto p1V = Point3D(p1Transformed.constData(), true).toVector3D();
        auto p2V = Point3D(p2Transformed.constData(), true).toVector3D();

        if (!Utils::clipLineInCube(p1V, p2V,
                                   -wScale/2, wScale/2,
                                   -hScale/2, hScale/2,
                                   zNear, zFar)
           )
            continue;

        QPointF p1 = QPointF(p1V.x() * cameraSize, -p1V.y() * cameraSize);
        QPointF p2 = QPointF(p2V.x() * cameraSize, -p2V.y() * cameraSize);

        Utils::drawLine(
            (p1 + center).toPoint(),
            (p2 + center).toPoint(),
            segment.getColor().rgb(),
            canvas
        );
    }
}

void SceneViewer::drawAspectLines()
{
    int w = width() - cameraSize * wScale;
    int h = height() - cameraSize * hScale;

    QColor bg = QWidget::palette().color(QWidget::backgroundRole());

    Utils::fillRectangle(bg, canvas, {0, 0, width(), h/2});
    Utils::fillRectangle(bg, canvas, {0, height() - h/2, width(), h/2});
    Utils::fillRectangle(bg, canvas, {0, 0, w/2, height()});
    Utils::fillRectangle(bg, canvas, {width() - w/2, 0, w/2, height()});
}

double SceneViewer::findMaxLen()
{
    double max = 0.0;
    for (Object3D *obj : objects){
        auto transform = obj->getTransform();

        for (const Segment &segment : obj->getSegments()){
            auto p1 = Point3D((transform * segment.getP1().toMatrix4()).constData());
            auto p2 = Point3D((transform * segment.getP2().toMatrix4()).constData());

            double arr[] = {
                p1.getX(),
                p1.getY(),
                p1.getZ(),
                p2.getX(),
                p2.getY(),
                p2.getZ()
            };

            for (auto &p : arr)
                max = qMax(max, qAbs(p));
        }
    }
    return max;
}

QColor SceneViewer::getBackground() const
{
    return background;
}

void SceneViewer::setBackground(const QColor &value)
{
    background = value;
    update();
}

QGenericMatrix<4, 4, double> SceneViewer::getTransformScene() const
{
    return transformScene;
}

void SceneViewer::setTransformScene(const QGenericMatrix<4, 4, double> &value)
{
    transformScene = value;
    update();
}

double SceneViewer::getHScale() const
{
    return hScale;
}

double SceneViewer::getWScale() const
{
    return wScale;
}

double SceneViewer::getZFar() const
{
    return zFar;
}

double SceneViewer::getZNear() const
{
    return zNear;
}

void SceneViewer::setHScale(double val)
{
    hScale = val;
    update();
}

void SceneViewer::resetScene()
{
    transformScene = TransformMatrix::identity4();
    update();
}

void SceneViewer::setWScale(double val)
{
    wScale = val;
    update();
}

void SceneViewer::setObjects(const std::vector<Object3D *> &value)
{
    objects = value;
    update();
}

int SceneViewer::heightForWidth(int w) const
{
    return (int)(hScale / wScale * w);
}

double SceneViewer::getZ() const
{
    return -transformCamera(2, 3);
}

void SceneViewer::setZ(double val)
{
    transformCamera = TransformMatrix::shift(0, 0, -val);
    update();
}

void SceneViewer::setZNear(double val)
{
    zNear = val;
    update();
}

void SceneViewer::setZFar(double val)
{
    zFar = val;
    update();
}
