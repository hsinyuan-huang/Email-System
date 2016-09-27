//By momo™
#include <cstdio>
#include <fstream>
#include <set>
#include <stack>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <cstring>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <algorithm>
#include "sha1.h"

#define PB push_back
#define MP make_pair
#define FOR(it, c) for(__typeof((c).begin())it=(c).begin();it!=(c).end();it++)

using namespace std;

#include "pagerank.h"
#include "dataST.h"
#include "dataST2.h"

#define PB push_back

unordered_map<string, string> mon2num = {
	{"January","01"}, {"February","02"}, {"March", "03"}, {"April", "04"}, {"May", "05"}, {"June", "06"},
	{"July", "07"}, {"August", "08"}, {"September", "09"}, {"October", "10"}, {"November", "11"}, 
	{"December", "12"}
};

bool alpnum(char c){
	return ('0' <= c && c <= '9') || ('a' <= c && c <= 'z');
}

int cstkn;
char cstk[10000];

inline int prior(char c){
	if(c == '!') return 4;
	else if(c == '&') return 3;
	else if(c == '|') return 2;
	else return 1;
}

void parsequery(string &keyexpr){
	string ori = keyexpr;
	keyexpr.clear();

	cstkn = 0;

	bool started = 0;

	for(int i = 0; i < (int)ori.size(); i++){
		if('A' <= ori[i] && ori[i] <= 'Z'){
			ori[i] = ori[i] - 'A' + 'a';
		}

		if(alpnum(ori[i])){
			started = 1;
			keyexpr.push_back(ori[i]);
		}
		else{
			if(started){
				keyexpr.push_back('$');
				started = 0;
			}

			if(ori[i] == '!'){
				cstk[cstkn ++] = '!';
			}
			else if(ori[i] == '&'){
				while(cstkn > 0 && prior(cstk[cstkn - 1]) >= 3){
					keyexpr.push_back(cstk[cstkn - 1]);
					cstkn --;
				}
				cstk[cstkn ++] = '&';
			}
			else if(ori[i] == '|'){
				while(cstkn > 0 && prior(cstk[cstkn - 1]) >= 2){
					keyexpr.push_back(cstk[cstkn - 1]);
					cstkn --;
				}
				cstk[cstkn ++] = '|';
			}
			else if(ori[i] == '('){
				cstk[cstkn ++] = '(';
			}
			else if(ori[i] == ')'){
				while(cstk[cstkn - 1] != '('){
					keyexpr.push_back(cstk[cstkn - 1]);
					cstkn --;
				}
				cstkn --;
			}
		}
	}

	if(started){
		keyexpr.push_back('$');
		started = 0;
	}

	while(cstkn > 0){
		keyexpr.push_back(cstk[cstkn - 1]);
		cstkn --;
	}
}

void ENCR(string &str, int l, int r, char cchar[]){
	SHA1 hasher;
	hasher << cchar;

	unsigned encode[10];
	hasher.Result(encode);

	char ccode[30];
	for(int i = 0; i < 5; i++) for(int j = 0; j < 4; j++)
		ccode[i * 4 + j] = ((encode[i] >> (8 * j)) & 255);
	ccode[20] = 0;

	if(r - l + 1 <= 20){
		for(int i = l; i <= r; i++)
			str[i] ^= ccode[i - l];
		return;
	}

	int mid = (l + r) / 2;
	ENCR(str, mid + 1, r, ccode + 10);
	ccode[10] = 0;
	ENCR(str, l, mid, ccode);
}

int vstMID[20000], MIDn;

int rmtype = 0;
int opttme = 0;

stack<int> removed;

