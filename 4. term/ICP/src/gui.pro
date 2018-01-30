#	Projekt: Hra 2016 - Othello
#	Predmet: Jazyk C++
# 	Datum: 6. 5. 2016
#  	Autori: 	
#  		Nikola Valesova, xvales02
#		Ondrej Vales, xvales03
# 
# 	Modul: gui.pro

QT       += core gui widgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TEMPLATE = app
MAKEFILE = gui_Makefile
TARGET = ../hra2016
DEPENDPATH += .
INCLUDEPATH += .
QMAKE_CXXFLAGS += -std=c++11

# Input
HEADERS += game.h board.h gui.h ai.h player.h interface.h interface_ai.h
SOURCES += hra2016-gui.cpp game.cpp board.cpp gui.cpp ai.cpp player.cpp
FORMS   += gui.ui
