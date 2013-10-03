# placeholder

HDR = $(wildcard src/*.hpp)
ALL_SRC = $(wildcard src/*.cpp)
BLACKLIST = src/calculate_stats_us_binpacking.cpp src/EvaluateMapping.cpp
SRC = $(filter-out $(BLACKLIST), $(ALL_SRC))
#HDR = code/*.cpp 
OBJ = ${SRC:.cpp=.o}
LINK = -lcrypto
EXE = SDC.exe
CPP = g++
CPPFLAGS = -O3 -g -Iincl/
STRIP = strip
.PHONY = clean export clean-all
TEMP = $(shell ls | grep -v "makefile")

all: ${EXE}

$(EXE): ${OBJ}
#		${CPP} ${CPPFLAGS} -o ${EXE} ${OBJ}
		${CPP} ${CPPFLAGS} ${OBJ} ${LINK} -o ${EXE}
		${STRIP} ${EXE}

Commons.o : makefile Commons.cpp Commons.hpp
		${CPP} ${CPPFLAGS} -c Commons.cpp -o Commons.o

MD5Hash.o : makefile Commons.o MD5Hash.cpp MD5Hash.hpp
		${CPP} ${CPPFLAGS} -c MD5Hash.cpp -o MD5Hash.o

Floyd-Warshall.o: makefile Commons.o Floyd-Warshall.cpp Floyd-Warshall.hpp MD5Hash.o
		${CPP} ${CPPFLAGS} -c Floyd-Warshall.cpp -o Floyd-Warshall.o

GraphIR.o: makefile Commons.o GraphIR.cpp GraphIR.hpp
		${CPP} ${CPPFLAGS} -c GraphIR.cpp -o GraphIR.o

HEFT.o: makefile Commons.o HEFT.cpp HEFT.hpp
		${CPP} ${CPPFLAGS} -c HEFT.cpp -o HEFT.o

SDC.o: makefile Commons.o SDC.cpp SDC.hpp MD5Hash.o GraphIR.o Floyd-Warshall.o HEFT.o
		${CPP} ${CPPFLAGS} -c SDC.cpp -o SDC.o

# Make modifiers
clean:
		rm -rf ${OBJ} ${EXE}
		-rm ${TEMP} #'-' at the start of the line forces make to ignore errors from this command

clean-all:
		rm -rf ${OBJ} ${EXE} input/* output/*

export:
		rm -rf ${OBJ} ${EXE}
		tar --exclude=*git* --exclude=*input* --exclude=*output* --exclude=.git* -czvf SDC.tar.gz ../SDC/*
