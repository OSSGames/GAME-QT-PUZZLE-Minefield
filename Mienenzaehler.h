/*

 Name: Minefield
 Autor: Andreas Konarski
 Plattform: Alle Systeme, auf denen QT 4 verfuegbar ist. Kompatibel mit QT 5.0.0.
 Lizenz: GPL v3 or later

 Kontakt: programmieren@konarski-wuppertal.de
 home: www.konarski-wuppertal.de

 Falls ich mit diesem Programm die Rechte von irgend jemand verletzen sollte, bitte ich um einen Hinweis. Wenn dies Tatsaechlich der Fall ist, entferne ich es schnellstmoeglich von meiner Homepage.

 */

#ifndef MIENENZAEHLER_H
#define MIENENZAEHLER_H

#include <QObject>

class Mienenzaehler : public QObject
{
    Q_OBJECT

public:
    Mienenzaehler(QObject *parent = 0);
    virtual ~Mienenzaehler();

    int mienen() const;
    int felder() const;

public slots:
    void setze_mienenzahl(int);
    void fuege_miene_hinzu();
    void entferne_miene();
    void setze_feldzahl(int);
    void fuege_feld_hinzu();
    void entferne_feld();

signals:
    void neuer_mienenstand(int);
    void alle_felder_weg();

private:
    int Mienen, Felder;
};

#endif
