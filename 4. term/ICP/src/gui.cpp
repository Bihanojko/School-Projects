/*	Projekt: Hra 2016 - Othello
	Predmet: Jazyk C++
 	Datum: 6. 5. 2016
  	Autori: 	
  		Nikola Valesova, xvales02
		Ondrej Vales, xvales03
 
 	Modul: gui.cpp
*/

/** @file gui.cpp
	@brief Modul metod pro graficke uzivatelske rozhrani
 
	Modul obsahujci implementace metod pro graficke uzivatelske rozhrani 	
*/

#include "gui.h"

using namespace std;

// pole moznosti velikosti hraci desky v textovem retezci
char sizes_of_board[4][3] = {"6", "8", "10", "12"};
// pole moznosti velikosti hraci desky jako ciselna hodnota
int sizes_of_board_i[4] = {6, 8, 10, 12};


gui::gui(QWidget *parent) : QMainWindow(parent), ui(new Ui::gui)
{
    ui->setupUi(this);
    scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, 498, 498);
    connect(ui->button, SIGNAL(clicked()), this, SLOT(clicked()));
    connect(ui->button_quit, SIGNAL(clicked()), this, SLOT(clicked_quit()));
    connect(ui->button_undo, SIGNAL(clicked()), this, SLOT(clicked_undo()));
    connect(ui->button_redo, SIGNAL(clicked()), this, SLOT(clicked_redo()));
    ui->end1->setVisible(false);
    ui->end2->setVisible(false);
}

/**
 * @brief      Destruktor grafickeho uzivatelskeho rozhrani
 * Rusi objekty vytvorene pri praci s rozhranim
 */
gui::~gui()
{
	delete scene;
    delete ui;
}

/**
 * @brief      Metoda na uvodni inicializaci hry pomoci dialogu a zpracovani zadanych udaju
 *
 * @param      size       Velikost hraci desky
 * @param      players    Vektor dvou trid o hraci
 * @param      game_name  Nazev hry
 *
 * @return     Hodnota udavajici, zda se ma vytvorit nova hra, nebo nacist jiz vytvorena
 */
int gui::init(int &size, vector <player_t> &players, string &game_name)
{
    // promenna pro ulozeni volby protihrace
	int opponent;

	// vytvoreni dialogu a jeho inicializace
	init_dialogue *dialogue = new init_dialogue();
	dialogue->ui = this;
	dialogue->setWindowTitle("Othello");

	// pozastaveni vsech akci, zobrazeni dialogu
	QEventLoop pause;
	connect(dialogue, SIGNAL(carry_on()), &pause, SLOT(quit()));
	dialogue->show();
	pause.exec();

	// pokud uzivatel kliknul na tlacitko pro nacteni hry
	if (dialogue->load_game == true)
	{
		// ulozi se cesta k souboru a odrizne koncovka (.save)
		game_name = dialogue->game_name_lg.toStdString();
		game_name.replace(game_name.end() - 5, game_name.end(), "");
		return 2;
	}
	// pokud uzivatel kliknul na tlacitko pro zapoceti nove hry
	else
	{
		// ulozi se jmeno hry, jeji velikost i volba oponenta
		game_name = dialogue->game_name_ng->text().toStdString();
		size = sizes_of_board_i[dialogue->game_size_ng->currentIndex()];
		opponent = dialogue->opponent_ng->currentIndex();
		players[0].set_type((int) 0);
		players[1].set_type(opponent);
	}

	delete dialogue;	
	return 1;
}

/**
 * @brief      Konstruktor tridy pro uvodni dialog, nastaveni rozlozeni okna dialogu a jeho rozmeru
 *
 * @param      parent  Odkaz na objekt, ze ktereho dialog dedi
 */
init_dialogue::init_dialogue(QWidget *parent) : QWidget(parent)
{
	grid = new QGridLayout;
	grid->addWidget(start_new_game(), 0, 0);
	grid->addWidget(load_saved_game(), 0, 1);
	setLayout(grid);
	resize(600, 100);
}

/**
 * @brief      Dektruktor tridy pro uvodni dialog, uvolneni pridelene pameti 
 */
init_dialogue::~init_dialogue()
{
	delete game_name_label;
	delete game_name_ng;
	delete size_label;
	delete game_size_ng;
	delete opponent_label;
	delete opponent_ng;
	delete button_ng;

	delete choose_history_button;
	delete button_lg;
	delete show_path_lg;
	delete palette_lg;
	delete vbox_lg;
	delete groupbox_lg;

	delete grid;
	delete groupbox_ng;
	//delete vbox_ng;			// ERROR
}

/**
 * @brief      Definice casti dialogu, ktera se zabyva udaji o nove vytvarene hre
 *
 * @return     Objekt slozeny ze vsech dilcich komponent - textovych napisu, tlacitek...
 */
