all : ChaineMain ReconstitueReseau main

ChaineMain : ChaineMain.o Chaine.o SVGwriter.o
	gcc -o ChaineMain ChaineMain.o Chaine.o SVGwriter.o

ChaineMain.o : ChaineMain.c Chaine.h
	gcc -c ChaineMain.c

Chaine.o : Chaine.c Chaine.h SVGwriter.h
	gcc -c Chaine.c

SVGwriter.o : SVGwriter.c SVGwriter.h
	gcc -c SVGwriter.c

ReconstitueReseau : ReconstitueReseau.o Reseau.o Chaine.o SVGwriter.o
	gcc -o ReconstitueReseau ReconstitueReseau.o Reseau.o Chaine.o SVGwriter.o

ReconstitueReseau.o : ReconstitueReseau.c Reseau.h 
	gcc -c ReconstitueReseau.c

Reseau.o : Reseau.c Reseau.h SVGwriter.h
	gcc -c Reseau.c

main : main.o Chaine.o Reseau.o Hachage.o ArbreQuat.o SVGwriter.o
	gcc -o main main.o Chaine.o Reseau.o Hachage.o ArbreQuat.o SVGwriter.o

main.o : main.c Chaine.h Reseau.h Hachage.h ArbreQuat.h
	gcc -c main.c

Hachage.o : Hachage.c Hachage.h
	gcc -c Hachage.c

ArbreQuat.o : ArbreQuat.c ArbreQuat.h
	gcc -c ArbreQuat.c

clean : 
	del /Q  *.o #del /Q pour windows et rm -f pour linux
	del /Q *.exe