
doc: doc_devel.dvi pdf

pdf: doc_devel.dvi
	dvipdf doc_devel.dvi && mv doc_devel.pdf ../
doc_devel.dvi: doc_devel.tex doc_devel.ind
	latex doc_devel.tex && latex doc_devel.tex && latex doc_devel.tex
doc_devel.ind: doc_devel.idx
	makeindex doc_devel.idx
doc_devel.idx:
	latex doc_devel.tex
clean:
	rm -f doc_devel.{aux,idx,ind,ilg,lof,log,lot,toc,dvi} *.log *~ *.backup

.PHONY: clean doc pdf

