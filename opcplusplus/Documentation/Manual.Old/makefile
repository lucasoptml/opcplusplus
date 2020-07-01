# Kevin Depue (2006)
# Makefile for our manual.

manual: 
	latex manual.tex
	latex manual.tex
	dvips -tletter -Ppdf -o manual.ps manual.dvi
	ps2pdf manual.ps Manual.pdf
	rm -f *.aux *.toc *.undo *.log *.out
