# Author: Nikola Valesova
# Date: 15. 10. 2017
# Subject: Soft Computing
# File: xvales02.pro
# Faculty of Information Technology
# Brno University of Technology


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

QMAKE_MAKEFILE = xvales02Makefile

TARGET = xvales02

HEADERS += \
    FuzzyOperations.h \
    libs/qcustomplot.h

SOURCES += \
    main.cpp \
    FuzzyOperations.cpp \
    libs/qcustomplot.cpp

DISTFILES += \
    libs/changelog.txt \
    libs/GPL.txt
