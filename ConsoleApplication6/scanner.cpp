#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

using namespace std;
const int maxc = 25;
enum statetype{ newtoken, reservedword, variable, integer, real, delimiter };
enum chartype{ letter, digit, period, delim, blank, pod, eoln, illegal };

statetype stringtostate(string s)
{
	statetype answer;
	if (s == "NewToken")
		answer = newtoken;
	else if (s == "ReservedWord")
		answer = reservedword;
	else if (s == "Variable")
		answer = variable;
	else if (s == "Integer")
		answer = integer;
	else if (s == "Real")
		answer = real;
	else if (s == "Delimeter")
		answer = delimiter;
	return answer;
}

string statetostring(statetype s)
{
	string answer;
	if (s == newtoken)
		answer = "New Token";
	else if (s == reservedword)
		answer = "Reserved Word";
	else if (s == variable)
		answer = "Variable";
	else if (s == integer)
		answer = "Integer";
	else if (s == real)
		answer = "Real";
	else if (s == delimiter)
		answer = "Delimiter";
	return answer;
}

chartype stringtochar(string s)
{
	chartype answer;
	if (s == "Letter")
		answer = letter;
	else if (s == "Digit")
		answer = digit;
	else if (s == "Period")
		answer = period;
	else if (s == "Delimeter")
		answer = delim;
	else if (s == "Blank")
		answer = blank;
	else if (s == "POD")
		answer = pod;
	else if (s == "EOLN")
		answer = eoln;
	else if (s == "Illegal")
		answer = illegal;
	return answer;
}

string chartostring(chartype s)
{
	string answer;
	if (s == letter)
		answer ="Letter";
	else if (s == digit)
		answer ="Digit";
	else if (s == period)
		answer ="Period";
	else if (s == delim)
		answer = "Delimeter";
	else if (s == blank)
		answer = "Blank";
	else if (s == pod)
		answer = "POD";
	else if (s == eoln)
		answer = "EOLN";
	else if (s == illegal)
		answer ="Illegal";
	return answer;
}

chartype getchartype(char x){
	if (isalpha(x)) return letter;
	else if (isdigit(x)) return digit;
	else if (x == '.') return period;
	else if (x=='+'||x == '-'||x == '='||x == '^'|| x==',' || x=='>' || x=='<' || x=='"') 
		return delim;
	else if (x == '%' || x == '$') return pod;
	else if (x == ' ') return blank;
	else if (x == '|') return eoln;
	else return illegal;
}

void doaction(int x, char a, string &token, string res[], statetype &laststate, 
	ofstream &outf)
{
	bool is = false;
	switch (x){
	case 1:
		token += a;
		break;
	case 2:
		for (int i = 0; i < 14; i++){
			if (token == res[i]){
				is = true;
			}
		}
		if (is == true){
			outf << setw(10) << token << statetostring(laststate) << endl;
			token = "";
		}
		else
		{
			laststate = variable;
			outf << setw(10) << token << statetostring(laststate) << endl;
			token = "";
		}
		break;
	case 3:
		outf << setw(10) << token << statetostring(laststate) << endl;
		token = "";
		break;
	case 4:
		outf << setw(10) << token << statetostring(laststate) << endl;
		outf << "Improper usage: " << a << endl;
		token = "";
		break;
	case 5:
		outf << "Improper usage: " << a << endl;
		break;
	case 6:
		break;
	case 7:
		outf << "Illegal character: " << a << endl;
		break;
	case 8:
		for (int i = 0; i < 14; i++){
			if (token == res[i]){
				is = true;
			}
		}
		if (is == true){
			outf << setw(10) << token << statetostring(laststate) << endl;
			token = "";
			token += a;
		}
		else {
			laststate = variable;
			outf << setw(10) << token << statetostring(laststate) << endl;
			token = "";
			token += a;
		}
		break;
	case 9:
		outf << setw(10) << token << statetostring(laststate) << endl;
		token = "";
		token = +a;
		break;
	case 10:
		token += a;
		laststate = variable;
		outf << setw(10) << token << statetostring(laststate) << endl;
		token = "";
		break;
	case 11:
		for (int i = 0; i < 14; i++){
			if (token == res[i]){
				is = true;
			}
		}
		if (is == true){
			outf << setw(10) << token << statetostring(laststate) << endl;
			outf << "Illegal character: " << a << endl;
		}
		else {
			laststate = variable;
			outf << setw(10) << token << statetostring(laststate) << endl;
			outf << "Illegal character: " << a << endl;
		}
		break;
	case 12:
		token += a;
		outf << setw(10) << token << statetostring(laststate) << endl;
		token = "";
		break;
	case 13:
		outf << setw(10) << token << statetostring(laststate) << endl;
		outf << "Illegal character: " << a << endl;
		token = "";
		break;
	}
}

