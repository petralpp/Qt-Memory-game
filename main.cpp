/* Class MainWindow
 *
 * ----------
 * COMP.CS.110 SPRING 2021
 * ----------
 *
 * Muistipeli
 *
 * Ohjelma mallintaa muistipelin graafista käyttöliittymää.
 *
 * Pelin alkaessa käyttäjä valitsee valmiista vaihtoehdoista korttien
 * (8-20, tasaluku) ja pelaajien (2-4) määrän, ja halutessaan antaa
 * pelaajille myös nimet. Peli käynnistyy Start-painikkeesta.
 *
 * Pelin tarkoituksena on korttiparien kerääminen, jolloin kortteja eniten
 * kerännyt voittaa. Peli voi päättyä myös tasapeliin.
 * Kortteja käännetään klikkaamalla ja kahden kortin kääntämisen
 * jälkeen peli jatkuu Continue-painikkeesta. Jos kortit olivat pari,
 * vuorossa oleva pelaaja saa pisteen ja uuden vuoron, ja kortit
 * jäävät epäaktiivisina näkyville. Muutoin vuoro siirtyy seuraavalle
 * pelaajalle. Pelin tilanne näkyy yläoikealla tulostaulussa.
 * Kun kaikki kortit on käännetty, peli päättyy ja
 * voittaja tai tasapeli ilmoitetaan.
 *
 * Nimi: Petra Paloniemi
 * E-Mail: petra.paloniemi@tuni.fi
 **/

#include "mainwindow.hh"
#include <iostream>

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
