#include "simpsonregel.h"
#include <iostream>

using namespace std;

void Fehler(string meldung) {
	cout << meldung << endl;
	system("PAUSE");
	exit(0);
}

template<typename T, typename P>
T remove_if(T beg, T end, P pred)
{
    T dest = beg;
    for (T itr = beg;itr != end; ++itr)
        if (!pred(*itr))
            *(dest++) = *itr;
    return dest;
}


Knoten::Knoten(string strin) {
	int ebenencount;
	bool geloescht;
	do {
		if (strin.empty()) throw "Ungueltiger Term";
		geloescht = false;
		if (strin.at(0) == '(' && strin.at(strin.length() - 1) == ')') {
			ebenencount = 0;
			for (string::iterator i = strin.begin() + 1; i != strin.end() - 1; ++i) {
				if (*i == '(') ebenencount++;
				else if (*i == ')' && --ebenencount == -1) break;
			}
			if (ebenencount != -1) {
				strin.erase(0, 1);
				strin.erase(strin.length() - 1, 1);
				geloescht = true;
			}
		}
	} while (geloescht);

	if (DecimalRec::runWith(strin)) {
		ktyp = WERT;
		wert = strin;
	} else if (strin == "e") {
		ktyp = WERT;
		wert = "2.71828182845905";
	} else if (strin == "pi") {
		ktyp = WERT;
		wert = "3.14159265358979";
	} else if (VariableRec::runWith(strin)) {
		ktyp = VAR;
		wert = strin;
	} else {
		ebenencount = 0;
		int pos_multi = 0, pos_divi = 0, pos_potenz = 0;

		for (string::reverse_iterator i = strin.rbegin(); i != strin.rend(); ++i) {
			if (*i == '(')  ebenencount--;
			else if (*i == ')') ebenencount++;
			else if (ebenencount == 0) {
				switch (*i) {
				case '+':
					if ((strin.rend() - i) == 1 || (i[1] != '-' && i[1] != '+' && i[1] != '*' && i[1] != '/' && i[1] != '^')) {
						ktyp = ADDITION;
						links = new Knoten(strin.substr(0, strin.rend() - i - 1));
						rechts = new Knoten(strin.substr(strin.rend() - i, i - strin.rbegin()));
						return;
					}
					break;
				case '-':
					if ((strin.rend() - i) != 1 && i[1] != '-' && i[1] != '+' && i[1] != '*' && i[1] != '/' && i[1] != '^') {
						ktyp = SUBTRAKTION;
						links = new Knoten(strin.substr(0, strin.rend() - i - 1));
						rechts = new Knoten(strin.substr(strin.rend() - i, i - strin.rbegin()));
						return;
					}
					break;
				case '*':
					if (! pos_multi && ! pos_divi) pos_multi = strin.rend() - i - 1;
					break;
				case '/':
					if (! pos_multi && ! pos_divi) pos_divi = strin.rend() - i - 1;
					break;
				case '^':
					if (! pos_potenz) pos_potenz = strin.rend() - i - 1;
					break;
				}
			}
		}

		if (pos_multi) {
			ktyp = MULTIPLIKATION;
			links = new Knoten(strin.substr(0, pos_multi));
			rechts = new Knoten(strin.substr(pos_multi + 1, strin.length() - pos_multi - 1));
		}else if (pos_divi) {
			ktyp = DIVISION;
			links = new Knoten(strin.substr(0, pos_divi));
			rechts = new Knoten(strin.substr(pos_divi + 1, strin.length() - pos_divi - 1));
		} else if (pos_potenz) {
			ktyp = POTENZ;
			links = new Knoten(strin.substr(0, pos_potenz));
			rechts = new Knoten(strin.substr(pos_potenz + 1, strin.length() - pos_potenz - 1));
		} else if (strin.substr(0, 5) == "sqrt(") {
			ktyp = WURZEL;
			rechts = new Knoten(strin.substr(5, strin.length() - 6));
		} else if (strin.substr(0, 4) == "log(") {
			ktyp = LOG;
			rechts = new Knoten(strin.substr(4, strin.length() - 5));
		} else if (strin.substr(0, 3) == "ln(") {
			ktyp = LN;
			rechts = new Knoten(strin.substr(3, strin.length() - 4));
		} else if (strin.substr(0, 4) == "sin(") {
			ktyp = SINUS;
			rechts = new Knoten(strin.substr(4, strin.length() - 5));
		} else if (strin.substr(0, 4) == "cos(") {
			ktyp = COSINUS;
			rechts = new Knoten(strin.substr(4, strin.length() - 5));
		} else if (strin.substr(0, 4) == "tan(") {
			ktyp = TANGENS;
			rechts = new Knoten(strin.substr(4, strin.length() - 5));
		} else throw "Ungueltiger Term";
	}
}

