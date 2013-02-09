
doc: user_doc.dvi pdf

pdf: user_doc.dvi
	dvipdf user_doc.dvi && mv user_doc.pdf ../
user_doc.dvi: user_doc.tex user_doc.ind
	latex user_doc.tex && latex user_doc.tex && latex user_doc.tex
user_doc.ind: user_doc.idx
	makeindex user_doc.idx
user_doc.idx:
	latex user_doc.tex
clean:
	rm -f user_doc.{aux,idx,ind,ilg,lof,log,lot,toc,dvi} *.log *~ *.backup

.PHONY: clean doc pdf

