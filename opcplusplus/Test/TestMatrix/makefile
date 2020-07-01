# Test matrix - linux makefile.
# Kevin Depue (2007)

# opC++
OPCPP      = ../../Distribution/bin/linux32/release/opcpp 
OPCPPFLAGS = -globmode -force -compact -verbose -gd Generated/TestMatrix 
DOH        = -doh dialect.doh
OH         = -oh code.oh

# parameters
INCDIR = -I . -I Generated/
SOURCE = *.cpp

# G++ flags
GPPFLAGS = -O2

# G++ build
testmatrix-gpp: 
	$(OPCPP) $(OPCPPFLAGS) $(DOH) $(OH)
	g++ $(GPPFLAGS) $(INCDIR) $(SOURCE) -o Linux/testmatrix-gpp

# G++ 2.95 build
testmatrix-gpp2.95:
	$(OPCPP) $(OPCPPFLAGS) $(DOH) $(OH)
	/usr/bin/g++-2.95 $(GPPFLAGS) $(INCDIR) $(SOURCE) -o Linux/testmatrix-gpp2.95

# G++ 3.4 build
testmatrix-gpp3.4:
	$(OPCPP) $(OPCPPFLAGS) $(DOH) $(OH)
	/usr/bin/g++-3.4 $(GPPFLAGS) $(INCDIR) $(SOURCE) -o Linux/testmatrix-gpp3.4

# G++ 4.0 build
testmatrix-gpp4.0:
	$(OPCPP) $(OPCPPFLAGS) $(DOH) $(OH)
	/usr/bin/g++-4.0 $(GPPFLAGS) $(INCDIR) $(SOURCE) -o Linux/testmatrix-gpp4.0

# Intel flags
INTELFLAGS = -static -Qinstall /opt/intel/cc/10.0.023 -Zp4 -xW

# Intel build
testmatrix-intel:
	$(OPCPP) $(OPCPPFLAGS) $(DOH) $(OH)
	icpc ${INTELFLAGS} ${INCDIR} ${SOURCE} -o Linux/testmatrix-intel

# profile build (just opcpp)
profile:
	valgrind --tool=callgrind ../../Distribution/bin/linux32/debug/opcpp ${OPCPPFLAGS} ${DOH} ${OH}

clean:
	rm -f callgrind.*
	rm -fr Generated/
	rm -f Linux/testmatrix-*
	rm -f *~ *.o *.ti *.ii 