int main (){
	char inst[10];
	while(~scanf("%s", inst)){
		opttme ++;

		// undo
		if(inst[0] == 'u'){
			if(!removed.empty()){
				int MID = removed.top(); removed.pop();

				if(vstMID[MID]){
					printf("-\n");
					continue;
				}
				vstMID[MID] = 1; MIDn ++;

				mailDS.add(MID);
				printf("%d\n", MIDn);
			}
			else printf("No remove operations before\n");
		}

		// set
		if(inst[0] == 's'){
			scanf("%d", &rmtype);
		}

		// add
		if(inst[0] == 'a'){
			char mail[200];
			scanf("%s", mail);

			bool encrypt = 0;
			if(strcmp(mail, "-encrypt") == 0){ // If add operation is invalid, encrypt or not don't matter
				encrypt = 1;
				scanf("%s", mail);
			}

			ifstream fin;
			fin.open(mail);

			string useless;

			//===============

			string FROM;
			fin >> useless >> FROM;
			for(char &x : FROM){
				if(x >= 'A' && x <= 'Z') x = x - 'A' + 'a';
			}

			//===============

			long long DATE;
			string year, month, day, clok;
			fin >> useless >> day >> month >> year;
			fin >> useless >> clok; clok = clok.substr(0, 2) + clok.substr(3, 2);
			if((int)day.size() == 1) day = "0" + day;
			DATE = stoll(year + mon2num[month] + day + clok);

			//===============

			int MID;
			fin >> useless >> MID;

			if(vstMID[MID]){
				printf("-\n");
				continue;
			}
			vstMID[MID] = 1; MIDn ++;

			//===============

			fin >> useless;
			getline(fin, storage[MID].SUBJECT);

			//===============

			string TO;
			fin >> useless >> TO;
			for(char &x : TO){
				if(x >= 'A' && x <= 'Z') x = x - 'A' + 'a';
			}

			//===============

			string contxt, word;
			fin >> useless;
			getline(fin, storage[MID].CONTENT);
			getline(fin, word); storage[MID].CONTENT.append(word);
			getline(fin, word); storage[MID].CONTENT.append(" " + word);

			fin.close();
			//===============

			FROM.PB('$');
			TO.PB('$');

			storage[MID].addtme = opttme;
			storage[MID].readtme = opttme;
			storage[MID].readnum = 0;
			storage[MID].encrypt = encrypt;

			storage[MID].FROM = FROM;
			storage[MID].DATE = DATE;
			storage[MID].TO = TO;

			//Inserted SUBJECT
			//Inserted CONTENT

			string A; fin.open(mail);
			storage[MID].fulltxt.clear();

			getline(fin, A);
			storage[MID].fulltxt.PB(A);

			getline(fin, A);
			storage[MID].fulltxt.PB(A);

			getline(fin, A);
			storage[MID].fulltxt.PB(A);

			getline(fin, A);
			storage[MID].fulltxt.PB(A);

			getline(fin, A);
			storage[MID].fulltxt.PB(A);

			fin.close();

			// Using SHA1 to create butterfly effect
			if(storage[MID].encrypt){
				printf("Please Enter Your Password (a nonempty string consists of alpha-num): ");

				char password[3000];
				scanf("%s", password);

				SHA1 hasher;
				hasher << password;

				unsigned encode[10];
				hasher.Result(encode);

				char ccode[30];
				for(int i = 0; i < 5; i++) for(int j = 0; j < 4; j++)
					ccode[i * 4 + j] = ((encode[i] >> (8 * j)) & 255);
				ccode[20] = 0;

				ENCR(storage[MID].CONTENT, 0, storage[MID].CONTENT.size() - 1, ccode);
			}

			storage[MID].createtrie();
			mailDS.add(MID);

			printf("%d\n", MIDn);
		}

		// remove
		if(inst[0] == 'r'){
			int MID;
			scanf("%d", &MID);

			if(MID >= 20000 || !vstMID[MID]){
				printf("-\n");
				continue;
			}

			char opt[10];
			opt[0] = 'y';

			if(rmtype == 1 && mailDS.important(MID)){
				printf("This may be an important mail. Are you sure you want to remove this mail? (y/n): ");
				scanf("%s", opt);
			}
			if(rmtype == 2){
				printf("Are you sure you want to remove this mail? (y/n): ");
				scanf("%s", opt);
			}

			if(opt[0] == 'y'){
				vstMID[MID] = 0; MIDn --;
				mailDS.remove(MID);
			}

			printf("%d\n", MIDn);
			removed.push(MID);
		}

		// print
		if(inst[0] == 'p'){
			int MID;
			scanf("%d", &MID);

			if(MID >= 20000 || !vstMID[MID]){
				printf("-\n");
				continue;
			}

			char password[3000];
			if(storage[MID].encrypt){
				printf("This is an encrypted mail, please enter your password: ");
				scanf("%s", password);
			}

			printf("========================\n");
			printf("%s\n", storage[MID].fulltxt[0].c_str());
			printf("%s\n", storage[MID].fulltxt[1].c_str());
			printf("%s\n", storage[MID].fulltxt[2].c_str());
			printf("%s\n", storage[MID].fulltxt[3].c_str());
			printf("%s\n", storage[MID].fulltxt[4].c_str());
			printf("Content:\n");

			if(storage[MID].encrypt){
				string CONTENT = storage[MID].CONTENT;

				SHA1 hasher;
				hasher << password;

				unsigned encode[10];
				hasher.Result(encode);

				char ccode[30];
				for(int i = 0; i < 5; i++) for(int j = 0; j < 4; j++)
					ccode[i * 4 + j] = ((encode[i] >> (8 * j)) & 255);
				ccode[20] = 0;

				ENCR(CONTENT, 0, CONTENT.size() - 1, ccode);

				bool badpass = 0;
				for(int i = 0; i < CONTENT.size(); i++){
					if(CONTENT[i] < 32 || CONTENT[i] > 126){
						printf("Decrypted Text is Not Printable\n");
						badpass = 1;
						break;
					}
				}

				if(!badpass) printf("%s\n", CONTENT.c_str());
			}
			else printf("%s\n", storage[MID].CONTENT.c_str());

			printf("========================\n");

			storage[MID].readnum ++;
			storage[MID].readtme = opttme;
		}

		// query
		if(inst[0] == 'q'){
			string FROM, TO;

			bool no_key = 0;
			int sortype = 0;

			long long date1 = 0, date2 = 999999999999;
			string keyexpr;

			char str[3000];
			scanf("%s", str);

			while(str[0] == '-'){
				if(str[1] == 'f'){ // -f"MOMO"
					for(int i = 3; str[i + 1]; i++){
						char x = str[i];
						if(x >= 'A' && x <= 'Z') x = x - 'A' + 'a';

						FROM.PB(x);
					}
				}

				//===============

				if(str[1] == 't'){ // -t"MOMO"
					for(int i = 3; str[i + 1]; i++){
						char x = str[i];
						if(x >= 'A' && x <= 'Z') x = x - 'A' + 'a';

						TO.PB(x);
					}
				}

				//===============				

				if(str[1] == 'd'){
					if(str[2] == 'a'){ // -dat
						sortype = 1;
					}
					//===============
					else{
						string dstr = string(str + 2); // -d~

						if(dstr[0] != '~'){
							date1 = stoll(dstr);
							dstr = string(str + 15);
						}
						else dstr = string(str + 3);

						if(dstr[0] != 0){
							date2 = stoll(dstr);
						}
					}
				}

				//===============

				if(str[1] == 'r'){ // -rec
					sortype = 2;
				}

				//===============

				if(str[1] == '*'){ // -*rec
					sortype = 3;
				}

				//===============

				if(str[1] == 'i'){ // -imp
					sortype = 4;
				}

				//===============

				if(str[1] == 'x'){ // -x
					no_key = 1;
					break;
				}

				//===============

				scanf("%s", str);
			}

			FROM.PB('$');
			TO.PB('$');

			if(!no_key){
				keyexpr = string(str);
				parsequery(keyexpr);
			}

			mailDS.query(sortype, FROM, TO, date1, date2, keyexpr);
		}
	}
}
