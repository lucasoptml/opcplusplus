# Kevin Depue & Lucas Ellis (2008)
# Makefile for opC++ mini manual.

manual: 
	latex manual.tex
	latex manual.tex
	dvips -tletter -Ppdf -o manual.ps manual.dvi
	ps2pdf manual.ps Manual.pdf
	rm -f *.aux *.toc *.log *.out *.ps *.dvi
