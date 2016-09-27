int bstkn;
bool bstk[10000];

class DATA{
	public:
		int addtme;
		int readtme;

		int readnum;
		bool encrypt;

		long long DATE;
		string FROM, TO;
		string SUBJECT, CONTENT;
		vector<string> fulltxt;
		
		unordered_set<string> APPEAR;

		void tokenize(string contxt){
			for(char &x : contxt){
				if(!(x >= '0' && x <= '9') && !(x >= 'a' && x <= 'z') && !(x >= 'A' && x <= 'Z')){
					x = ' ';
				}
				if(x >= 'A' && x <= 'Z') x = x - 'A' + 'a';
			}

			string token;
			istringstream iss (contxt);
			while(iss >> token) APPEAR.insert(token);
		}

		void createtrie(){
			tokenize(SUBJECT);
			if(!encrypt) tokenize(CONTENT);
		}

		bool appear(string keyw){
			return APPEAR.count(keyw);
		}

		bool alpnum(char c){
			return ('0' <= c && c <= '9') || ('a' <= c && c <= 'z');
		}

		bool valid(string &cond){ // Postfix! Ex: "kitty$momo$|", "validation$!"
			if(cond.size() == 0) return 1;

			bstkn = 0;

			for(int i = 0; i < (int)cond.size(); i ++){
				if(cond[i] == '!'){
					bstk[bstkn - 1] = !bstk[bstkn - 1];
				}
				else if(cond[i] == '&'){
					bstk[bstkn - 2] = bstk[bstkn - 2] && bstk[bstkn - 1];
					bstkn --;
				}
				else if(cond[i] == '|'){
					bstk[bstkn - 2] = bstk[bstkn - 2] || bstk[bstkn - 1];
					bstkn --;
				}
				else{
					string keyw;
					while(cond[i] != '$'){
						keyw.PB(cond[i]);
						i ++;
					}
					bstk[bstkn ++] = appear(keyw);
				}
			}

			return bstk[0];
		}
}storage[10010];
