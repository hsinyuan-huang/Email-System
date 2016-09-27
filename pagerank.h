double pagerank[1000];
bool compG(int a, int b){
	return pagerank[a] > pagerank[b];
}

class social_graph{
	public:
		int peonum = 0;
		int edge = 0;

		int rank[1000]; //該位為第幾名
		int ranking[1000]; //第一名到...

		int diff = 0;
		int ori_edge = 0;

		unordered_map<string, int> nme2id;
		unordered_set<int> Gr[1000];

		double PR(string &A){
			return pagerank[nme2id[A]];
		}

		void insert(string &A, string &B){
			if(!nme2id.count(A)) nme2id[A] = peonum ++;
			int idA = nme2id[A];
			if(!nme2id.count(B)) nme2id[B] = peonum ++;
			int idB = nme2id[B];

			Gr[idA].insert(idB);

			diff ++;
			edge ++;
			if(diff >= ori_edge / 5){
				diff = 0;
				ori_edge = edge;

				poweriter();
			}
		}

		void erase(string &A, string &B){
			int idA = nme2id[A];
			int idB = nme2id[B];

			Gr[idA].erase(idB);

			diff ++;
			edge --;
			if(diff >= ori_edge / 5){
				diff = 0;
				ori_edge = edge;

				poweriter();
			}
		}

		bool near_center(string &A){
			int idA = nme2id[A];
			return rank[idA] <= peonum / 5;
		}

		void poweriter(){
			if(peonum == 0) return;

			for(int i = 0; i < peonum; i++) pagerank[i] = 1.0;

			for(int t = 0; t < 100; t++){
				double tmp[1000];
				for(int i = 0; i < peonum; i++){
					tmp[i] = pagerank[i];
					pagerank[i] = 0;
				}

				double addi = 0;
				for(int i = 0; i < peonum; i++){
					if(Gr[i].size() == 0) addi += tmp[i];
					else FOR(it, Gr[i]) pagerank[*it] += tmp[i] / Gr[i].size();
				}

				for(int i = 0; i < peonum; i++){
					pagerank[i] += addi / peonum;
				}
			}

			for(int i = 0; i < peonum; i++) ranking[i] = i;
			sort(ranking, ranking + peonum, compG);

			for(int i = 0; i < peonum; i++)
				rank[ranking[i]] = i;
		}
}gr;
