#include "Highscore.h"
#include <QTableWidgetItem>
#include <QStringList>
#include <QDir>
#include <QInputDialog>
#include <QSettings>

#define NAME_POSITION 0
#define BREITE_POSITION 1
#define HOEHE_POSITION 2
#define MIENEN_POSITION 3
#define ZEIT_POSITION 4

#define ZEILEN 15
#define LEER "-"
#define SPLITTER ":"

using namespace std;

Highscore::Highscore(QWidget *parent) : QDialog(parent), einstellungen(0)
{
    setupUi(this);

    tabelle->setRowCount(ZEILEN);

    // die tabelle mit elementen befuellen
    for (register int idx1 = 0; idx1 < tabelle->rowCount(); idx1++)
    {
        for (register int idx2 = 0; idx2 <= ZEIT_POSITION; idx2++)
        {
            QTableWidgetItem *tmp_item = new QTableWidgetItem();

            tmp_item->setText(LEER);

            tabelle->setItem(idx1, idx2, tmp_item);
        }
    }
}


Highscore::~Highscore()
{
}


bool Highscore::neues_ergebnis(int breite, int hoehe, int mienen, int stunden, int minuten, int sekunden)
{
    // die bewertung des neuen ergebnisses errechnen. je mehr mienen und felder, je mehr punkte
    int ergebnis = breite * hoehe * (mienen / breite * hoehe * mienen);

    // den sekundenverbrauch fuer das neue ergebnis berechnen
    int sekundenverbrauch = (stunden * 3600) + (minuten * 60) + sekunden;

    bool erg = false;

    int ziel = -1;

    for (register int idx = ZEILEN - 1; idx >= 0; idx--)
    {
        int element_ergebnis = 0;
        int element_sekundenverbrauch = 0;

        QStringList tmp_zeit(tabelle->item(idx, ZEIT_POSITION)->text().split(SPLITTER, QString::SkipEmptyParts));

        // wenn das element an idx leer ist ist es auf jeden fall unterlegen
        if (tabelle->item(idx, BREITE_POSITION)->text() == LEER || tabelle->item(idx, HOEHE_POSITION)->text() == LEER || tabelle->item(idx, MIENEN_POSITION)->text() == LEER || tabelle->item(idx, ZEIT_POSITION)->text() == LEER)
        {
            ziel = idx;
        }

        // ansonsten
        else if (tmp_zeit.size() == 3)
        {
            // die bewertung des tabellenelements an idx berechnen. je mehr mienen und felder, je mehr punkte
            element_ergebnis = tabelle->item(idx, BREITE_POSITION)->text().toInt() * tabelle->item(idx, HOEHE_POSITION)->text().toInt() * (tabelle->item(idx, MIENEN_POSITION)->text().toInt() / breite * hoehe * mienen);

            // den sekundenverbrauch fuer des tabellenelements an idx berechnen
            element_sekundenverbrauch = (tmp_zeit.at(0).toInt() * 3600) + (tmp_zeit.at(1).toInt() * 60) + tmp_zeit.at(2).toInt();
        }

        // wenn das neue ergebnis mehr mienen enthaelt als das element an idx oder gleich viele mienen enthaelt, jedoch schneller geloest wurde ist das neue ergebnis ueberlegen und rueckt auf
        if (ergebnis > element_ergebnis || (ergebnis == element_ergebnis && sekundenverbrauch < element_sekundenverbrauch)) ziel = idx;
    }

    // wenn das neue element in die tabelle darf
    if (ziel >= 0 && ziel < tabelle->rowCount())
    {
        // eine neue reihe an ziel in die tabelle einfuegen ...
        tabelle->insertRow(ziel);

        // ... und mit elementen befuellen
        tabelle->setItem(ziel, NAME_POSITION, new QTableWidgetItem());
        tabelle->setItem(ziel, BREITE_POSITION, new QTableWidgetItem());
        tabelle->setItem(ziel, HOEHE_POSITION, new QTableWidgetItem());
        tabelle->setItem(ziel, MIENEN_POSITION, new QTableWidgetItem());
        tabelle->setItem(ziel, ZEIT_POSITION, new QTableWidgetItem());

        // der letzte muss die tabelle verlassen
        tabelle->removeRow(tabelle->rowCount() - 1);

        erg = true;
        bool ok;

        // den namen des nutzers abfragen
        QString name_vorbereitung(QDir::home().dirName());

        // wenn name_vorbereitung nicht leer ist
        if (name_vorbereitung.isEmpty() == false)
        {
            QChar erstes_zeichen = name_vorbereitung.at(0);

            if (erstes_zeichen.isLower() == true) name_vorbereitung[0] = erstes_zeichen.toUpper();
        }

        QString name = QInputDialog::getText(parentWidget(), tr("Minefield"), tr("You got highscore !"), QLineEdit::Normal, name_vorbereitung, &ok);

        // wenn kein nutzername eingegeben wurde den namen auf unbekannt setzen
        if (ok == false || name.isEmpty() == true) name = tr("Unknown");

        // wenn der name LEER entspricht den namen ebenfalls auf unbekannt setzen
        if (name == LEER) name = tr("Unknown");

        QString stunden_string = QString::number(stunden);
        QString minuten_string = QString::number(minuten);
        QString sekunden_string = QString::number(sekunden);

        if (stunden < 10) stunden_string.prepend('0');
        if (minuten < 10) minuten_string.prepend('0');
        if (sekunden < 10) sekunden_string.prepend('0');

        tabelle->item(ziel, NAME_POSITION)->setText(name);
        tabelle->item(ziel, BREITE_POSITION)->setText(QString::number(breite));
        tabelle->item(ziel, HOEHE_POSITION)->setText(QString::number(hoehe));
        tabelle->item(ziel, MIENEN_POSITION)->setText(QString::number(mienen));
        tabelle->item(ziel, ZEIT_POSITION)->setText(stunden_string + SPLITTER + minuten_string + SPLITTER + sekunden_string);

        // die highscoretabelle anzeigen
        show();
    }

    // wenn das neue ergebnis es in die highscore geschafft hat wird true zurueckgegeben, andernfalls false
    return erg;
}


