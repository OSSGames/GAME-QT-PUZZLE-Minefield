#include "Benutzerlevel.h"
#include "Minenfeld.h"

using namespace std;

Benutzerlevel::Benutzerlevel(QWidget *parent) : QDialog(parent)
{
    setupUi(this);

    // signal - slot verbindungen
    // das anpassen des wertbereich von mienenbox ermoeglichen
    connect(breite_box, SIGNAL(valueChanged(int)), this, SLOT(min_max_steuerung_mienen_box()));
    connect(hoehe_box, SIGNAL(valueChanged(int)), this, SLOT(min_max_steuerung_mienen_box()));

    // das annehmen ermoeglichen
    connect(ok_button, SIGNAL(clicked()), this, SLOT(annehmen()));

    // initialisierungen
    min_max_steuerung_mienen_box();
}


Benutzerlevel::~Benutzerlevel()
{
}


void Benutzerlevel::min_max_steuerung_mienen_box()
{
    // minimal und maximalwert fuer mienen
    mienen_box->setRange(1, breite_box->value() * hoehe_box->value());

    // vorab gewaehlte anzahl an mienen
    mienen_box->setValue(breite_box->value() * hoehe_box->value() / 10);
}


void Benutzerlevel::annehmen()
{
    accept();

    emit neues_spiel(breite_box->value(), hoehe_box->value(), mienen_box->value());
}
