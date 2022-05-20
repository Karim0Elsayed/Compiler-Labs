#include <iostream>
#include <stdio.h>
#include <vector>
#include <set>
#include <algorithm>
#include <iterator>
#include <fstream>
using namespace std;
using namespace std;
/*
 the main concept which my code based on the ASCII code for Characters
 if i said  : "array[A] = 15;" this mean at position that corresponding to A=65 in ASCII
 i will put value 15
*/
#define eps '@'
class CFG {
public:
	//Members 
	set<char> First[256], Follow[256];
	vector<string> productions[256];
	int terminals[256], nonTerminals[256];
	bool changedFirst, changedFollow;
	char startSymbol;

	//Functions to handel Grammar
	CFG();
	vector<string> mySpliter(string str); // To splite each Non-Terminal Productions
	bool isUpper(char ch); //To test if char terminal or Non-terminal
	void addProduction(char ch, string str); // Make extra task that check Terminals , NonTer
	void addStartSymbol(char ch);

	//Functions to Compute First and Follow
	void ComputeFirst();
	void addFirst(int i, int j, int index);
	void ComputeFollow();
	void addFollow(int i, int j, int index, int k);

	//Functions to print Result 
	void printFirst();
	void printFollow();
	
};

//Implementation for functions that handle Grammar
CFG::CFG() {
	for (int i = 0; i < 256; i++) {
		nonTerminals[i] = 0;
		terminals[i] = 0;
	}
	changedFirst = false;
}
vector<string> CFG::mySpliter(string str) {
	vector<string> res;
	string temp;
	for (int i = 0; i < str.size(); i++) {
		if (str[i] == '|') {
			res.push_back(temp);
			temp = "";
		}
		else
			temp += str[i];
	}
	res.push_back(temp);
	return res;
}
void CFG::addProduction(char ch, string str) {
	productions[ch] = mySpliter(str);
	nonTerminals[ch] = 1;

	//To Know Terminals and NonTerminals
	for (int i = 0; i < str.length(); i++) {
		char temp = str[i];
		if (temp != '|') {
			if (isUpper(temp))
				nonTerminals[temp] = 1;
			else
				terminals[temp] = 1;
		}
	}
}
void CFG::addStartSymbol(char ch) {
	this->startSymbol = ch;
	Follow[ch].insert('$');

}
bool CFG::isUpper(char ch) {
	return ch >= 'A' && ch <= 'Z';
}


//Implementation for Compute First and Follow
void CFG::ComputeFirst() {
	changedFirst = false;
	for (int i = 0; i < 256; i++) {
		vector<string> temp = productions[i];
		for (int j = 0; j < temp.size(); j++) {
			addFirst(i, j, 0);
		}
	}
	if (changedFirst)
		ComputeFirst();
}
void CFG::addFirst(int i, int j, int index) {
	if (index >= productions[i][j].size())
		return;
	else {
		int siz = First[i].size();
		char c = productions[i][j][index];
		if (!isUpper(c)) // if terminal add it to First[i]
			First[i].insert(c);
		else {
			set <char> uni, a = First[i], b = First[c];
			set_union(a.begin(), a.end(),
				b.begin(), b.end(),
				inserter(uni, uni.begin()));
			if (b.find(eps) != b.end()) {
				//uni.erase(eps); // if eps in union then erase it
				First[i] = uni;
				if (index == productions[i][j].size() - 1) //if A -> # then add eps to First
					First[i].insert(eps);
				else
					addFirst(i, j, index + 1);
			}
		}
		int siz2 = First[i].size();
		if (!changedFirst)
			changedFirst = siz != siz2;
	}
}
void CFG::ComputeFollow(){
	changedFollow=false;
	for(int i=0 ; i<256 ; i++){
		if(nonTerminals[i]){
			for(int j=0 ; j<256 ; j++){
				vector<string> temp = productions[j];
				for(int k=0 ; k<temp.size();k++){
					string currRule = temp[k];
					for(int v=0 ; v < currRule.size();v++){
						if(currRule[v]==i)
							addFollow(i , j , k , v+1);
						/*
                                v>> position of Non-Terminal
                                i>> position of productions 
                                    to LHS A--->Bc then i here 
                                    corresponding to A
                                j>> position of each production
                                k>> position of each char
                                */
					}
				}
			}
		
		}
	}
	if(changedFollow)
		ComputeFollow();
}
void CFG::addFollow(int v , int i , int j , int k ){
	//v is the Non-Terminal
	//i is the 
	int size1 = Follow[v].size();
	vector<string> temp = productions[i];
	string currRule = temp[j];

	//if A->bB then Follow(B) = Follow(A)
	if(k == currRule.length()){
		set<char> uni , a = Follow[v] , b= Follow[i];
		set_union(a.begin(),a.end() ,
				  b.begin(), b.end(),
				  inserter(uni , uni.begin()));
		Follow[v] = uni ;
		return;
	}
	if(k > currRule.length())
		return;
	char ch = currRule[k];

	//A -> bCa then Follow(C) = a
	if(!isUpper(ch)){
		Follow[v].insert(ch);
	}
	
	else{
		//A -> CB then Follow(C) = First(B)-{eps}
		set<char> uni , a=Follow[v] , b = First[ch];
		set_union(a.begin(), a.end(),
				  b.begin(), b.end(),
				  inserter(uni,uni.begin()));

		// A-> BC and find eps in first(C) then Follow(B) = Follow(A)
		if(b.find(eps) != b.end()){
			uni.erase(eps);
			Follow[v]=uni;
			if( k == currRule.length() -1){
				set<char> uni , a=Follow[v] , b = Follow[i];
				set_union(a.begin(), a.end(),
						  b.begin(), b.end(),
						  inserter(uni,uni.begin()));
				Follow[v]=uni;
			}
			else
				addFollow(v , i , j , k+1);
		
		}
	}
	int size2 = Follow[v].size();
	if(!changedFollow)
		changedFollow = size1 != size2; 
}


//Implementation for print Result 
void CFG::printFirst() {
	cout << "First Sets: \n";
	for (int i = 0; i < 256; i++) {
		set<char> temFirst = First[i];
		set<char>::iterator ite;
		if (productions[i].size()) {
			cout << char(i) << " : ";
			if (temFirst.size()) {
				for (ite = temFirst.begin(); ite != temFirst.end(); ite++)
					cout << *ite << "  ";
			}
			cout << endl;
		}

	}
}
void CFG::printFollow(){
	cout << "Follow Sets: \n";
	for (int i = 0; i < 256; i++) {
		set<char> temFollow = Follow[i];
		set<char>::iterator ite;
		if (productions[i].size()) {
			cout << char(i) << " : ";
			if (temFollow.size()) {
				for (ite = temFollow.begin(); ite != temFollow.end(); ite++)
					cout << *ite << "  ";
			}
			cout << endl;
		}

	}
}


int main() {
	CFG myGrammar;
	myGrammar.addStartSymbol('S');
	myGrammar.addProduction('S', "ACB|Cbb|Ba");
	myGrammar.addProduction('A', "da|BC");
	myGrammar.addProduction('B', "g|@");
	myGrammar.addProduction('C', "h|@");
	myGrammar.ComputeFirst();
	myGrammar.printFirst();
	myGrammar.ComputeFollow();
	myGrammar.printFollow();
	system("pause");
	return 0;
}