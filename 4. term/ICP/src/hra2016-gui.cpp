/*  Projekt: Hra 2016 - Othello
    Predmet: Jazyk C++
    Datum: 6. 5. 2016
    Autori:     
        Nikola Valesova, xvales02
        Ondrej Vales, xvales03
 
    Modul: hra2016-gui.cpp
*/

/** @file hra2016-gui.cpp
	@brief Zdrojovy soubor hry s gui rozhranim
 
	Obsahuje vytvoreni rozhrani gui a instance hry	
*/

#include <iostream>
#include <vector>
#include <QApplication>

#include "game.h"
#include "gui.h"

using namespace std;


/**
 * @brief      Funkce rozehrani hry s gui rozhranim
 */

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    int size, type;
    string game_name;
    vector <player_t> players;
    players.reserve(2);

    gui interface_gui;
    interface_gui.setWindowTitle(QString::fromStdString("Othello"));
    ais ai;
	// uvodni rozhovor
    type = interface_gui.init(size, players, game_name);

    interface_gui.show();
    if (type == 1)	// nova hra
    {
        game_t game (&interface_gui, ai, size, game_name);
        game.play(players, true);
    }
    else	// nacteni hry
    {
        game_t game (&interface_gui, ai, game_name);
        game.play(players, false);
    }
    return 0;
}