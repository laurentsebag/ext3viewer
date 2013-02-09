
doc: abstract.dvi pdf

pdf: abstract.dvi
	dvipdf abstract.dvi && mv abstract.pdf ../
abstract.dvi: abstract.tex
	latex abstract.tex && latex abstract.tex && latex abstract.tex
clean:
	rm -f abstract.{aux,idx,ind,ilg,lof,log,lot,toc,dvi} *.log *~ *.backup

.PHONY: clean doc pdf

