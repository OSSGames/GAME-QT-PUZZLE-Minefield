/*

 Name: Minefield
 Autor: Andreas Konarski
 Plattform: Alle Systeme, auf denen QT 4 verfuegbar ist. Kompatibel mit QT 5.0.0.
 Lizenz: GPL v3 or later

 Kontakt: programmieren@konarski-wuppertal.de
 home: www.konarski-wuppertal.de

 Falls ich mit diesem Programm die Rechte von irgend jemand verletzen sollte, bitte ich um einen Hinweis. Wenn dies Tatsaechlich der Fall ist, entferne ich es schnellstmoeglich von meiner Homepage.

 */

#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>
#include <QSize>
#include <QMap>
#include <QString>
#include <QList>

class Minenfeld;
class Mienenzaehler;
class Viewer;

class Scene : public QGraphicsScene
{
    Q_OBJECT

public:
    Scene(Viewer *parent = 0);
    virtual ~Scene();

    int breite() const;
    int hoehe() const;
    int bomben() const;

public slots:
    void neues_spiel(int, int);
    void neues_spiel(int, int, int);
    void sperre_felder();
    void sprengung_erfolgt();

signals:
    void freigabe(bool);
    void verbleibende_mienen(int);
    void gewonnen();
    void sprengung();
    void mienenwarnung();
    void aufgedeckt();
    void beflaggt();

private slots:
    void erzeuge_steine();
    void positioniere_steine();
    void verknuepfe_nachbarn();
    void streue_mienen_ein();
    void loesche_spiel();

private:
    int groessenmassstab, Breite, Hoehe, mienenzahl;
    QSize zuletzt_angenommene_groesse;
    QMap<QString, Minenfeld*> minenliste;
    QList <Minenfeld*> scharfe, gesamtliste;
    bool gesprengt;
    Mienenzaehler *mienenzaehler;
    Viewer *viewer;
};

#endif
