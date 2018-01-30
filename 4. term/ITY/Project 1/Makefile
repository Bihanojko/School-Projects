NAME=proj1

all: $(NAME).pdf

$(NAME).dvi: $(NAME).tex
	latex $(NAME).tex

$(NAME).ps: $(NAME).dvi
	dvips $(NAME).dvi

$(NAME).pdf: $(NAME).ps
	ps2pdf $(NAME).ps

clean:
	rm -fr *.dvi *.log *.aux *.ps
