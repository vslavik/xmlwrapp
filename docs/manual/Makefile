SRC = $(wildcard *.texi)

OUTFIL = xmlwrapp

MAIN = manual.texi

all: pdf html info

pdf: $(OUTFIL).pdf

$(OUTFIL).pdf: $(SRC)
	texi2pdf --batch --verbose --output=$@ $(MAIN)

html: $(OUTFIL).html

$(OUTFIL).html: $(SRC)
	makeinfo --html --verbose --output=$@ $(MAIN)

info: $(OUTFIL).info

$(OUTFIL).info: $(SRC)
	makeinfo --verbose --output=$@ $(MAIN)
