/*

 Name: Minefield
 Autor: Andreas Konarski
 Plattform: Alle Systeme, auf denen QT 4 verfuegbar ist. Kompatibel mit QT 5.0.0.
 Lizenz: GPL v3 or later

 Kontakt: programmieren@konarski-wuppertal.de
 home: www.konarski-wuppertal.de

 Falls ich mit diesem Programm die Rechte von irgend jemand verletzen sollte, bitte ich um einen Hinweis. Wenn dies Tatsaechlich der Fall ist, entferne ich es schnellstmoeglich von meiner Homepage.

 */

#ifndef MINENFELD_H
#define MINENFELD_H

#include <QGraphicsItem>
#include <QObject>
#include <QSize>
#include <QList>

#define GROESSE 40
#define SICHERHEITSRAND 2

class Mienenzaehler;

class Minenfeld : public QObject, public QGraphicsItem
{
    Q_OBJECT

public:
    Minenfeld(QObject *oparent = 0, QGraphicsItem *gparent = 0);
    virtual ~Minenfeld();

    bool ist_mine() const;
    void registriere_mienenzaehler(Mienenzaehler*);

public slots:
    void registriere_nachbarn(Minenfeld*);
    void setze_mine();
    void nachbar_ist_miene();
    void decke_auf(char);
    void decke_auf();
    void setze_flagge();
    void entferne_flagge();
    void setEnabled(bool);
    void reset();

signals:
    void bin_miene();
    void wurde_aufgedeckt(char);
    void sprengung();
    void mienenwarnung();
    void aufgedeckt();
    void beflaggt();

private:
    char Feldwert;
    bool Aufgedeckt, Beflaggt, Freigegeben;
    QList<Minenfeld*> Nachbarn;
    Mienenzaehler *mienenzaehler;

    virtual void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
    virtual QRectF boundingRect() const;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent*);
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent*);
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent*);
};

#endif
