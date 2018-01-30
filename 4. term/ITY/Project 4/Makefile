NAME=proj4

all:
	latex $(NAME).tex
	bibtex $(NAME).aux
	latex $(NAME).tex
	latex $(NAME).tex
	dvips $(NAME).dvi
	ps2pdf -sPAPERSIZE=a4 $(NAME).ps

clean:
	rm -fr *.dvi *.log *.aux *.ps *.pdf *.bbl *.blg