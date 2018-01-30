# Projekt 5
# Typografie a publikovani
# Autor: Nikola Valesova
# Datum: 1. 5. 2016

NAME=proj5

all:
	latex $(NAME).tex
	latex $(NAME).tex
	dvips $(NAME).dvi
	ps2pdf -sPAPERSIZE=a4 $(NAME).ps

clean:
	rm -fr *.dvi *.log *.aux *.ps *.pdf *.out *.nav *.toc *.snm