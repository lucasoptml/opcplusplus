# opCPP Makefile
# can just be copied elsewhere and used

#
# makes generated files
#

#todo: only thing I need to add is making sure the directories are created
#1. extract directory from source
#2. mkdir (maybe as a target as well?)

#test dir too for now.

#todo: switch test w/ inc later
OHFILES = $(wildcard Inc/*.oh)
OHFILES+= $(wildcard Test/*.oh)

OCPPFILES=$(patsubst %.oh,Generated/%.ocpp,$(OHFILES))
OOHFILES=$(patsubst %.oh,Generated/%.ooh,$(OHFILES))

OCPPTEXT=$(patsubst %,\n\#include \"%\",$(OCPPFILES))
OOHTEXT=$(patsubst %,\n\#include \"%\",$(OOHFILES))

CPP_HEADER:=//Generated.cpp: Generated opCpp File\n\n
H_HEADER:=//Generated.h: Generated opCpp File\n\n

COMPILER := Release/opcpp.exe
#note: add -silent
COMPILEROPT := -verbose

#stupid cmd issue in windows :P
ifeq "$(OS)" "Windows_NT"
	MKDIR=mkdirectory
else
	MKDIR=mkdir
endif


all: generated


generated: Generated/Generated.cpp Generated/Generated.h

Generated/Generated.cpp: $(OCPPFILES) Generated/
	@echo Generating Generated.cpp...
	@echo -e "$(CPP_HEADER)$(OCPPTEXT)" > Generated/Generated.cpp


Generated/:
	@mkdir --parents Generated

	

Generated/Generated.h: $(OOHFILES) Generated/
	@echo Generating Generated.h...
	@echo -e "$(H_HEADER)$(OOHTEXT)" > Generated/Generated.h

#should call opcpp actually.
#should also make .ocpp & .ooh readonly
Generated/%.ocpp: %.oh
	@mkdir --parents $(@D)
	@$(COMPILER) $(COMPILEROPT) -oh $< -ocpp $@


#kinda weird here..
#

#not really necessary?
Generated/%.ooh: Generated/%.ocpp %.oh
	@cp $*.oh $@


clean:
	@echo Cleaning...
	@rm -r Generated/
	@echo Done.

