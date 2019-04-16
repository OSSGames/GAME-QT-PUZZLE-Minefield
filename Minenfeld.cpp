#include "Minenfeld.h"
#include "Mienenzaehler.h"
#include <QPoint>
#include <QPainter>
#include <QRect>
#include <QRectF>
#include <QPen>
#include <QFont>
#include <QGraphicsSceneMouseEvent>
#include <QPointF>

using namespace std;

Minenfeld::Minenfeld(QObject *oparent, QGraphicsItem *gparent) : QObject(oparent), QGraphicsItem(gparent), Feldwert(0), Aufgedeckt(false), Beflaggt(false), Freigegeben(false), mienenzaehler(0)
{
}


Minenfeld::~Minenfeld()
{
}


void Minenfeld::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    painter->setRenderHint(QPainter::Antialiasing);

    // wenn das feld nicht aufgedeckt ist
    if (Aufgedeckt == false)
    {
        // denn aussenring malen
        painter->setBrush(Qt::darkGray);

        painter->drawRect(boundingRect().adjusted(painter->pen().widthF() / 2, painter->pen().widthF() / 2, painter->pen().widthF() / 2, painter->pen().widthF() / 2));

        if (Beflaggt == true)
        {
            QPointF mitte((double) GROESSE / (double) 3, (double) GROESSE / (double) 2);

            QPen flaggen_stift(Qt::black);
            flaggen_stift.setWidth((double) GROESSE / (double) 15);

            painter->setBrush(Qt::black);
            painter->setPen(flaggen_stift);

            QLineF flaggenstab_oben(mitte, QPointF(mitte.x(), 0));
            QLineF flaggenstab_unten(mitte, QPointF(mitte.x(), GROESSE));

            flaggenstab_oben.setLength((double) flaggenstab_oben.length() * (double) 0.7);
            flaggenstab_unten.setLength((double) flaggenstab_unten.length() * (double) 0.8);

            QLineF flaggenstab(flaggenstab_unten.p2(), flaggenstab_oben.p2());

            painter->drawLine(flaggenstab);

            flaggenstab_oben.setP1(flaggenstab.p2());
            flaggenstab_oben.setP2(flaggenstab.p1());

            flaggenstab_oben.setAngle(flaggenstab_oben.angle() + 90);

            flaggenstab_oben.setLength((double) GROESSE / 2.5);

            painter->drawLine(flaggenstab_oben);

            flaggenstab_oben.setP1(QPointF(flaggenstab_oben.p1().x(), flaggenstab_oben.p1().y() + GROESSE / 10));
            flaggenstab_oben.setP2(QPointF(flaggenstab_oben.p2().x(), flaggenstab_oben.p2().y() + GROESSE / 10));
            painter->drawLine(flaggenstab_oben);

            flaggenstab_oben.setP1(QPointF(flaggenstab_oben.p1().x(), flaggenstab_oben.p1().y() + GROESSE / 10));
            flaggenstab_oben.setP2(QPointF(flaggenstab_oben.p2().x(), flaggenstab_oben.p2().y() + GROESSE / 10));
            painter->drawLine(flaggenstab_oben);

            flaggenstab_oben.setP1(QPointF(flaggenstab_oben.p1().x(), flaggenstab_oben.p1().y() + GROESSE / 10));
            flaggenstab_oben.setP2(QPointF(flaggenstab_oben.p2().x(), flaggenstab_oben.p2().y() + GROESSE / 10));
            painter->drawLine(flaggenstab_oben);
        }
    }

    // wenn das feld aufgedeckt ist
    else
    {
        // denn aussenring malen
        painter->setBrush(Qt::white);

        painter->drawRect(boundingRect().adjusted(painter->pen().widthF() / 2, painter->pen().widthF() / 2, painter->pen().widthF() / 2, painter->pen().widthF() / 2));

        double schriftmass = (double) GROESSE / (double) 10;

        int schriftgroesse = schriftmass * 8;

        // wenn es sich bei dem feld weder um ein null feld handelt noch um ein vermientes
        if (Feldwert > 0 && Feldwert <= 8)
        {
            QFont font;
            font.setPixelSize(schriftgroesse);
            painter->setFont(font);

            painter->drawText(QRect(QPoint((int) schriftmass, (int) schriftmass), QSize(schriftgroesse, schriftgroesse)), Qt::AlignHCenter | Qt::AlignVCenter, QString::number(Feldwert));
        }

        // wenn das feld vermient ist
        else if (Feldwert == -1)
        {
            QPointF mitte((double) GROESSE / (double) 2, (double) GROESSE / (double) 2);

            QPen mienen_stift(Qt::black);
            mienen_stift.setWidth((double) GROESSE / (double) 15);

            painter->setBrush(Qt::black);

            painter->drawEllipse(mitte, (double) GROESSE / (double) 4, (double) GROESSE / (double) 4);

            painter->setPen(mienen_stift);

            QLineF mienen_hilfslinie(mitte, QPointF(mitte.x(), 0));
            mienen_hilfslinie.setLength((double) mienen_hilfslinie.length() * 0.7);

            for (register int idx = 0; idx < 360; idx += 24)
            {
                mienen_hilfslinie.setAngle(idx);

                painter->drawLine(mienen_hilfslinie);
            }
        }
    }
}


