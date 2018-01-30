/* Author: Nikola Valesova
 * Date: 15. 10. 2017
 * Subject: Soft Computing
 * File: main.cpp
 * Faculty of Information Technology
 * Brno University of Technology
*/


#include "FuzzyOperations.h"

#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>


// MAIN - initialize main window and show it
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QMainWindow window;
    FuzzyOperations *widget = new FuzzyOperations();
    window.resize(900, 650);
    window.setCentralWidget(widget);
    window.setWindowTitle("Fuzzy Operations");
    window.setStyleSheet("background-color:white;");
    window.show();

    return a.exec();
}
