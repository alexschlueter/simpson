#include "lexer.h"
#include <iostream>

bool VariableRec::runWith(string strin) {
	return (strin.length() == 1 && (strin.find_first_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz") != -1));
}

bool NumberRec::runWith(string in) {
	if (DecimalRec::runWith(in)) return true;
	else return (RationalRec::runWith(in));
}

state DecimalRec::delta(state stt, char str) {
	switch (stt) {
	case START:
		if (str <= 57 && str >=49) return NFIRST;
		else if (str == '0') return ZFIRST;
		else if (str == '-' || str == '+') return VORZ;
		else return DEAD;
	case ACCPT:
		if (str <=57 && str >= 48) return ACCPT;
		else return DEAD;
	case ZFIRST:
		if (str == ',' || str == '.') return FIRSTT;
		else return DEAD;
	case NFIRST:
		if (str == ',' || str == '.') return FIRSTT;
		else if (str <=57 && str >= 48) return NFIRST;
		else return DEAD;
	case FIRSTT:
		if (str <=57 && str >= 48) return ACCPT;
		else return DEAD;
	case VORZ:
		if (str <= 57 && str >=49) return NFIRST;
		else return DEAD;
	}
}

bool DecimalRec::runWith(string in) {
	state stt = START;
	for (string::iterator i = in.begin(); i != in.end(); ++i)
		if ((stt = delta(stt, *i)) == DEAD) return false;
	return (stt == ACCPT || stt == NFIRST || stt == ZFIRST);
}

state RationalRec::delta(state stt, char str) {
	switch (stt) {
	case START:
		if (str <= 57 && str >=49) return NFIRST;
		else if (str == '0') return ZFIRST;
		else if (str == '-' || str == '+') return VORZ;
		else return DEAD;
	case ACCPT:
		if (str <=57 && str >= 48) return ACCPT;
		else return DEAD;
	case ZFIRST:
		if (str == '/') return FIRSTT;
		else return DEAD;
	case NFIRST:
		if (str == '/') return FIRSTT;
		else if (str <=57 && str >= 48) return NFIRST;
		else return DEAD;
	case FIRSTT:
		if (str <=57 && str >= 48) return ACCPT;
		else if (str == '-' || str == '+') return VORZNT;
		else return DEAD;
	case VORZ:
		if (str <= 57 && str >=49) return NFIRST;
		else return DEAD;
	case VORZNT:
		if (str <= 57 && str >=49) return ACCPT;
		else return DEAD;
	}
}

bool RationalRec::runWith(string in) {
	state stt = START;
	for (string::iterator i = in.begin(); i != in.end(); ++i)
		if ((stt = delta(stt, *i)) == DEAD) return false;
	return (stt == ACCPT || stt == NFIRST || stt == ZFIRST);
}

bool TermRec::runWith(string in) {
	string::iterator i;
	for (i = in.begin(); i != in.end(); ++i)
		if (OperatorRec::runWith(*i)) {break;}
	return (NumberRec::runWith(in.substr(0, i - in.begin())) && NumberRec::runWith(in.substr(i  - in.begin() + 1, in.length() - (i - in.begin() + 1))));
}

bool OperatorRec::runWith(char in) {
	return (in == '+');
}