void printstate(ofstream &outd, statetype state[][8]){
	int i;
	outd << "\n\n\nState Table" << endl;
	outd << left << setw(15) << "" << setw(15) << "Letter" << setw(15) << "Digit" << setw(15) <<
		"Period" << setw(15) << "Delimeter" << setw(15) << "Blank" << setw(15) << "% or $" 
		<< setw(15) << "EOLN" << setw(15) << "Illegal" << endl;
	for (i = 0; i < 135;i++) outd << "-";
	outd << setw(15) << "\nNew Token";
	for (i = 0; i < 8; i++) outd << setw(15) << statetostring(state[0][i]);
	outd << setw(15) << "\nReserved Word";
	for (i = 0; i < 8; i++) outd << setw(15) << statetostring(state[1][i]);
	outd << setw(15) << "\nVariable";
	for (i = 0; i < 8; i++) outd << setw(15) << statetostring(state[2][i]);
	outd << setw(15) << "\nInteger";
	for (i = 0; i < 8; i++) outd << setw(15) << statetostring(state[3][i]);
	outd << setw(15) << "\nReal";
	for (i = 0; i < 8; i++) outd << setw(15) << statetostring(state[4][i]);
	outd << setw(15) << "\nDelimeter";
	for (i = 0; i < 8; i++) outd << setw(15) << statetostring(state[5][i]);
}

void printactions(ofstream &outd, int action[][8]){
	int i;
	outd << "\n\n\nAction Table" << endl;
	outd << setw(15) << "" << setw(15) << "Letter" << setw(15) << "Digit" << setw(15) <<
		"Period" << setw(15) << "Delimeter" << setw(15) << "Blank" << setw(15) << "% or $"
		<< setw(15) << "EOLN" << setw(15) << "Illegal" << endl;
	for (i = 0; i < 135; i++) outd << "-";
	outd << setw(15) << "\nNew Token";
	for (i = 0; i < 8; i++) outd << setw(15) << action[0][i];
	outd << setw(15) << "\nReserved Word";
	for (i = 0; i < 8; i++) outd << setw(15) << action[1][i];
	outd << setw(15) << "\nVariable";
	for (i = 0; i < 8; i++) outd << setw(15) << action[2][i];
	outd << setw(15) << "\nInteger";
	for (i = 0; i < 8; i++) outd << setw(15) << action[3][i];
	outd << setw(15) << "\nReal";
	for (i = 0; i < 8; i++) outd << setw(15) << action[4][i];
	outd << setw(15) << "\nDelimeter";
	for (i = 0; i < 8; i++) outd << setw(15) << action[5][i];
}

void getreserve(string reserve[], ofstream &outd){
	ifstream inf("reserve.dat");
	outd << "Reserve Words:\n";
	while (!inf.eof()){
		for (int i = 0; i < 14; i++) {
			inf >> reserve[i];
			outd << reserve[i] << endl;
		}
	}
}

void getexplain(string explain[], ofstream &outd){
	ifstream infe("explain.dat");
	string line;
	outd << "\n\n\nAction Explanations:\n";
	while (!infe.eof()){
		for (int i = 0; i < 13; i++){
			getline(infe, line);
			explain[i] = line;
			outd << line << endl;
		}
	}
}

void readactions(int action[][8]){
	ifstream inf2("action.dat");
	while (!inf2.eof()){
		for (int i = 0; i < 6; i++){
			for (int j = 0; j < 8; j++)	inf2 >> action[i][j];
		}
	}
}

void readstates(statetype states[][8]){
	ifstream infs("state.dat");
	string line;
	while (!infs.eof()){
		for (int i = 0; i < 6; i++){
			for (int j = 0; j < 8; j++){
				infs >> line;
				states[i][j] = stringtostate(line);
			}
		}
	}
}

void scan(string reserve[], int action[][8], statetype FSM[][8],ofstream &outf){
	ifstream  inff("prog1.bas");
	string line, token;
	int i, len, act;
	char ch;
	chartype cct;
	statetype state = newtoken;
	statetype laststate = newtoken;
	outf << left << setw(10) << "Token" << "Token Type" << endl;
	for (i = 0; i < 23; i++) outf << "-";
	outf << endl;
	while (!inff.eof())
	{
		getline(inff, line);
		line += '|';
		len = line.length();
		for (i = 0; i < len; i++)
		{
			ch = line[i];
			cct = getchartype(ch);
			act = action[state][cct];
			laststate = state;
			state = FSM[state][cct];
			doaction(act, ch, token, reserve, laststate, outf);
		}
	}
}

int main()
{
	string reserve[maxc];
	int action[6][8];
	string explain[13];
	statetype FSM[6][8];
	ofstream outf("output.out");
	ofstream outd("resources.out");

	getreserve(reserve, outd);
	getexplain(explain, outd);

	readactions(action);
	readstates(FSM);
	printstate(outd, FSM);
	printactions(outd, action);
	
	scan(reserve, action, FSM, outf);
}