#ifndef LEXERH
#define LEXERH

#include <string>

using namespace std;

class VariableRec {
public:
	static bool runWith(string);
};

enum state {START, DEAD, ACCPT,		 ZFIRST			, NFIRST					, FIRSTT								, VORZ		     , VORZNT};
//			q0	 , tot , akzeptiert, Null am Anfang , Zahl auﬂer Null am Anfang , erstes Trennzeichen (Komma oder Bruch), Minus am Anfang, Minus nach Trennzeichen

class NumberRec {
private:

public:
	static bool runWith(string);
};

class DecimalRec {
private:

public:
	static state delta(state, char);
	static bool runWith(string);
};

class RationalRec {
private:

public:
	static state delta(state, char);
	static bool runWith(string);
};

class TermRec {
public:
	static bool runWith(string);
};

class OperatorRec {
public:
	static bool runWith(char);
};

#endif