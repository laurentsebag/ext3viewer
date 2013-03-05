pdf: ps
	ps2pdf ./ext3Viewer.1.ps
ps:
	groff -t -e -mandoc -Tps ./ext3Viewer.1 > ./ext3Viewer.1.ps
#txt:
#  groff -t -e -mandoc -Tascii ext3Viewer.1 | col -bx > ext3viewer_manpage.txt
clean:
	rm -f ./ext3Viewer.1.ps ./ext3Viewer.1.pdf *~

.PHONY: ps pdf clean
