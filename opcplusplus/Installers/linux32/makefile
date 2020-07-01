# Kevin Depue (2007)
# opC++ Linux Makefile.

# flags
CFLAGS = -fpermissive -static -Qinstall /opt/intel/cc/10.0.023 -Zp4 

# includes
INCDIR  = -I ../../../Lib/boost/ 
INCDIR += -I ../../../opBasis/Inc/opstl/ 
INCDIR += -I ../../../Lib/md5/
INCDIR += -I ../../Inc/ 

# source
SOURCE  = ../../Src/RegexSource.cpp
SOURCE += ../../../Lib/boost/libs/filesystem/src/operations_posix_windows.cpp
SOURCE += ../../../Lib/boost/libs/filesystem/src/convenience.cpp
SOURCE += ../../../Lib/boost/libs/filesystem/src/path_posix_windows.cpp
SOURCE += ../../../Lib/boost/libs/filesystem/src/exception.cpp
SOURCE += ../../../Lib/md5/md5.c
SOURCE += ../../Src/opCPPSource.cpp
SOURCE += ../../Src/LinuxSource.cpp

intel32:
	icpc ${CFLAGS} ${INCDIR} ${SOURCE} -o ../../Distribution/bin/linux32/release/opcpp 
	chmod a-st+rwx ../../Distribution/bin/linux32/release/opcpp
	upx -9 ../../Distribution/bin/linux32/release/opcpp

intel32-debug:
	icpc ${CFLAGS} -g ${INCDIR} ${SOURCE} -o ../../Distribution/bin/linux32/debug/opcpp
	chmod a-st+rwx ../../Distribution/bin/linux32/debug/opcpp
	
# clean
clean:
	rm -f *~ *.o *.ti *.ii 