QRectF Minenfeld::boundingRect() const
{
    return QRect(QPoint(0, 0), QSize(GROESSE, GROESSE));
}


void Minenfeld::registriere_nachbarn(Minenfeld *nachbar)
{
    Nachbarn.append(nachbar);

    // signal - slot verbindungen
    // wenn ein nachbar sich als miene identifiziert darauf reagieren
    connect(nachbar, SIGNAL(bin_miene()), this, SLOT(nachbar_ist_miene()));

    // aneinander grenzende 0 felder sollen die aufdeckung weitergeben
    connect(nachbar, SIGNAL(wurde_aufgedeckt(char)), this, SLOT(decke_auf(char)));
}


void Minenfeld::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    // wenn die steuerung freigegeben ist und der klick sich innerhalb des sicheren bereiches befindet
    if (Freigegeben == true && event->pos().x() > SICHERHEITSRAND && event->pos().y() > SICHERHEITSRAND)
    {
        QGraphicsItem::mousePressEvent(event);

        // wenn das feld nicht bereits aufgedeckt ist deckt die linke maustaste das feld auf
        if (event->button() == Qt::LeftButton && Aufgedeckt == false) decke_auf();

        // wenn das feld nicht bereits aufgedeckt ist setzt oder wiederruft die rechte maustaste flaggen
        else if (event->button() == Qt::RightButton && Aufgedeckt == false)
        {

            // wenn das feld nicht bereits beflaggt ist
            if (Beflaggt == false)
            {
                // wenn noch mienen verfuegbar sind
                if (mienenzaehler->mienen() > 0) setze_flagge();
            }

            // ansonsten die flagge entfernen
            else entferne_flagge();
        }
    }
}


void Minenfeld::setze_mine()
{
    Feldwert = -1;

    update();

    emit bin_miene();
}


bool Minenfeld::ist_mine() const
{
    bool erg = false;

    if (Feldwert == -1) erg = true;

    return erg;
}


void Minenfeld::nachbar_ist_miene()
{
    if (Feldwert != -1)
    {
        Feldwert++;

        update();
    }
}


void Minenfeld::decke_auf(char wert)
{
    if (Aufgedeckt == false && Beflaggt == false)
    {
        // wenn das feld, das die aufdeckung ausgeloest hat ein 0 feld ist und dieses feld ebenfalls ein 0 feld ist
        if (wert == 0 && Feldwert != -1) decke_auf();
    }
}


void Minenfeld::decke_auf()
{
    if ((Aufgedeckt == false && Beflaggt == false) || Freigegeben == false)
    {
        Aufgedeckt = true;

        update();

        emit aufgedeckt();

        if (Feldwert == -1) emit sprengung();

        emit wurde_aufgedeckt(Feldwert);

        if (Freigegeben == true) mienenzaehler->entferne_feld();
    }
}


void Minenfeld::setze_flagge()
{
    if (Beflaggt == false)
    {
        Beflaggt = true;

        update();

        emit beflaggt();

        mienenzaehler->entferne_miene();
    }
}

void Minenfeld::entferne_flagge()
{
    if (Beflaggt == true)
    {
        Beflaggt = false;

        update();

        mienenzaehler->fuege_miene_hinzu();
    }
}


void Minenfeld::setEnabled(bool wert)
{
    Freigegeben = wert;
}


void Minenfeld::reset()
{
    for (register int idx = 0; idx < Nachbarn.size(); idx++)
    {
        // wenn ein nachbar sich als miene identifiziert darauf reagieren
        disconnect(Nachbarn.at(idx), SIGNAL(bin_miene()), this, SLOT(nachbar_ist_miene()));

        // aneinander grenzende 0 felder sollen die aufdeckung weitergeben
        disconnect(Nachbarn.at(idx), SIGNAL(wurde_aufgedeckt(char)), this, SLOT(decke_auf(char)));
    }

    Feldwert = 0;
    Aufgedeckt = Beflaggt = Freigegeben = false;
    Nachbarn.clear();
}


void Minenfeld::registriere_mienenzaehler(Mienenzaehler* objekt)
{
    mienenzaehler = objekt;
}


void Minenfeld::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
    QGraphicsItem::hoverEnterEvent(event);

    if (Feldwert == -1 && Beflaggt == false) emit mienenwarnung();
}


void Minenfeld::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
    QGraphicsItem::hoverLeaveEvent(event);
}
