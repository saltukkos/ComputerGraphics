#include "loadsaver.h"

#include <QFile>
#include <QTextStream>

#define NEXTLINE(X) do{ \
                        char temp = 0;\
                        while (temp != '\n'){\
                            (X) >> temp;\
                            if ((X).status() != QTextStream::Ok)\
                                return false;\
                        }\
                    } while(0)

static bool readMatrix(QTextStream &stream, double *res){
    for (int i = 0; i < 3; ++i){
        double d1, d2, d3;
        stream >> d1 >> d2 >> d3;
        NEXTLINE(stream);

        res[i*4    ] = d1;
        res[i*4 + 1] = d2;
        res[i*4 + 2] = d3;
    }
    return true;
}

static bool readColor(QTextStream &stream, QColor &c){
    int r, g, b;
    stream >> r >> g >> b;
    NEXTLINE(stream);

    c = QColor(r, g, b);
    return true;
}

static void writeMatrix(QTextStream &stream, const double *mat){
    for (int i = 0; i < 3; ++i){
        for (int j = 0; j < 3; ++j){
            stream << mat[j*4 + i] << ' ';
        }
        stream << endl;
    }
}

static void writeColor(QTextStream &stream, QColor c){
    stream << c.red() << ' ' << c.green() << ' ' << c.blue() << endl;
}


bool LoadSaver::loadFromFile(const QString &filename, std::vector<std::unique_ptr<SurfaceOfRevolution>> &surfaces, SceneViewer &scene)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly))
        return false;


    QTextStream stream(&file);

    double n,m,k1,a,b,c1,d;
    stream >> n >> m >> k1 >> a >> b >> c1 >> d;
    NEXTLINE(stream);

    double z, zn, zf, sw, sh;
    stream >> z >> zn >> zf >> sw >> sh;
    NEXTLINE(stream);

    double arr[16] = {0};
    arr[15] = 1.;
    if (!readMatrix(stream, arr))
        return false;

    auto sceneTransform = QGenericMatrix<4, 4, double>(arr);
    QColor bg;
    if (!readColor(stream, bg))
        return false;

    int k = 0;
    stream >> k;
    NEXTLINE(stream);

    for (int i = 0; i < k; ++i){
        SurfaceOfRevolution *surface = new SurfaceOfRevolution;
        surfaces.push_back(std::move(std::unique_ptr<SurfaceOfRevolution>(surface)));

        QColor c;
        if (!readColor(stream, c))
            return false;

        surface->setColor(c.rgb());

        surface->setN(n);
        surface->setK(k1);
        surface->setAngleStart(c1);
        surface->setAngleEnd(d);

        double x, y, z;
        stream >> x >> y >> z;
        NEXTLINE(stream);

        surface->setX(x);
        surface->setY(y);
        surface->setZ(z);

        if (!readMatrix(stream, arr))
            return false;

        surface->setRotationMatrix(QGenericMatrix<4, 4, double>(arr));

        int nPoints = 0;
        stream >> nPoints;
        NEXTLINE(stream);

        for (int j = 0; j < nPoints; ++j){
            double x, y;
            stream >> x >> y;
            if (j != nPoints - 1)
                NEXTLINE(stream);
            surface->getSpline().push_back({x, y});
        }
        surface->generateRevolution();
    }

    if (stream.status() != QTextStream::Ok)
        return false;

    scene.setZ(z);
    scene.setZNear(zn);
    scene.setZFar(zf);

    scene.setBackground(bg);

    scene.setTransformScene(sceneTransform);

    return true;
}

bool LoadSaver::saveToFile(const QString &filename, const std::vector<std::unique_ptr<SurfaceOfRevolution>> &surfaces, const SceneViewer &scene)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadWrite))
        return false;

    QTextStream stream(&file);

    if (surfaces.size() > 0){
        stream << surfaces[0]->getN() << " 51 " << surfaces[0]->getK() << " 0 1 " << surfaces[0]->getAngleStart() << ' ' << surfaces[0]->getAngleEnd() << endl;
    } else {
        stream << "10 10 10 0 1 0 1" << endl;
    }

    stream << scene.getZ() << ' ' << scene.getZNear() << ' ' << scene.getZFar() << ' ' << scene.getWScale() << ' ' << scene.getHScale() << endl;

    writeMatrix(stream, scene.getTransformScene().constData());

    writeColor(stream, scene.getBackground());

    stream << surfaces.size() << endl;

    for (uint i = 0; i < surfaces.size(); ++i){
        SurfaceOfRevolution *surface = surfaces[i].get();
        writeColor(stream, QColor(surface->getColor()));

//        stream << surface->getN() << ' ' << surface->getK() << endl;
        stream << surface->getX() << ' ' << surface->getY() << ' ' << surface->getZ() << endl;

        writeMatrix(stream, surface->getRotationMatrix().constData());
        const auto &points = surface->getSpline();

        stream << points.size() << endl;
        for (uint j = 0; j < points.size(); ++j)
            stream << points[j].x() << ' ' << points[j].y() << endl;

    }

    return stream.status() == QTextStream::Ok;
}
