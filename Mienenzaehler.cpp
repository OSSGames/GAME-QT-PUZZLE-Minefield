#include "Mienenzaehler.h"

using namespace std;

Mienenzaehler::Mienenzaehler(QObject *parent) : QObject(parent), Mienen(0), Felder(0)
{
}


Mienenzaehler::~Mienenzaehler()
{
}


int Mienenzaehler::mienen() const
{
    return Mienen;
}


void Mienenzaehler::setze_mienenzahl(int wert)
{
    Mienen = wert;

    emit neuer_mienenstand(Mienen);
}


void Mienenzaehler::fuege_miene_hinzu()
{
    Felder++;

    emit neuer_mienenstand(++Mienen);
}


void Mienenzaehler::entferne_miene()
{
    emit neuer_mienenstand(--Mienen);

    if (--Felder == 0) emit alle_felder_weg();
}


int Mienenzaehler::felder() const
{
    return Felder;
}


void Mienenzaehler::setze_feldzahl(int wert)
{
    Felder = wert;
}


void Mienenzaehler::fuege_feld_hinzu()
{
    ++Felder;
}


void Mienenzaehler::entferne_feld()
{
    if (--Felder == 0) emit alle_felder_weg();
}
