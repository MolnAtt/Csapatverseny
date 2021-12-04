#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <string>

using namespace std;

#include <algorithm>

class Graf
{
public:
	Graf()
	{
		int tempM;
		cin >> N >> tempM;
		csucslista = new vector<vector<int>>(N+1, vector<int>());
		for (int i = 1; i <= tempM; i++)
		{
			int honnan;
			int hova;
			cin >> honnan >> hova;
			osszekot(honnan, hova);
		}
	}
	void diagnosztika()
	{
		for (int i = 1; i <= N; i++)
		{
			cerr << i << ": [ ";
			for (auto& szomszed : csucslista->at(i))
				cerr << szomszed << " ";
			cerr << "]\n";
		}
	}
	friend std::ostream& operator<<(std::ostream& stream, const Graf& g)
	{
		stream << "\n graphvizbe: \n ";
		for (int i = 0; i < g.N; i++)
			if (0 == g.csucslista->at(i).size())
				stream << i << ";\n";
			else
				for (int j = 0; j < g.csucslista->at(i).size(); j++)
					stream << i << " -> " << g.csucslista->at(i)[j] << ";\n";
		return stream;
	}
	bool van_el(const int a, const int b) { return vektorbeli_helye(csucslista->at(a), b) != csucslista->at(a).size(); }
	int vektorbeli_helye(const vector<int>& v, const int b) { int i = 0; while (i < v.size() && v[i] != b) i++; return i; }
	void osszekot(int a, int b) { csucslista->at(a).push_back(b); M++; }
	void remove_edge(const int a, const int b)
	{
		pair<int, int> ittvannak = csucslistabeli_helye(a, b);
		if (ittvannak.first < csucslista->at(a).size())
		{
			remove_vektorbol(csucslista->at(a), ittvannak.first);
			M--;
		}
		else
			cerr << "nincs is ilyen el!";
	}
	void add_node() { csucslista->push_back(vector<int>()); N++; };
	int fok(int a) { return csucslista->at(a).size(); }
	bool hurok(int a) { return vektorbeli_helye(csucslista->at(a), a) < csucslista->at(a).size(); }
	bool izolalt(int a) { return 0 == fok(a); }
	int find_melysegi(int csucs, bool (*predicate)(int))
	{
		vector<string> szin(N, "feher");
		stack<int> tennivalok;
		tennivalok.push(csucs);
		while (!tennivalok.empty())
		{
			int tennivalo = tennivalok.top();
			tennivalok.pop();

			if (predicate(tennivalo))
				return tennivalo;
			szin[tennivalo] = "fekete";

			for (int& szomszed : csucslista->at(tennivalo))
			{
				if (szin[szomszed] == "feher")
				{
					tennivalok.push(szomszed);
					szin[szomszed] = "szurke";
				}
			}
		}

		return -1;
	};
	int find_szelessegi(int csucs, bool (*predicate)(int))
	{
		vector<string> szin(N, "feher");
		queue<int> tennivalok;
		tennivalok.push(csucs);

		while (!tennivalok.empty())
		{
			int tennivalo = tennivalok.front();
			tennivalok.pop();
			if (predicate(tennivalo))
				return tennivalo;
			szin[tennivalo] = "fekete";

			for (int& szomszed : csucslista->at(tennivalo))
			{
				if (szin[szomszed] == "feher")
				{
					tennivalok.push(szomszed);
					szin[szomszed] = "szurke";
				}
			}
		}

		return -1;
	};
	int tavolsag(int csucs, bool (*predicate)(int))
	{
		vector<int> dist(N, -1);
		dist[csucs] = 0; 
		vector<string> szin(N, "feher");
		queue<int> tennivalok;
		tennivalok.push(csucs);

		while (!tennivalok.empty())
		{
			int tennivalo = tennivalok.front();
			tennivalok.pop();
			if (predicate(tennivalo)) 
				return dist[tennivalo];
			szin[tennivalo] = "fekete";

			for (int& szomszed : csucslista->at(tennivalo))
			{
				if (szin[szomszed] == "feher")
				{
					tennivalok.push(szomszed);
					dist[szomszed] = dist[tennivalo] + 1;
					szin[szomszed] = "szurke";
				}
			}
		}

		return -1;
	};
	vector<int> utvonal(int end, const vector<int>& honnan)
	{
		stack<int> verem;
		while (honnan[end] != -2)
		{
			verem.push(end);
			end = honnan[end];
		}
		vector<int> result;
		while (!verem.empty())
		{
			result.push_back(verem.top());
			verem.pop();
		}
		return result;
	}
	vector<int> legrovidebb_ut(int csucs, bool (*predicate)(int))
	{
		vector<int> honnan(N, -1); // Amikor besz�rk�lt, HONNAN sz�rk�tett�k be a szelessegi bejaras;
		vector<string> szin(N, "feher");
		queue<int> tennivalok;
		tennivalok.push(csucs);

		while (!tennivalok.empty())
		{
			int tennivalo = tennivalok.front();
			tennivalok.pop();
			if (predicate(tennivalo)) // feldolgoz�s = feket�re sz�nez�s
				return utvonal(tennivalo, honnan);
			szin[tennivalo] = "fekete";

			for (int& szomszed : csucslista->at(tennivalo))
			{
				if (szin[szomszed] == "feher")
				{
					tennivalok.push(szomszed);
					honnan[szomszed] = tennivalo; // �tvonalvektor
					szin[szomszed] = "szurke";
				}
			}
		}

		return vector<int>();
	};
	int pont_generalt_reszgraf_szamossaga(int csucs, bool (*predicate)(int))
	{
		int db = 0;
		vector<string> szin(N, "feher");
		stack<int> tennivalok;
		tennivalok.push(csucs);

		while (!tennivalok.empty())
		{
			int tennivalo = tennivalok.top();
			tennivalok.pop();
			if (predicate(tennivalo))
				db++;
			szin[tennivalo] = "fekete";

			for (int& szomszed : csucslista->at(tennivalo))
			{
				if (szin[szomszed] == "feher")
				{
					tennivalok.push(szomszed);
					szin[szomszed] = "szurke";
				}
			}
		}
		return db;
	}
	vector<int> kivalogatas(int csucs, bool (*predicate)(int))
	{
		vector<int> result;
		vector<string> szin(N, "feher");
		stack<int> tennivalok;
		tennivalok.push(csucs);

		while (!tennivalok.empty())
		{
			int tennivalo = tennivalok.top();
			tennivalok.pop();
			if (predicate(tennivalo))
				result.push_back(tennivalo);
			szin[tennivalo] = "fekete";

			for (int& szomszed : csucslista->at(tennivalo))
			{
				if (szin[szomszed] == "feher")
				{
					tennivalok.push(szomszed);
					szin[szomszed] = "szurke";
				}
			}
		}
		return result;
	}
	bool osszefuggo() { return N = 0 || (N == pont_generalt_reszgraf_szamossaga(0, [](int x) {return x == x; })); }

private:
	int N, M;
	vector<vector<int>>* csucslista;
	void remove_vektorbol(vector<int>& v, int i) { v.erase(v.begin() + i); }
	pair<int, int> csucslistabeli_helye(int a, int b)
	{
		int i = 0; while (i < csucslista->at(a).size() && csucslista->at(a)[i] != b) { i++; }
		int j = 0; while (j < csucslista->at(b).size() && csucslista->at(b)[j] != a) { j++; }		
		return pair<int, int> { i, j };
	}
};

int main()
{
	Graf graf;
	graf.diagnosztika();
	cerr << graf;
}


/*
tesztinput
6 7
1 2
1 3
3 1
2 4
4 1
5 2
5 3

*/
