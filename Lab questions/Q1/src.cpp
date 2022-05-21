#include<iostream>
#include<sstream>
#include<fstream>
using namespace std;
// Grammar
// S -> aSbS | bSaS | eps
bool S(int currChar);
int cursor;
string s;

bool match(char ch){
	//1st we must check length of str
	//2nd we first match ch==s[cursor] then increment it
	return cursor < s.length() && ch == s[cursor++];
}
bool E1(int currChar){
	cursor = currChar;
	return match('a') && S(cursor) && match('b') && S(cursor); 
}
bool E2(int currChar){
	cursor = currChar;
	return match('b') && S(cursor) && match('a') && S(cursor); 
}
bool S(int currChar){
	cursor = currChar;
	if(E1(cursor))
		return true;
	cursor = currChar;
	if(E2(cursor))
		return true;
	cursor = currChar;
	return true;
}
int main(){
	ifstream in("data.txt");
	ofstream out("Parsing.txt");
	while(getline(in,s)){
		if(S(0) && cursor==s.length())
			out<<"Parsed :)"<<endl;
		else
			out<<"Can't Parsed :(" <<endl;
	}
	system("Pause");
	return 0;
}