#	Projekt: Hra 2016 - Othello
#	Predmet: Jazyk C++
# 	Datum: 6. 5. 2016
#  	Autori: 	
#  		Nikola Valesova, xvales02
#		Ondrej Vales, xvales03
#
# 	Makefile

FLAGS=-std=c++11

all:hra2016-cli hra2016

hra2016-cli:hra2016.o game.o board.o cli.o ai.o player.o
	g++ $(FLAGS) hra2016.o game.o board.o cli.o ai.o player.o -o ../hra2016-cli

hra2016:
	qmake gui.pro
	$(MAKE) -f gui_Makefile

hra2016.o:hra2016.cpp
	g++ $(FLAGS) -c hra2016.cpp -o hra2016.o

hra2016-gui.o:hra2016-gui.cpp
	g++ $(FLAGS) -c hra2016-gui.cpp -o hra2016-gui.o

game.o:game.cpp
	g++ $(FLAGS) -c game.cpp -o game.o

board.o:board.cpp
	g++ $(FLAGS) -c board.cpp -o board.o

cli.o:cli.cpp
	g++ $(FLAGS) -c cli.cpp -o cli.o

gui.o:cli.cpp
	g++ $(FLAGS) -c gui.cpp -o gui.o

ai.o:ai.cpp
	g++ $(FLAGS) -c ai.cpp -o ai.o

player.o:player.cpp
	g++ $(FLAGS) -c player.cpp -o player.o

clean:
	rm -f *.o ../hra2016-cli ../hra2016 gui_Makefile moc_gui.cpp ui_gui.h
