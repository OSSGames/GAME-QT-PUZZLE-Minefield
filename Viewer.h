/*

 Name: Minefield
 Autor: Andreas Konarski
 Plattform: Alle Systeme, auf denen QT 4 verfuegbar ist. Kompatibel mit QT 5.0.0.
 Lizenz: GPL v3 or later

 Kontakt: programmieren@konarski-wuppertal.de
 home: www.konarski-wuppertal.de

 Falls ich mit diesem Programm die Rechte von irgend jemand verletzen sollte, bitte ich um einen Hinweis. Wenn dies Tatsaechlich der Fall ist, entferne ich es schnellstmoeglich von meiner Homepage.

 */

#ifndef VIEWER_H
#define VIEWER_H

#include <QGraphicsView>
#include <QSize>

class Viewer : public QGraphicsView
{
    Q_OBJECT

public:
    Viewer(QWidget *parent = 0);
    Viewer(QGraphicsScene *scene, QWidget *parent = 0);
    virtual ~Viewer();
};

#endif
