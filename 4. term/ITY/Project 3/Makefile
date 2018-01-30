NAME=proj3

all: $(NAME).pdf

$(NAME).dvi: $(NAME).tex
	latex $(NAME).tex
	latex $(NAME).tex

$(NAME).ps: $(NAME).dvi
	dvips -t a4 $(NAME).dvi

$(NAME).pdf: $(NAME).ps
	ps2pdf -sPAPERSIZE=a4 $(NAME).ps

clean:
	rm -fr *.dvi *.log *.aux *.ps
