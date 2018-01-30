/*  Projekt: Hra 2016 - Othello
    Predmet: Jazyk C++
    Datum: 6. 5. 2016
    Autori:     
        Nikola Valesova, xvales02
        Ondrej Vales, xvales03
 
    Modul: gui.h
*/

/** @file gui.h
	@brief Modul trid souvisejicich s grafickym uzivatelskym rozhranim
 
	Modul obsahujici tridy souvisejici s grafickym uzivatelskym rozhranim
*/

#ifndef GUI_H
#define GUI_H

#include <QInputDialog>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QMouseEvent>
#include <QPen>
#include <QBrush>
#include <QDir>
#include <QtGui>
#include <QGroupBox>
#include <QComboBox>
#include <QGridLayout>
#include <QFileDialog>
#include <QCloseEvent>
#include <string>
#include <vector>
#include <iostream>

#include "ui_gui.h"
#include "interface.h"
#include "board.h"

using namespace std;

namespace Ui 
{
	class gui;
};

/**
 * @brief      Definice tridy grafickeho uzivatelskeho rozhrani
 * Trida implementuje metody rozhrani interface_t v podobe grafickeho rozhrani
 */
class gui : public QMainWindow, public interface_t
{	
    Q_OBJECT
    QGraphicsScene *scene;
    int type;
public:
	explicit gui(QWidget *parent = 0);
	~gui();
	void print(board_t &board, int whose_turn);
	void print_end(string who);
	int init(int &size, vector <player_t> &players, string &game_name);
	void get_move(board_t &board, vector<int> &next_move, vector <vector <int>> &possible_positions);
private:
	Ui::gui *ui;
    void closeEvent(QCloseEvent *event);
signals:
	void sgn();
public slots:
    void clicked();
    void clicked_quit();
    void clicked_undo();
    void clicked_redo();
};


/**
 * @brief      Definice tridy initial dialogue, ktera obsahuje metody a atributy spojene
 * s uvodnim dialogem a ziskavanim udaju z neho
 */
class init_dialogue : public QWidget
{
    Q_OBJECT

public:
    gui *ui;
    init_dialogue(QWidget * parent = 0);
    ~init_dialogue();

    QGridLayout *grid;
    QGroupBox *groupbox_ng;
    QLabel *game_name_label;
    QLabel *size_label;
    QLineEdit *game_name_ng;
    QComboBox *game_size_ng;
    QLabel *opponent_label;
    QComboBox *opponent_ng;
    QPushButton *button_ng;
    QVBoxLayout *vbox_ng;

    QGroupBox *groupbox_lg;
    QPushButton *choose_history_button;
    QPushButton *button_lg;
    QLineEdit *show_path_lg;
    QPalette *palette_lg;
    QVBoxLayout *vbox_lg;

    QString game_name_lg;
    bool load_game;

private:
    QGroupBox *start_new_game();
    QGroupBox *load_saved_game();
    void closeEvent(QCloseEvent *event);

public slots:
    void handle_ng();
    void handle_lg();
    void open_folder();

signals:
    void carry_on();
};

#endif