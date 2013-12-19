DIR_HPP=./inc/
DIR_CPP=./src/
DIR_OBJ=./tmp/
DIR_BIN=./bin/
DIR_TMP=./tmp/

CXXFLAGS=-O2 -Wall -pedantic -std=c++11 -I $(DIR_HPP)
LFLAGS=`allegro-config --libs`
OBJS=Bitmap.o main.o Window.o

# search paths
vpath %.hpp $(DIR_HPP)
vpath %.cpp $(DIR_CPP)
# vpath %.o $(DIR_OBJ) # wtf - does not work

SPH: $(OBJS)
	echo -e "\033[01;33m[make]\033[00;32m Linking all files..."
	echo -e "\033[01;33m[make]\033[01;36m $(addprefix $(DIR_OBJ), $^) \t\033[00;31m$(LFLAGS)\033[00m"
	$(CXX) $(LFLAGS) $(addprefix $(DIR_OBJ), $^) -o $(DIR_BIN)SPH
	ln -s -f $(DIR_BIN)SPH ./SPH
	echo -e "\033[01;33m[make]\033[01;36m $(DIR_BIN)SPH \033[00;32m has been built successfully. \033[00m"

$(OBJS): %.o: %.cpp
	@[ -d $(DIR_OBJ) ] || mkdir -p $(DIR_OBJ)
	@[ -d $(DIR_BIN) ] || mkdir -p $(DIR_BIN)
	@[ -d $(DIR_MAN) ] || mkdir -p $(DIR_MAN)
	@[ -d $(DIR_TMP) ] || mkdir -p $(DIR_TMP)
	echo -e "\033[01;33m[make]\033[01;36m $< \t\033[00;31m$(CXXFLAGS)\033[00m"
	$(CXX) $(CXXFLAGS) $< -o $(DIR_OBJ)$@ -c

clean:
	rm $(DIR_OBJ)*.o
	rm $(DIR_BIN)*
	rm ./SPH
	echo -e "\033[01;33m[make]\033[00;32m All *.o and binary files removed.\033[00m"

.PHONY: all clean doc
.SILENT :
