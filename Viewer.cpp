#include "Viewer.h"
#include <QResizeEvent>
#include <QScrollBar>
#include "Scene.h"

using namespace std;

Viewer::Viewer(QWidget *parent) : QGraphicsView(parent)
{
    setBackgroundBrush(Qt::gray);
}


Viewer::Viewer(QGraphicsScene *scene, QWidget *parent) : QGraphicsView(scene, parent)
{
    setBackgroundBrush(Qt::gray);
}


Viewer::~Viewer()
{
}
