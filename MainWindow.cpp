#include "MainWindow.h"
#include "Scene.h"
#include "Highscore.h"
#include "Benutzerlevel.h"
#include "Minenfeld.h"
#include <QMessageBox>
#include <QLabel>
#include <QSettings>
#include <QTimer>
#include <QDebug>

#define GROESSE_WARTEZEIT 500

using namespace std;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    // die benutzeroberflaeche aufbauen
    setupUi(this);

    // osx spezifische einstellungen vornehmen
#ifdef Q_OS_MAC

    setUnifiedTitleAndToolBarOnMac(true);

#endif

    // unter macos soll menu_datei samt inhalt nicht zu sehen sein wenn nur action_schliessen enthalten ist
#ifdef Q_OS_MAC

    action_schliessen->setVisible(false);
    if (menu_datei->actions().size() == 1 && menu_datei->actions().first() == action_schliessen) menu_datei->menuAction()->setVisible(false);

#endif

    // scene erstellen ...
    scene = new Scene(viewer);

    // ... und in viewer registrieren
    viewer->setScene(scene);

    // einige label erzeugen ...
    zeit_label = new QLabel(this);
    mienenstatus_label = new QLabel(this);

    // ... und der statusleiste als permanentes widget hinzufuegen
    statusBar()->addPermanentWidget(zeit_label);
    statusBar()->addPermanentWidget(mienenstatus_label);

    // siegmeldung erzeugen
    siegmeldung = new QMessageBox(this);
    siegmeldung->setWindowModality(Qt::WindowModal);
    siegmeldung->setWindowTitle(tr("Minefield - You won !"));
    siegmeldung->setText(tr("You won ! But no new highscore."));
    siegmeldung->addButton(QMessageBox::Ok);

    // einstellungen erstellen
    einstellungen = new QSettings("konarski-wuppertal", "Minefield", this);

    // benutzerlevel erzeugen
    benutzerlevel = new Benutzerlevel(this);

    // den aktualisierungs timer fuer zeit_label erzeugen
    zeit_label_timer = new QTimer(this);
    zeit_label_timer->setInterval(1000);

    // unter macosx nicht noetig
#ifndef Q_OS_MAC

    // groesse_timer erstellen
    groesse_timer = new QTimer(this);
    groesse_timer->setSingleShot(true);

#endif

    // highscore erzeugen
    highscore = new Highscore(this);
    highscore->registriere_einstellungen(einstellungen);

    // signal - slot verbindungen
    // action_schliessen ermoeglichen
    connect(action_schliessen, SIGNAL(triggered(bool)), this, SLOT(close()));

    // starten von spielen ermoeglichen
    connect(action_spiel5, SIGNAL(triggered(bool)), this, SLOT(einfaches_spiel()));
    connect(action_spiel10, SIGNAL(triggered(bool)), this, SLOT(mittleres_spiel()));
    connect(action_spiel15, SIGNAL(triggered(bool)), this, SLOT(schweres_spiel()));

    // action_about ermoeglichen
    connect(action_about, SIGNAL(triggered(bool)), this, SLOT(about()));

    // action_about_qt ermoeglichen
    connect(action_about_qt, SIGNAL(triggered(bool)), qApp, SLOT(aboutQt()));

    // das anzeigen der mienenzahl ermoeglichen
    connect(scene, SIGNAL(verbleibende_mienen(int)), this, SLOT(aktualisiere_mienenstatus_label(int)));

    // das stoppen der zeitmessung ermoeglichen
    connect(scene, SIGNAL(gewonnen()), zeit_label_timer, SLOT(stop()));
    connect(scene, SIGNAL(sprengung()), zeit_label_timer, SLOT(stop()));

    // das anzeigen der siegmeldung ermoeglichen
    connect(scene, SIGNAL(gewonnen()), this, SLOT(gewonnen()));

    // das anzeigen von benutzerlevel ermoeglichen
    connect(action_eigenes_spiel, SIGNAL(triggered(bool)), this, SLOT(eigenes_spiel()));

    // das annehmen der werte aus benutzerlevel ermoeglichen
    connect(benutzerlevel, SIGNAL(neues_spiel(int, int, int)), scene, SLOT(neues_spiel(int, int, int)));
    connect(benutzerlevel, SIGNAL(neues_spiel(int, int, int)), this, SLOT(groesse_anpassen()));
    connect(benutzerlevel, SIGNAL(neues_spiel(int, int, int)), this, SLOT(starte_zeitmessung()));

    // reaktionen auf zeit_label_timer ermoeglichen
    connect(zeit_label_timer, SIGNAL(timeout()), this, SLOT(aktualisiere_zeit_label()));

    // action_highscore ermoeglichen
    connect(action_highscore, SIGNAL(triggered(bool)), highscore, SLOT(show()));

    QStringList argumente(qApp->arguments());

    if (argumente.size() == 2 && argumente.last() == "--developer")
    {
        qDebug() << "Developer Modus ist an :-)";

        // im debugmodus mienenwarnungen ermoeglichen
        connect(scene, SIGNAL(mienenwarnung()), this, SLOT(mienenwarnung()));
    }

    // unter macosx nicht noetig
