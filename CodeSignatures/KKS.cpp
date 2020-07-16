#define _CRT_SECURE_NO_WARNINGS 
#include "KKS.h"
using namespace std;
KKS::KKS() {

}
void KKS::init() {
	ifstream inconfig(".config");
	string outFile;
	int randSeed;
	inconfig >> outFile >> n >> randSeed;
	inconfig.close();
	ofstream oflog(".log");
	if (randSeed < 0) {
		randSeed = time(0);
	}
	srand(randSeed);
	oflog << "srand = " << randSeed << "\n";
	if (outFile[0] != '-') {
		freopen(outFile.c_str(), "w", stdout);
	}
	oflog.close();
}
void KKS::generate_pair(string filename, Crypto_public_key** pub_k, Crypto_private_key** priv_k) {
	ifstream info(filename);
	int t;
	unsigned int poly;
	int n;

	info >> t >> poly;
	Matrix<Galois> g(1, t + 1);

	Galois::stash_global_poly(poly);
	for (int i = 0; i <= t; i++) {
		info >> g(0, i);
	}

	info >> n;
	vector<int> L_nums(n);
	vector<Galois> L(n);
	for (int i = 0; i < n; i++) {
		info >> L_nums[i];
		if (L_nums[i] == -1) {
			L[i] = Galois(0, poly);
		}
		else {
			L[i] = Galois(1, poly);
			for (int j = 0; j < L_nums[i]; j++) {
				L[i] *= Galois(2, poly);
			}
		}
	}

	info.close();

	Matrix<Galois> X(t, t);
	Matrix<Galois> Y(t + 1, n);
	Matrix<Galois> Z(n, n);

	for (int i = 0; i < t; i++) {
		for (int j = 0; j < t; j++) {
			X(i, j) = (i + j < t) ? g(0, i + j + 1) : Galois(0, poly);
		}
	}

	for (int j = 0; j < n; j++) {
		Y(0, j) = Galois(1, poly);
	}
	for (int i = 1; i <= t; i++) {
		for (int j = 0; j < n; j++) {
			Y(i, j) = Y(i - 1, j) * L[j];
		}
	}
	Matrix<Galois> h = g * Y;

	for (int i = 0; i < n; i++) {
		Z(i, i) = h(0, i).inv();
	}

	Matrix<Galois> H = X * Y.submatrix(0, t, 0, n) * Z;

	Galois::unstash_global_poly();
	Galois::stash_global_poly(3);

	int m = 0;
	while (poly >> m) {
		m++;
	}
	m--;
	Matrix<Galois> G(m * t, n);
	for (int i = 0; i < t; i++) {
		for (int j = 0; j < n; j++) {
			unsigned int val = H(i, j).get_val();
			for (int k = 0; k < m; k++) {
				G(i * m + k, j) = Galois(((val >> k) & 1), 3);
			}
		}
	}
	G = G.repair();
	G.inv();
	G = G.transpose();
	G = G.submatrix(t * m, n, 0, n);
	G = Matrix<Galois>::randInvertible(n - t * m) * G;

	*pub_k = new Goppa_public(t, G);

	int k = n - m * t;

	G = G.transpose();
	G.hconcat(Matrix<Galois>(n));
	G.eliminate(false, k);
	G = G.submatrix(0, k, k, k + n).transpose();

	*priv_k = new Goppa_private(poly, g, H, Y, G, L_nums);

	Galois::unstash_global_poly();
	this->n = n;
	this->t1 = t;
	this->k = k;
	this->r = n - k;
	this->H = H;
}
void KKS::subcode_generate_pair(string filename, Crypto_public_key** pub_k, Crypto_private_key** priv_k) {
	ifstream info(filename);
	int t;
	unsigned int poly;
	int n;

	info >> t >> poly;
	Matrix<Galois> g(1, t + 1);

	Galois::stash_global_poly(poly);
	for (int i = 0; i <= t; i++) {
		info >> g(0, i);
	}

	info >> n;
	vector<int> L_nums(n);
	vector<Galois> L(n);
	for (int i = 0; i < n; i++) {
		info >> L_nums[i];
		if (L_nums[i] == -1) {
			L[i] = Galois(0, poly);
		}
		else {
			L[i] = Galois(1, poly);
			for (int j = 0; j < L_nums[i]; j++) {
				L[i] *= Galois(2, poly);
			}
		}
	}

	info.close();

	Matrix<Galois> X(t, t);
	Matrix<Galois> Y(t + 1, n);
	Matrix<Galois> Z(n, n);

	for (int i = 0; i < t; i++) {
		for (int j = 0; j < t; j++) {
			X(i, j) = (i + j < t) ? g(0, i + j + 1) : Galois(0, poly);
		}
	}

	for (int j = 0; j < n; j++) {
		Y(0, j) = Galois(1, poly);
	}
	for (int i = 1; i <= t; i++) {
		for (int j = 0; j < n; j++) {
			Y(i, j) = Y(i - 1, j) * L[j];
		}
	}
	Matrix<Galois> h = g * Y;

	for (int i = 0; i < n; i++) {
		Z(i, i) = h(0, i).inv();
	}

	Matrix<Galois> H = X * Y.submatrix(0, t, 0, n) * Z;

	Galois::unstash_global_poly();
	Galois::stash_global_poly(3);

	int m = 0;
	while (poly >> m) {
		m++;
	}
	m--;
	Matrix<Galois> G(m * t, n);
	for (int i = 0; i < t; i++) {
		for (int j = 0; j < n; j++) {
			unsigned int val = H(i, j).get_val();
			for (int k = 0; k < m; k++) {
				G(i * m + k, j) = Galois(((val >> k) & 1), 3);
			}
		}
	}
	G = G.repair();
	G.inv();
	G = G.transpose();
	G = G.submatrix(t * m, n, 0, n);
	G = Matrix<Galois>::randInvertible(n - t * m) * G;

	*pub_k = new Goppa_public(t, G);

	int k = n - m * t;

	G = G.transpose();
	G.hconcat(Matrix<Galois>(n));
	G.eliminate(false, k);
	G = G.submatrix(0, k, k, k + n).transpose();

	*priv_k = new Goppa_private(poly, g, H, Y, G, L_nums);

	Galois::unstash_global_poly();
	this->ns = n;
	this->t2 = t;
	this->G = G;
}
void KKS::generateKey() {
	random_device rd;
	mt19937 gen(rd());
	init();
	generate_pair(".goppa01", (Crypto_public_key**)&public_key, (Crypto_private_key**)&private_key);
	subcode_generate_pair("a.goppa02", (Crypto_public_key**)&public_key, (Crypto_private_key**)&private_key);
	uniform_int_distribution<> dis(0, n - 1);
	vector<int> used;
	for (int i = 0; J.size() < ns; i++) {
		int val = dis(gen);
		if (val < n) {
			bool flag = true;
			for (int j = 0; j < used.size(); j++) {
				if (val == used[j]) {
					flag = false;
					break;
				}
			}
			if (flag) {
				J.push_back(val);
				used.push_back(val);
			}
		}
	}
	sort(J.begin(), J.end());
	HJ = Matrix<Galois>(H.size(0), ns);
	for (int i = 0; i < ns; i++) {
		for (int j = 0; j < H.size(0); j++) {
			HJ(j, i) = H(j, J[i]);
		}
	}
	G = G.transpose();
	F = HJ * G.transpose();
}
mgal KKS::str_to_mgal(string& s) {
	int k = (int)s.size() * 8;
	mgal x(1, k);
	for (int i = 0; i < k; i++) {
		x(0, i) = Galois(((s[i / 8] >> (i % 8)) & 1), 3);
	}
	return x;
}
string KKS::mgal_to_str(mgal& x) {
	stringstream ss;
	char c = 0;
	int k = x.size(1);
	for (int i = 0; i < k; ++i) {
		c |= (x(0, i).isZero() ? 0 : 1) << (i % 8);
		if ((i + 1) % 8 == 0) {
			ss << c;
			c = 0;
		}
	}
	if (k % 8) {
		ss << c;
	}
	return ss.str();
}
void KKS::load(const string& filename) {
	ifstream fin(filename, ios_base::in);
	fin.exceptions(ios_base::badbit | ios_base::failbit);
	fin.seekg(ios_base::beg);
	getline(fin, message);
	fin.close();
}
void KKS::save(const string& filename) {
	ofstream fout(filename, ios_base::out);
	fout.exceptions(ios_base::badbit | ios_base::failbit);
	fout.seekp(ios_base::beg);
	fout << output;
	fout.close();
}
void KKS::sign() {
	sigmaStar = str_to_mgal(message) * G;
	sigma = Matrix<Galois>(1, n);
	for (int i = 0; i < n; i++) {
		sigma(0, i) = 0;
	}
	for (int i = 0; i < ns; i++) {
		sigma(0, J[i]) = sigmaStar(0, J[i]);
	}
	output = mgal_to_str(sigma);
}
bool KKS::verify() {
	bool condition1 = false;
	mgal product1 = H * sigma.transpose();
	mgal product2 = F * str_to_mgal(message);
	condition1 = (product1 == product2);
	bool condition2 = false;
	int weight = 0;
	for (int i = 0; i < n; i++) {
		if (sigma(0, i) != 0) {
			weight++;
		}
	}
	condition2 = (weight >= t1) && (weight <= t2);
	return condition1 & condition2;
}
KKS::~KKS() {

}