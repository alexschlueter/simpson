#include "simpsonregel.h"
#include <iostream>
#include <limits>
#include <Windows.h>

using namespace std;

int main(int argc, const char argv[]) {
	string in;
	double untere_grenze = 0, obere_grenze = 0;
	int teilintervalle;
	char debug=0;
	for (;;) {
		cout << "Funktion	f(x) = ";
		getline(cin, in);
		try {
			BinBaum funktion(in);
			cout << "Ich habe verstanden:	" << funktion << endl;
			cout << "Untere Grenze	:";
			cin >> untere_grenze;
			cout << "Obere Grenze	: ";
			cin >> obere_grenze;
			cout << "Teilintervalle	: ";
			cin >> teilintervalle;
			for (int i = 1; i <= teilintervalle; i++)
				cout << i << (i == 1?" Intervall	: ":" Intervalle	: ") << SimpsonRechnung::integrieren(funktion, untere_grenze, obere_grenze, i) << endl;
			cin.ignore((numeric_limits<streamsize>::max)(), '\n');
		} catch (const char *meldung) {
			cout << meldung << endl;
		}
	}
	
	return 0;
}