QGroupBox *init_dialogue::start_new_game()
{
	// nadpis
	groupbox_ng = new QGroupBox(tr("Vytvorit novou hru"));
	// textove pole k prvni volbe
	game_name_label = new QLabel("Jmeno hry:");
	game_name_ng = new QLineEdit();
	// textove pole k druhe volbe
	size_label = new QLabel("Velikost hraci plochy:");
	// vytvoreni prvku s moznosti vyberu z vice preddefinovanych moznosti
	game_size_ng = new QComboBox();
	game_size_ng->addItem(sizes_of_board[0]);
	game_size_ng->addItem(sizes_of_board[1]);
	game_size_ng->addItem(sizes_of_board[2]);
	game_size_ng->addItem(sizes_of_board[3]);
    game_size_ng->setCurrentIndex(1);
	game_size_ng->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

	// textove pole ke treti volbe
	opponent_label = new QLabel("Protihrac:");
	// vytvoreni druheho prvku s moznosti vyberu z vice preddefinovanych moznosti
	opponent_ng = new QComboBox();
	opponent_ng->addItem("Druhy hrac");
	opponent_ng->addItem("Pocitac (lehci)");
	opponent_ng->addItem("Pocitac (tezsi)");
	opponent_ng->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

	// vytvoreni tlacitka pro spusteni hry se zadanymi parametry
	button_ng = new QPushButton(tr("Hrat"));

	// propojeni signalu s metodami
	connect(button_ng, SIGNAL(clicked()), this, SLOT(handle_ng()));

	// vytvoreni vysledneho okna k zobrazeni
	QVBoxLayout *vbox_ng = new QVBoxLayout;
	vbox_ng->addWidget(game_name_label);
	vbox_ng->addWidget(game_name_ng);
	vbox_ng->addWidget(size_label);
	vbox_ng->addWidget(game_size_ng);
	vbox_ng->addWidget(opponent_label);
	vbox_ng->addWidget(opponent_ng);
	vbox_ng->addWidget(button_ng);
	vbox_ng->addStretch(1);
	groupbox_ng->setLayout(vbox_ng);

	return groupbox_ng;
}

/**
 * @brief      Definice casti dialogu, ktera se zabyva udaji o nacteni ulozene hry
 *
 * @return     Objekt slozeny ze vsech dilcich komponent - textovych napisu, tlacitek...
 */
QGroupBox *init_dialogue::load_saved_game()
{
	// nadpis
	groupbox_lg = new QGroupBox(tr("Nacist ulozenou hru"));
	// vytvoreni tlacitka pro vyhledani historie k nacteni
	choose_history_button = new QPushButton(tr("Vyhledat historii"));
	// vytvoreni tlacitka pro nacteni zadane historie
	button_lg = new QPushButton(tr("Nacist"));
	show_path_lg = new QLineEdit();
	show_path_lg->setReadOnly(true);
	palette_lg = new QPalette();
	palette_lg->setColor(QPalette::Base, Qt::gray);
	palette_lg->setColor(QPalette::Text, Qt::darkGray);
	show_path_lg->setPalette(*palette_lg);

	// propojeni signalu s metodami
	connect(button_lg, SIGNAL(clicked()), this, SLOT(handle_lg()));
	connect(choose_history_button, SIGNAL(clicked()), this, SLOT(open_folder()));

	// vytvoreni vysledneho okna k zobrazeni
	vbox_lg = new QVBoxLayout;
	vbox_lg->addStretch(1);
	vbox_lg->addWidget(choose_history_button);
	vbox_lg->addWidget(show_path_lg);
	vbox_lg->addWidget(button_lg);
	vbox_lg->addStretch(1);
	groupbox_lg->setLayout(vbox_lg);

	return groupbox_lg;
}

/**
 * @brief      Metoda na zpracovani signalu vyvolaneho stisknutim tlacitka na vyber historie
 */
void init_dialogue::open_folder()
{
    game_name_lg = QFileDialog::getOpenFileName(this, tr("Vyhledat historii"), "./examples/", tr("Historie (*.save)"));
    show_path_lg->setText(game_name_lg);
}

/**
 * @brief      Metoda na zpracovani signalu vyvolaneho stisknutim tlacitka na zapoceti nove hry
 */
void init_dialogue::handle_ng()
{
	// kontrola, zda bylo vyplneno pole se jmenem hry
	if (!(game_name_ng->text().isEmpty()))
	{
		load_game = false;
		carry_on();
		hide();
		ui->show();
	}

	return;
}

/**
 * @brief      Metoda na zpracovani signalu vyvolaneho stisknutim tlacitka na nacteni ulozene hry
 */
void init_dialogue::handle_lg()
{
	// kontrola, zda bylo vyplneno pole se jmenem hry
	if (!(game_name_lg.isEmpty()))
	{
		load_game = true;
		carry_on();
		hide();
		ui->show();
	}

	return;
}