void Highscore::registriere_einstellungen(QSettings* einstellungen_)
{
    einstellungen = einstellungen_;
}


void Highscore::einstellungen_laden()
{
    // die breite der spalten laden
    tabelle->setColumnWidth(NAME_POSITION, einstellungen->value(QString("tabelle/spalte_") + QString::number(NAME_POSITION), tabelle->columnWidth(NAME_POSITION)).toInt());
    tabelle->setColumnWidth(BREITE_POSITION, einstellungen->value(QString("tabelle/spalte_") + QString::number(BREITE_POSITION), tabelle->columnWidth(BREITE_POSITION)).toInt());
    tabelle->setColumnWidth(HOEHE_POSITION, einstellungen->value(QString("tabelle/spalte_") + QString::number(HOEHE_POSITION), tabelle->columnWidth(HOEHE_POSITION)).toInt());
    tabelle->setColumnWidth(MIENEN_POSITION, einstellungen->value(QString("tabelle/spalte_") + QString::number(MIENEN_POSITION), tabelle->columnWidth(MIENEN_POSITION)).toInt());
    tabelle->setColumnWidth(ZEIT_POSITION, einstellungen->value(QString("tabelle/spalte_") + QString::number(ZEIT_POSITION), tabelle->columnWidth(ZEIT_POSITION)).toInt());

    // den inhalt der tabelle laden
    for (register int idx = 0; idx < tabelle->rowCount() && einstellungen != 0; idx++)
    {
        // den namen laden
        tabelle->item(idx, NAME_POSITION)->setText(einstellungen->value(QString("highscore/") + QString::number(idx) + QString("name"), LEER).toString());

        // die breite laden
        tabelle->item(idx, BREITE_POSITION)->setText(einstellungen->value(QString("highscore/") + QString::number(idx) + QString("breite"), LEER).toString());

        // die hoehe laden
        tabelle->item(idx, HOEHE_POSITION)->setText(einstellungen->value(QString("highscore/") + QString::number(idx) + QString("hoehe"), LEER).toString());

        // die mienen laden
        tabelle->item(idx, MIENEN_POSITION)->setText(einstellungen->value(QString("highscore/") + QString::number(idx) + QString("mienen"), LEER).toString());

        // die zeit laden
        tabelle->item(idx, ZEIT_POSITION)->setText(einstellungen->value(QString("highscore/") + QString::number(idx) + QString("zeit"), LEER).toString());
    }
}


void Highscore::einstellungen_speichern()
{
    // die breite der spalten speichern
    einstellungen->setValue(QString("tabelle/spalte_") + QString::number(NAME_POSITION), tabelle->columnWidth(NAME_POSITION));
    einstellungen->setValue(QString("tabelle/spalte_") + QString::number(BREITE_POSITION), tabelle->columnWidth(BREITE_POSITION));
    einstellungen->setValue(QString("tabelle/spalte_") + QString::number(HOEHE_POSITION), tabelle->columnWidth(HOEHE_POSITION));
    einstellungen->setValue(QString("tabelle/spalte_") + QString::number(MIENEN_POSITION), tabelle->columnWidth(MIENEN_POSITION));
    einstellungen->setValue(QString("tabelle/spalte_") + QString::number(ZEIT_POSITION), tabelle->columnWidth(ZEIT_POSITION));

    // den inhalt der tabelle speichern
    for (register int idx = 0; idx < tabelle->rowCount() && einstellungen != 0; idx++)
    {
        // den namen speichern
        einstellungen->setValue(QString("highscore/") + QString::number(idx) + QString("name"), tabelle->item(idx, NAME_POSITION)->text());

        // die breite speichern
        einstellungen->setValue(QString("highscore/") + QString::number(idx) + QString("breite"), tabelle->item(idx, BREITE_POSITION)->text());

        // die hoehe speichern
        einstellungen->setValue(QString("highscore/") + QString::number(idx) + QString("hoehe"), tabelle->item(idx, HOEHE_POSITION)->text());

        // die mienen speichern
        einstellungen->setValue(QString("highscore/") + QString::number(idx) + QString("mienen"), tabelle->item(idx, MIENEN_POSITION)->text());

        // die zeit speichern
        einstellungen->setValue(QString("highscore/") + QString::number(idx) + QString("zeit"), tabelle->item(idx, ZEIT_POSITION)->text());
    }
}
