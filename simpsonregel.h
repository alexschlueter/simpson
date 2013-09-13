#ifndef SimpsonH
#define SimpsonH

#include <string>
#include "lexer.h"

using namespace std;

void Fehler(string);

enum knotentyp { ADDITION, SUBTRAKTION, MULTIPLIKATION, DIVISION, POTENZ, WURZEL, LOG, LN, SINUS, COSINUS, TANGENS, WERT, VAR };

struct Knoten {
	Knoten *links;
	Knoten *rechts;
	string wert;
	knotentyp ktyp;

	Knoten(string);

	double berechnen(double);

	friend ostream& operator << (ostream&, const Knoten&);
};

class BinBaum {
private:
	Knoten *ursprung;

public:
	BinBaum(string);

	double berechnen(double);

	friend ostream& operator << (ostream&, const BinBaum&);
};

class SimpsonRechnung {
private:

public:
	static double integrieren(BinBaum, double, double, int); 
};

#endif //SimpsonH