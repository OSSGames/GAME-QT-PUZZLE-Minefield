/*

 Name: Minefield
 Autor: Andreas Konarski
 Plattform: Alle Systeme, auf denen QT 4 verfuegbar ist. Kompatibel mit QT 5.0.0.
 Lizenz: GPL v3 or later

 Kontakt: programmieren@konarski-wuppertal.de
 home: www.konarski-wuppertal.de

 Falls ich mit diesem Programm die Rechte von irgend jemand verletzen sollte, bitte ich um einen Hinweis. Wenn dies Tatsaechlich der Fall ist, entferne ich es schnellstmoeglich von meiner Homepage.

 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define VERSION "0.49"

#include <QMainWindow>
#include <QDateTime>
#include "ui_MainWindow.h"

class Scene;
class QLabel;
class QMessageBox;
class QSettings;
class Benutzerlevel;
class QTimer;
class Highscore;

class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();

private slots:
    void einfaches_spiel();
    void mittleres_spiel();
    void schweres_spiel();
    void eigenes_spiel();
    void about();
    void aktualisiere_mienenstatus_label(int);
    void einstellungen_laden();
    void einstellungen_speichern();
    void groesse_anpassen();
    void aktualisiere_zeit_label();
    void starte_zeitmessung();
    void gewonnen();
    void mienenwarnung();

private:
    Scene *scene;
    QLabel *mienenstatus_label, *zeit_label;
    QMessageBox *siegmeldung;
    QSettings *einstellungen;
    Benutzerlevel *benutzerlevel;
    QTimer *zeit_label_timer;
    QDateTime startzeit;
    Highscore *highscore;

    // unter macosx nicht noetig
#ifndef Q_OS_MAC

    QTimer *groesse_timer;

#endif

    virtual void showEvent(QShowEvent*);
};

#endif