#ifndef Q_OS_MAC

    // groesse_timer anschliessen
    connect(groesse_timer, SIGNAL(timeout()), this, SLOT(groesse_anpassen()));

#endif

    // einstellungen laden
    einstellungen_laden();
}


void MainWindow::einfaches_spiel()
{
    scene->neues_spiel(5, 5);

    groesse_anpassen();

    starte_zeitmessung();
}


void MainWindow::about()
{
    QMessageBox::about(this, tr("About Minefield"), tr("Minefield version %1 \n\nAuthor:\tAndreas Konarski\nLicence:\tgpl v3 or later\n\nContact:\n\nprogrammieren@konarski-wuppertal.de\nwww.konarski-wuppertal.de").arg(VERSION));
}


void MainWindow::mittleres_spiel()
{
    scene->neues_spiel(10, 10);

    groesse_anpassen();

    starte_zeitmessung();
}


MainWindow::~MainWindow()
{
    einstellungen_speichern();
}


void MainWindow::schweres_spiel()
{
    scene->neues_spiel(15, 15);

    groesse_anpassen();

    starte_zeitmessung();
}


void MainWindow::aktualisiere_mienenstatus_label(int wert)
{
    mienenstatus_label->setText(QString::number(wert) + tr(" mines left"));
}


void MainWindow::einstellungen_laden()
{
    // die fenstergeometrie laden
    restoreGeometry(einstellungen->value("hauptfenster/geometry").toByteArray());

    // die einstellungen fuer das spielfeld laden
    scene->neues_spiel(einstellungen->value("scene/breite", 5).toInt(), einstellungen->value("scene/hoehe", 5).toInt(), einstellungen->value("scene/bomben", 5).toInt());

    // die highscore laden
    highscore->einstellungen_laden();

    starte_zeitmessung();
}


void MainWindow::einstellungen_speichern()
{
    // die fenstergeometrie speichern
    einstellungen->setValue("hauptfenster/geometry", saveGeometry());

    // die einstellungen fuer das spielfeld speichern
    einstellungen->setValue("scene/breite", scene->breite());
    einstellungen->setValue("scene/hoehe", scene->hoehe());
    einstellungen->setValue("scene/bomben", scene->bomben());

    // die highscore speichern
    highscore->einstellungen_speichern();
}


void MainWindow::eigenes_spiel()
{
    benutzerlevel->show();
}


void MainWindow::groesse_anpassen()
{
    int links, oben, rechts, unten;

    centralWidget()->layout()->getContentsMargins(&links, &oben, &rechts, &unten);

    int breite = scene->width() + links + rechts + 4, hoehe = scene->height() + oben + unten + statusBar()->height() + 4;

#ifndef Q_OS_MAC

    hoehe += menuBar()->height();

#endif

    setMaximumSize(QSize(breite, hoehe));
}


void MainWindow::showEvent(QShowEvent*)
{
    // nur fuer macosx
#ifdef Q_OS_MAC

    groesse_anpassen();

    // unter macosx nicht noetig
#else

    groesse_timer->start();

#endif
}


void MainWindow::aktualisiere_zeit_label()
{
    int verstrichene_sekunden = startzeit.secsTo(QDateTime::currentDateTime());

    int stunden = verstrichene_sekunden / 3600;

    int minuten = (verstrichene_sekunden - (stunden * 3600)) / 60;

    int sekunden = verstrichene_sekunden - (minuten * 60) - (stunden * 3600);

    QString stunden_string = QString::number(stunden);
    QString minuten_string = QString::number(minuten);
    QString sekunden_string = QString::number(sekunden);

    if (stunden < 10) stunden_string.prepend('0');
    if (minuten < 10) minuten_string.prepend('0');
    if (sekunden < 10) sekunden_string.prepend('0');

    zeit_label->setText(stunden_string + ":" + minuten_string + ":" + sekunden_string);
}


void MainWindow::starte_zeitmessung()
{
    startzeit = QDateTime::currentDateTime();
    aktualisiere_zeit_label();
    zeit_label_timer->start();
}


void MainWindow::gewonnen()
{
    int verstrichene_sekunden = startzeit.secsTo(QDateTime::currentDateTime());

    int stunden = verstrichene_sekunden / 3600;

    int minuten = (verstrichene_sekunden - (stunden * 3600)) / 60;

    int sekunden = verstrichene_sekunden - (minuten * 60) - (stunden * 3600);

    if (highscore->neues_ergebnis(scene->breite(), scene->hoehe(), scene->bomben(), stunden, minuten, sekunden) == false) siegmeldung->show();
}


void MainWindow::mienenwarnung()
{
    qDebug() << "Vorsicht Miene!!!";
}
