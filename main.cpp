/*

 Name: Minefield
 Autor: Andreas Konarski
 Begonnen: 20.07.2009.
 Erstellt: 21.02.2013.
 Version: 0.49
 Plattform: Alle Systeme, auf denen QT 4 verfuegbar ist. Kompatibel mit QT 5.0.0.
 Lizenz: GPL v3 or later
 
 Kontakt: programmieren@konarski-wuppertal.de
 home: www.konarski-wuppertal.de
 
 Falls ich mit diesem Programm die Rechte von irgend jemand verletzen sollte, bitte ich um einen Hinweis. Wenn dies Tatsaechlich der Fall ist, entferne ich es schnellstmoeglich von meiner Homepage.
 
 */

#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>
#include <QLocale>
#include "MainWindow.h"

using namespace std;

int main(int argc, char *argv[])
{
    QApplication anwendung(argc, argv);

    // die anwendung uebersetzen
    QTranslator uebersetzer, uebersetzer2;

    QString dateiname_eigene_uebersetzung;
    dateiname_eigene_uebersetzung = QString(":/translations/Uebersetzung_%1").arg(QLocale::system().name());

    uebersetzer.load("qt_" + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    uebersetzer2.load(dateiname_eigene_uebersetzung);

    // dafuer sorgen, das alle standartwidgets auf deutsch uebersetzt wird
    anwendung.installTranslator(&uebersetzer);
    anwendung.installTranslator(&uebersetzer2);

    // den anwendungsnamen setzen
    anwendung.setApplicationName("Minefield");

    MainWindow hauptfenster;

    hauptfenster.show();

    return anwendung.exec();
}