double Knoten::berechnen(double funktionswert) {
	switch (ktyp) {
	case ADDITION: return links->berechnen(funktionswert) + rechts->berechnen(funktionswert);
	case SUBTRAKTION: return links->berechnen(funktionswert) - rechts->berechnen(funktionswert);
	case MULTIPLIKATION: return links->berechnen(funktionswert) * rechts->berechnen(funktionswert);
	case DIVISION: return links->berechnen(funktionswert) / rechts->berechnen(funktionswert);
	case POTENZ: return pow(links->berechnen(funktionswert), rechts->berechnen(funktionswert));
	case WURZEL: return sqrt(rechts->berechnen(funktionswert));
	case LOG: return log10(rechts->berechnen(funktionswert));
	case LN: return log(rechts->berechnen(funktionswert));
	case SINUS: return sin(rechts->berechnen(funktionswert));
	case COSINUS: return cos(rechts->berechnen(funktionswert));
	case TANGENS: return tan(rechts->berechnen(funktionswert));
	case WERT: return strtod(wert.c_str(), NULL);
	case VAR: if (wert == "x" || wert == "X") return funktionswert;
	}
}

ostream& operator << (ostream& os, const Knoten& kt) {

	switch (kt.ktyp) {
	case ADDITION:
		os << "(" << *(kt.links) << "+" << *(kt.rechts) << ")";
		break;
	case SUBTRAKTION:
		os << "(" << *(kt.links) << "-" << *(kt.rechts) << ")";
		break;
	case MULTIPLIKATION:
		os << "(" << *(kt.links) << "*" << *(kt.rechts) << ")";
		break;
	case DIVISION:
		os << "(" << *(kt.links) << "/" << *(kt.rechts) << ")";
		break;
	case POTENZ:
		os << "(" << *(kt.links) << "^" << *(kt.rechts) << ")";
		break;
	case WURZEL:
		os << "sqrt(" << *(kt.rechts) << ")";
		break;
	case LOG:
		os << "log(" << *(kt.rechts) << ")";
		break;
	case LN:
		os << "ln(" << *(kt.rechts) << ")";
		break;
	case SINUS:
		os << "sin(" << *(kt.rechts) << ")";
		break;
	case COSINUS:
		os << "cos(" << *(kt.rechts) << ")";
		break;
	case TANGENS:
		os << "tan(" << *(kt.rechts) << ")";
		break;
	case WERT:
		os << kt.wert;
		break;
	case VAR:
		os << kt.wert;
		break;
	}
	return os;
}

BinBaum::BinBaum(string strin) {
	strin.erase(remove_if(strin.begin(), strin.end(), isspace), strin.end());
	ursprung = new Knoten(strin);
}

double BinBaum::berechnen(double funktionswert) {
	return ursprung->berechnen(funktionswert);
}

ostream& operator << (ostream& os, const BinBaum& baum) {
	os << *(baum.ursprung);
	return os;
}

double SimpsonRechnung::integrieren(BinBaum funktion, double untere_grenze, double obere_grenze, int teilintervalle) {
	double summe = 0, h;
	h = (obere_grenze - untere_grenze) / teilintervalle;
	for (int i = 1; i <= teilintervalle; i++)
		summe += (funktion.berechnen(untere_grenze + (i - 1)* h) + 4 * funktion.berechnen((2 * untere_grenze + (2 * i - 1) * h) / 2) + funktion.berechnen(untere_grenze + i * h));
	return (h/6) * summe;
}