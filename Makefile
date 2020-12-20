CXX=g++
CXXFLAGS=-Wall -ggdb -Iinclude
AR=/usr/bin/ar -r 

srcl := $(shell find . -name "src/lib/*.cpp")
objl := $(subst src, obj, $(srcl:.cpp=.o))

srcc := $(shell find . -name "src/console/*.cpp")
objc := $(subst src, obj, $(srcc:.cpp=.o))

.PHONY: directories

all: directories bin/mine-morse.a bin/mine-morse_c 
	@echo "Compilation complète"
	@echo "LIB"
	@echo $(objl)
	@echo "CONSOLE"
	@echo $(objc)
	@echo ""

bin/mine-morse_c: ${objc} bin/mine-morse.a
	@echo "Compilation du programme console"
	$(CXX) $(CXXFLAGS) $^ -o bin/$@ 

bin/mine-morse.a: ${objl}
	@echo "Compilation de la librairie"
	${AR} bin/$@ $^

obj/%.o: src/%.cpp include/%.h
	@echo "Pré-compilation de $<"
	$(CXX) $(CXXFLAGS) -o $@ -c $<

directories: bin obj

bin:
	@echo "Création de bin"
	mkdir -p $@

obj:
	@echo "Création de obj"
	mkdir -p $@
	mkdir -p $@/lib
	mkdir -p $@/console


clean: 
	@echo "Nettoyage"
	rm -r bin
	rm -r obj
