CXX=g++
CXXFLAGS=-std=c++2a -Wall -O3 -Iinclude
CXXFLAGSDEBUG=-std=c++2a -Wall -ggdb -Iinclude
AR=/usr/bin/ar -r
WAIT=echo "Appuyez pour continuer" && read _

srcl := $(shell find src/lib -type f -name *.cpp)
objl := $(subst src, obj, $(srcl:.cpp=.o))

srcc := $(shell find src/console -type f -name *.cpp)
objc := $(subst src, obj, $(srcc:.cpp=.o))

.PHONY: directories

all: directories bin/mine-morse.a bin/mine-morse_c
	@echo "Compilation réussie !"

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

clean: 
	@echo "Nettoyage\n"
	rm -rf bin
	rm -rf obj
	rm -rf test/str*.wav
	rm -rf test/str*_d.txt
	@echo "\nNettoyé"

test: all
	@echo "TESTS DU PROGRAMME\n"
	@echo "\tENCODAGE / DECODAGE LOCAL\n"
	bin/mine-morse_c enc test/str01.txt test/str01.wav
	@$(WAIT)
	bin/mine-morse_c dec test/str01.wav test/str01_d.txt
	@$(WAIT)
	bin/mine-morse_c enc test/str02.txt test/str02.wav
	@$(WAIT)
	bin/mine-morse_c dec test/str02.wav test/str02_d.txt
	@$(WAIT)

	@echo "\tVARIATIONS DES PARAMETRES\n"
	bin/mine-morse_c enc "Argument en ligne de commande" test/str03.wav
	@$(WAIT)
	bin/mine-morse_c dec test/str03.wav
	@$(WAIT)
	
	@echo "\tTEST DE DECODAGE DU MORSE GENERE EN LIGNE\n"
	bin/mine-morse_c dec test/site01_a.wav
	@$(WAIT)
	bin/mine-morse_c dec test/site01_b.wav
	@$(WAIT)
	bin/mine-morse_c dec test/site02_a.wav
	@$(WAIT)
	bin/mine-morse_c dec test/site02_b.wav
	@$(WAIT)
	bin/mine-morse_c dec test/site03_a.wav
	@$(WAIT)
	bin/mine-morse_c dec test/site03_b.wav
	@$(WAIT)

	@echo "\n\n\tVous pouvez essayer de décoder un fichier généré par ce programme à l'aide d'un site web (lien dans le README.md), par exemple avec test/str02.wav"