all : ChaineMain ReconstitueReseau temps GrapheMain

ChaineMain : ChaineMain.o Chaine.o SVGwriter.o
	gcc -o ChaineMain ChaineMain.o Chaine.o SVGwriter.o -lm

ChaineMain.o : ChaineMain.c Chaine.h
	gcc -c ChaineMain.c

ReconstitueReseau : ReconstitueReseau.o Reseau.o Chaine.o SVGwriter.o Hachage.o ArbreQuat.o
	gcc -o ReconstitueReseau ReconstitueReseau.o Reseau.o Chaine.o SVGwriter.o Hachage.o ArbreQuat.o -lm

ReconstitueReseau.o : ReconstitueReseau.c Reseau.h Hachage.h ArbreQuat.h
	gcc -c ReconstitueReseau.c

temps : temps.o Chaine.o Reseau.o Hachage.o ArbreQuat.o SVGwriter.o
	gcc -o temps temps.o Chaine.o Reseau.o Hachage.o ArbreQuat.o SVGwriter.o -lm

temps.o : temps.c Chaine.h Reseau.h Hachage.h ArbreQuat.h
	gcc -c temps.c

GrapheMain : GrapheMain.o Reseau.o Chaine.o Graphe.o SVGwriter.o File.o ListeEntiers.o ArbreQuat.o
	gcc -o GrapheMain GrapheMain.o Reseau.o Chaine.o Graphe.o SVGwriter.o File.o ListeEntiers.o ArbreQuat.o -lm

GrapheMain.o : GrapheMain.c Reseau.h Graphe.h ArbreQuat.h
	gcc -c GrapheMain.c

Chaine.o : Chaine.c Chaine.h SVGwriter.h
	gcc -c Chaine.c

SVGwriter.o : SVGwriter.c SVGwriter.h
	gcc -c SVGwriter.c

Reseau.o : Reseau.c Reseau.h Chaine.h SVGwriter.h
	gcc -c Reseau.c

Hachage.o : Hachage.c Hachage.h Reseau.h
	gcc -c Hachage.c

ArbreQuat.o : ArbreQuat.c ArbreQuat.h Reseau.h
	gcc -c ArbreQuat.c

Graphe.o : Graphe.c Graphe.h File.h ListeEntiers.h
	gcc -c Graphe.c

File.o : File.c File.h
	gcc -c File.c

ListeEntiers.o : ListeEntiers.c ListeEntiers.h
	gcc -c ListeEntiers.c

clean : 
	del /Q  *.o #del /Q pour windows et rm -f pour linux
	del /Q *.exe