/**
 * @brief      Metoda na zavreni okna
 *
 * @param      event  Prichozi udalost
 */
void init_dialogue::closeEvent(QCloseEvent *event)
{
	event = event;
	exit(0);
}

/**
 * @brief      Metoda na zavreni okna gui
 *
 * @param      event  Prichozi udalost
 */
void gui::closeEvent(QCloseEvent *event)
{
	event = event;
	exit(0);
}

/**
 * @brief      Metoda zobrazujici stav hry
 *
 * @param      board       Hraci deska ktera se zobrazi
 * @param      whose_turn  Kdo je na tahu
 */
void gui::print(board_t &board, int whose_turn)
{
	int size = board.get_size();
	// fixni velikost zobrazovane desky
	static double step = 498.0 / size;

	// vycisteni hraci desky a vykresleni pozadi
	scene->clear();
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if ((i + j) % 2 == 0)
			{
				scene->addRect(i * step, j * step, step, step, QPen(Qt::NoPen), QBrush(QColor(0, 0, 175)));
			}
			else
			{
				scene->addRect(i * step, j * step, step, step, QPen(Qt::NoPen), QBrush(QColor(0, 0, 125)));
			}
		}
	}

	// vykresleni kamenu
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if (board.get_board()[i * size + j] == 1)
			{
				scene->addEllipse(j * step + 5, i * step + 5, step - 10, step - 10, QPen(Qt::NoPen), QBrush(QColor(255, 255, 255)));
			}
			else if (board.get_board()[i * size + j] == 2)
			{
				scene->addEllipse(j * step + 5, i * step + 5, step - 10, step - 10, QPen(Qt::NoPen), QBrush(QColor(0, 0, 0)));
			}
			else if (board.get_board()[i * size + j] == 3)
			{
				scene->addEllipse(j * step + (step / 4) + 5, i * step + (step / 4) + 5, step - (step / 2) - 10, step - (step / 2) - 10, QPen(Qt::NoPen), QBrush(QColor(127, 127, 127)));
			}
		}
	}

	// vypis skore a udaju kdo je na tahu
	string aux = "Hrac 1 : " + std::to_string(board.get_score()[0]) + "   " + std::to_string(board.get_score()[1]) + " : Hrac 2";
	ui->name_field->setText(QString::fromStdString(aux));
	aux = "Na tahu je hrac: " + std::to_string(whose_turn + 1);
	ui->who_field->setText(QString::fromStdString(aux));

	ui->graphicsView->setScene(scene);
}

/**
 * @brief      Metoda zobrazujici konec hry
 *
 * @param      who  Kdo vyhral
 */
void gui::print_end(string who)
{
    ui->end2->setText(QString::fromStdString("<html><head/><body><p><span style=\" color:#ff0000;\">Vyhral hrac cislo " + who + "!</span></p></body></html>"));
    ui->end1->setVisible(true);
    ui->end2->setVisible(true);
	ui->graphicsView->setScene(scene);

	QEventLoop pause;
	while (true)
	{
		connect(this, SIGNAL(sgn()), &pause, SLOT(quit()));
		pause.exec();

		if (type == -5)
		{
			return;
		}
	}
}

/**
 * @brief      Metoda dotazujici se na dalsi tah
 *
 * @param      board                   Hraci deska
 * @param      next_move               Vektor do ktereho je ulozen zadany tah
 * @param      possible_positions      Seznam moznych pozic
 */
void gui::get_move(board_t &board, vector<int> &next_move, vector <vector <int>> &possible_positions)
{
	static double step = 498.0 / board.get_size();
	QEventLoop pause;

	while (true)
	{
		// cekani na kliknuti
		connect(this, SIGNAL(sgn()), &pause, SLOT(quit()));
		pause.exec();

		// vypocet na jake policko bylo kliknuto
		if (type == 0)
		{
			QPoint pos = ui->graphicsView->mapFromGlobal(QCursor::pos());

			next_move[0] = pos.x() / step;
			next_move[1] = pos.y() / step;

			for (const vector<int> &v : possible_positions)
			{
				if (v[0] == next_move[1] && v[1] == next_move[0])
					return;
			}
		} else 	// undo, redo nebo quit
		{
			next_move[0] = type;
			return;
		}
	}
}

/**
 * @brief      Metoda obsluhujici kliknuti na desku
 */
void gui::clicked()
{
	type = 0;
	sgn();
}

/**
 * @brief      Metoda obsluhujici kliknuti na guit
 */
void gui::clicked_quit()
{
	type = -5;
	sgn();
}

/**
 * @brief      Metoda obsluhujici kliknuti na undo
 */
void gui::clicked_undo()
{
	type = -1;
	sgn();
}

/**
 * @brief      Metoda obsluhujici kliknuti na redo
 */
void gui::clicked_redo()
{
	type = -2;
	sgn();
}