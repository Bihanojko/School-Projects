#	Projekt: Hra 2016 - Othello
#	Predmet: Jazyk C++
# 	Datum: 6. 5. 2016
#  	Autori: 	
#  		Nikola Valesova, xvales02
#		Ondrej Vales, xvales03
#
# 	Makefile

compile:
	cd src && $(MAKE)

run: compile
	./hra2016 &
	./hra2016-cli

clean:
	rm -rf ./doc/* xvales02-xvales03.zip
	cd src && $(MAKE) clean

doxygen:
	doxygen ./src/Doxyfile

pack:
	zip xvales02-xvales03.zip * ./doc/* ./examples/* ./src/*
