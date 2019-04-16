#include "Scene.h"
#include "Minenfeld.h"
#include "Random.h"
#include "Mienenzaehler.h"
#include "Viewer.h"
#include <QStringList>
#include <QDateTime>

#define SPLITTER ","

using namespace std;

Scene::Scene(Viewer *parent) : QGraphicsScene(parent), Breite(0), Hoehe(0), mienenzahl(0), gesprengt(false), viewer(parent)
{
    // mienenzaehler erzeugen
    mienenzaehler = new Mienenzaehler(this);
    mienenzaehler->setObjectName("mienenzaehler");

    // den zufallssimulator initialisieren
    Random::initialisiere();

    // signal - slot verbindungen
    // das durchreichen der verbleibenden mienen ermoeglichen
    connect(mienenzaehler, SIGNAL(neuer_mienenstand(int)), this, SIGNAL(verbleibende_mienen(int)));

    // wenn alle felder aufgedeckt und beflaggt sind sollen nutzereingaben gesperrt werden
    connect(mienenzaehler, SIGNAL(alle_felder_weg()), this, SLOT(sperre_felder()));
}


Scene::~Scene()
{
}


void Scene::neues_spiel(int breite, int hoehe)
{
    loesche_spiel();

    gesprengt = false;

    Breite = breite;
    Hoehe = hoehe;

    mienenzahl = Breite * Hoehe / 5;

    erzeuge_steine();

    verknuepfe_nachbarn();

    streue_mienen_ein();

    positioniere_steine();

    emit freigabe(true);
}


void Scene::erzeuge_steine()
{
    int fehlende_minen = (Breite * Hoehe) - gesamtliste.size();

    setSceneRect(0, 0, (Breite * GROESSE), (Hoehe * GROESSE));

    // wenn mienen fehlen die fehlende zahl mienen erzeugen
    if (fehlende_minen > 0)
    {
        for (register int idx = 0; idx < fehlende_minen; idx++)
        {
            Minenfeld *tmp_mine = new Minenfeld(this);

            tmp_mine->registriere_mienenzaehler(mienenzaehler);
            tmp_mine->setAcceptHoverEvents(true);

            // das melden von sprengungen ermoeglichen
            connect(tmp_mine, SIGNAL(sprengung()), this, SLOT(sprengung_erfolgt()));

            // das steuern der freigabe fuer die mine ermoeglichen
            connect(this, SIGNAL(freigabe(bool)), tmp_mine, SLOT(setEnabled(bool)));

            // mienenwarnungen fuer den developer modus ermoeglichen
            connect(tmp_mine, SIGNAL(mienenwarnung()), this, SIGNAL(mienenwarnung()));

            // aufdeckmeldung durchreichen ermoeglichen
            connect(tmp_mine, SIGNAL(aufgedeckt()), this, SIGNAL(aufgedeckt()));

            // die beflaggungsmeldung durchreichen ermoeglichen
            connect(tmp_mine, SIGNAL(beflaggt()), this, SIGNAL(beflaggt()));

            gesamtliste.append(tmp_mine);
        }
    }

    // wenn zuviele mienen da sind die ueberfluessigen loeschen, jedoch nur wenn mehr mienen vorhanden sind, als fuer ein schweres spiel benoetigt werden
    else if (fehlende_minen < 0 && gesamtliste.size() > (15 * 15))
    {
        for (register int idx = 0; idx > fehlende_minen; idx--)
        {
            delete gesamtliste.takeLast();
        }
    }

    // die objektnamen der mienen dem neuen standort anpassen
    register int gesamt_idx = 0;

    for (register int zeile = 0; gesamt_idx < gesamtliste.size() && zeile < Hoehe; zeile++)
    {
        for (register int spalte = 0; gesamt_idx < gesamtliste.size() && spalte < Breite; spalte++, gesamt_idx++)
        {
            Minenfeld *tmp_mine = gesamtliste.at(gesamt_idx);

            tmp_mine->setObjectName(QString::number(spalte) + SPLITTER + QString::number(zeile));

            addItem(tmp_mine);

            minenliste.insert(tmp_mine->objectName(), tmp_mine);
        }
    }
}


void Scene::positioniere_steine()
{
    for(register int idx1 = 0; idx1 < Hoehe; idx1++)
    {

        for(register int idx2 = 0; idx2 < Breite; idx2++)
        {
            minenliste.value(QString::number(idx2) + SPLITTER + QString::number(idx1))->setPos(idx2 * GROESSE, idx1 * GROESSE);
        }
    }
}


