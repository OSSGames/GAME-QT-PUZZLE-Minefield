/*

 Name: Minefield
 Autor: Andreas Konarski
 Plattform: Alle Systeme, auf denen QT 4 verfuegbar ist. Kompatibel mit QT 5.0.0.
 Lizenz: GPL v3 or later

 Kontakt: programmieren@konarski-wuppertal.de
 home: www.konarski-wuppertal.de

 Falls ich mit diesem Programm die Rechte von irgend jemand verletzen sollte, bitte ich um einen Hinweis. Wenn dies Tatsaechlich der Fall ist, entferne ich es schnellstmoeglich von meiner Homepage.

 */

#ifndef BENUTZERLEVEL_H
#define BENUTZERLEVEL_H

#include <QDialog>
#include "ui_Benutzerlevel.h"

class Benutzerlevel : public QDialog, private Ui::Benutzerlevel
{
    Q_OBJECT

public:
    Benutzerlevel(QWidget *parent = 0);
    virtual ~Benutzerlevel();

signals:
    void neues_spiel(int, int, int);

private slots:
    void min_max_steuerung_mienen_box();
    void annehmen();
};

#endif
