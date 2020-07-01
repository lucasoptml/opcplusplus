# opC++ default project makefile.

#=======================
# opC++ variables
#=======================

OPCPPFLAGS  = -globmode -compact 
OPCPPGENDIR = -gd Generated/default 
OPCPPDOH    = -doh dialect.doh
OPCPPOH     = -oh code.oh
OPCPPDEPEN  = *.doh *.oh
OPCPPTARGET = Generated/default/Generated.oohindex 

#=======================
# g++ variables
#=======================

GPPFLAGS  = -O2
GPPINCDIR = -I . -I Generated/
GPPSOURCE = *.cpp
GPPDEPEN  = $(OPCPPTARGET) *.h *.cpp

#=======================
# builds
#=======================

# opcpp build
$(OPCPPTARGET): $(OPCPPDEPEN)
	opcpp $(OPCPPFLAGS) $(OPCPPGENDIR) $(OPCPPDOH) $(OPCPPOH)

# G++ build
default: $(GPPDEPEN) 
	g++ $(GPPFLAGS) $(GPPINCDIR) $(GPPSOURCE) -o default

clean:
	rm -fr Generated/
	rm -f default
