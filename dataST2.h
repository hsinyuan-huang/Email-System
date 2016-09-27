struct datecmp{
	bool operator() (const int& lhs, const int& rhs) const{
		if(storage[lhs].DATE != storage[rhs].DATE)
			return storage[lhs].DATE < storage[rhs].DATE;
		return lhs < rhs;
	}
};

bool comp(const int &iA, const long long &tB){
	return storage[iA].DATE < tB;
}

bool compdat(int A, int B){
	if(storage[A].DATE != storage[B].DATE)
		return storage[A].DATE > storage[B].DATE;
	return A < B;
}

bool comprec(int A, int B){
	if(storage[A].addtme != storage[B].addtme)
		return storage[A].addtme > storage[B].addtme;
	return A < B;
}

bool compprec(int A, int B){
	if(storage[A].readtme != storage[B].readtme)
		return storage[A].readtme > storage[B].readtme;
	return A < B;
}

bool compimp(int A, int B){
	double v1 = gr.PR(storage[A].FROM) + gr.PR(storage[A].TO);
	double v2 = gr.PR(storage[B].FROM) + gr.PR(storage[B].TO);
	if(v1 != v2)
		return v1 > v2;
	if(storage[A].readnum != storage[B].readnum)
		return storage[A].readnum > storage[B].readnum;
	if(storage[A].addtme != storage[B].addtme)
		return storage[A].addtme > storage[B].addtme;
	return A < B;
}

class DATAS{
	public:
		unordered_map<string, set<int, datecmp> > in_MID;

		void add(int MID){
			gr.insert(storage[MID].FROM, storage[MID].TO);

			in_MID[storage[MID].FROM + storage[MID].TO].insert(MID);
			in_MID[storage[MID].FROM + "$"].insert(MID);
			in_MID["$" + storage[MID].TO].insert(MID);
			in_MID["$$"].insert(MID);
		}

		void remove(int MID){
			gr.erase(storage[MID].FROM, storage[MID].TO);

			in_MID[storage[MID].FROM + storage[MID].TO].erase(MID);
			in_MID[storage[MID].FROM + "$"].erase(MID);
			in_MID["$" + storage[MID].TO].erase(MID);
			in_MID["$$"].erase(MID);
		}

		bool important(int MID){
			return gr.near_center(storage[MID].FROM) || storage[MID].encrypt;
		}

		void query(int sortype, string &FROM, string &TO, long long &date1, long long &date2, string &cond){
			vector<int> ans;

			set<int, datecmp> &cand = in_MID[FROM + TO];
			set<int, datecmp>::iterator it = lower_bound(cand.begin(), cand.end(), date1, comp);

			for(; it != cand.end(); it ++){
				DATA &x = storage[*it];
				if(x.DATE > date2) break;

				if(x.valid(cond)) ans.push_back(*it);
			}

			if(ans.empty()){
				printf("-\n");
				return;
			}

			if(sortype == 0) sort(ans.begin(), ans.end());
			if(sortype == 1) sort(ans.begin(), ans.end(), compdat);
			if(sortype == 2) sort(ans.begin(), ans.end(), comprec);
			if(sortype == 3) sort(ans.begin(), ans.end(), compprec);
			if(sortype == 4) sort(ans.begin(), ans.end(), compimp);

			bool first = 1;
			for(int &x : ans){
				if(!first) printf(" ");

				first = 0;
				printf("%d", x);
			}
			puts("");
		}
}mailDS;