void Scene::verknuepfe_nachbarn()
{
    for (register int zeile = 0; zeile < Hoehe; zeile++)
    {
        for (register int spalte = 0; spalte < Breite; spalte++)
        {
            // nw
            if (minenliste.contains(QString::number(spalte - 1) + SPLITTER + QString::number(zeile - 1)) == true) minenliste.value(QString::number(spalte) + SPLITTER + QString::number(zeile))->registriere_nachbarn(minenliste.value(QString::number(spalte - 1) + SPLITTER + QString::number(zeile - 1)));

            // n
            if (minenliste.contains(QString::number(spalte) + SPLITTER + QString::number(zeile - 1)) == true) minenliste.value(QString::number(spalte) + SPLITTER + QString::number(zeile))->registriere_nachbarn(minenliste.value(QString::number(spalte) + SPLITTER + QString::number(zeile - 1)));

            // no
            if (minenliste.contains(QString::number(spalte + 1) + SPLITTER + QString::number(zeile - 1)) == true) minenliste.value(QString::number(spalte) + SPLITTER + QString::number(zeile))->registriere_nachbarn(minenliste.value(QString::number(spalte + 1) + SPLITTER + QString::number(zeile - 1)));

            // w
            if (minenliste.contains(QString::number(spalte - 1) + SPLITTER + QString::number(zeile)) == true) minenliste.value(QString::number(spalte) + SPLITTER + QString::number(zeile))->registriere_nachbarn(minenliste.value(QString::number(spalte - 1) + SPLITTER + QString::number(zeile)));

            // o
            if (minenliste.contains(QString::number(spalte + 1) + SPLITTER + QString::number(zeile)) == true) minenliste.value(QString::number(spalte) + SPLITTER + QString::number(zeile))->registriere_nachbarn(minenliste.value(QString::number(spalte + 1) + SPLITTER + QString::number(zeile)));

            // sw
            if (minenliste.contains(QString::number(spalte - 1) + SPLITTER + QString::number(zeile + 1)) == true) minenliste.value(QString::number(spalte) + SPLITTER + QString::number(zeile))->registriere_nachbarn(minenliste.value(QString::number(spalte - 1) + SPLITTER + QString::number(zeile + 1)));

            // s
            if (minenliste.contains(QString::number(spalte) + SPLITTER + QString::number(zeile + 1)) == true) minenliste.value(QString::number(spalte) + SPLITTER + QString::number(zeile))->registriere_nachbarn(minenliste.value(QString::number(spalte) + SPLITTER + QString::number(zeile + 1)));

            // so
            if (minenliste.contains(QString::number(spalte + 1) + SPLITTER + QString::number(zeile + 1)) == true) minenliste.value(QString::number(spalte) + SPLITTER + QString::number(zeile))->registriere_nachbarn(minenliste.value(QString::number(spalte + 1) + SPLITTER + QString::number(zeile + 1)));
        }
    }
}


void Scene::streue_mienen_ein()
{
    QList<Minenfeld*> unscharfe(minenliste.values());

    while (unscharfe.size() > 1 && scharfe.size() < mienenzahl)
    {
        Minenfeld *ziel = unscharfe.at(Random::random(0, unscharfe.size() - 1));

        ziel->setze_mine();

        unscharfe.removeAll(ziel);

        scharfe.append(ziel);
    }

    if (scharfe.size() < mienenzahl && unscharfe.isEmpty() == false)
    {
        Minenfeld *ziel = unscharfe.first();

        ziel->setze_mine();

        unscharfe.removeAll(ziel);

        scharfe.append(ziel);
    }

    mienenzaehler->setze_mienenzahl(scharfe.size());
    mienenzaehler->setze_feldzahl(Breite * Hoehe);
}


void Scene::sprengung_erfolgt()
{
    if (gesprengt == false)
    {
        gesprengt = true;

        emit freigabe(false);

        emit sprengung();

        for (register int idx = 0; idx < scharfe.size(); idx++) scharfe.at(idx)->decke_auf();
    }
}


void Scene::loesche_spiel()
{
    groessenmassstab = 0;

    emit freigabe(false);

    QStringList schluessel(minenliste.keys());

    for (register int idx = 0; idx < schluessel.size(); idx++)
    {
        removeItem(minenliste.value(schluessel.at(idx)));

        minenliste.value(schluessel.at(idx))->reset();
    }

    scharfe.clear();
    minenliste.clear();
}


void Scene::sperre_felder()
{
    emit freigabe(false);

    if (gesprengt == false) emit gewonnen();
}


void Scene::neues_spiel(int breite, int hoehe, int bomben)
{
    loesche_spiel();

    gesprengt = false;

    Breite = breite;
    Hoehe = hoehe;

    mienenzahl = bomben;

    erzeuge_steine();

    verknuepfe_nachbarn();

    streue_mienen_ein();

    positioniere_steine();

    emit freigabe(true);
}


int Scene::breite() const
{
    return Breite;
}


int Scene::hoehe() const
{
    return Hoehe;
}


int Scene::bomben() const
{
    return mienenzahl;
}
