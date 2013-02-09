
doc: rapport.dvi rapport.tex

pdf: rapport.dvi
	dvipdf rapport.dvi && mv rapport.pdf ../
rapport.dvi: rapport.tex rapport.ind
	latex rapport.tex && latex rapport.tex && latex rapport.tex
rapport.ind: rapport.idx
	makeindex rapport.idx
rapport.idx:
	latex rapport.tex
clean:
	rm -f rapport.{aux,idx,ind,ilg,lof,log,lot,toc,dvi} *.log *~ *.backup

.PHONY: clean doc pdf

