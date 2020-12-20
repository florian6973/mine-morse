CXX=g++
CXXFLAGS=-std=c++2a -Wall -ggdb -Iinclude
AR=/usr/bin/ar -r

srcl := $(shell find src/lib -type f -name *.cpp)
objl := $(subst src, obj, $(srcl:.cpp=.o))

srcc := $(shell find src/console -type f -name *.cpp)
objc := $(subst src, obj, $(srcc:.cpp=.o))

.PHONY: directories

all: directories bin/mine-morse.a bin/mine-morse_c

ar: all run

bin/mine-morse_c: ${objc} bin/mine-morse.a
	@echo "Compilation du programme console"
	$(CXX) $(CXXFLAGS) $^ -o $@ 
	@echo ""

bin/mine-morse.a: ${objl}
	@echo "Compilation de la librairie"
	${AR} $@ $^
	@echo ""

obj/%.o: src/%.cpp include/%.h
	@echo "Pré-compilation de $<"
	$(CXX) $(CXXFLAGS) -o $@ -c $<
	@echo ""

directories: bin obj

bin:
	@echo "Création de bin"
	mkdir -p $@
	@echo ""

obj:
	@echo "Création de obj"
	mkdir -p $@
	mkdir -p $@/lib
	mkdir -p $@/console
	@echo ""

run: bin/mine-morse_c
	@echo "Programme console\n"
	@bin/mine-morse_c $(ARGS)

clean: 
	@echo "Nettoyage\n"
	rm -rf bin
	rm -rf obj
	@echo "\nNettoyé"
