#define _CRT_SECURE_NO_WARNINGS
#include "CFS.h"
using namespace std;
void CFS::init() {
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
CFS::CFS() {

}
void CFS::generateKey() {
	init();
	generate_pair(".goppa01", (Crypto_public_key**)&public_key, (Crypto_private_key**)&private_key);
	Q = Matrix<Galois>::randInvertible(n - k);
	P = Matrix<Galois>::randInvertible(n);
	H = Q * H;
	H *= P;
}
mgal CFS::hash(mgal& m, bool isRandom) {
	mgal outHash(1, n - k);
	if (isRandom) {
		random_device rd;
		mt19937 gen(rd());
		uniform_real_distribution<> dis(0, 1);
		for (int i = 0; i < (n - k); i++) {
			double val = dis(gen);
			if (val >= 0.5) {
				outHash(0, i) = 1;
			}
			else {
				outHash(0, i) = 0;
			}
		}
	}
	return outHash;
}
mgal CFS::str_to_mgal(string& s) {
	int k = (int)s.size() * 8;
	mgal x(1, k);
	for (int i = 0; i < k; i++) {
		x(0, i) = Galois(((s[i / 8] >> (i % 8)) & 1), 3);
	}
	return x;
}
string CFS::mgal_to_str(mgal& x) {
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
void CFS::load(const string& filename) {
	ifstream fin(filename, ios_base::in);
	fin.exceptions(ios_base::badbit | ios_base::failbit);
	fin.seekg(ios_base::beg);
	getline(fin, message);
	fin.close();
}
void CFS::save(const string& filename) {
	ofstream fout(filename, ios_base::out);
	fout.exceptions(ios_base::badbit | ios_base::failbit);
	fout.seekp(ios_base::beg);
	fout << output;
	fout.close();
}
void CFS::sign() {
	mgal temp = str_to_mgal(message);
	string str;
	int value = 0;
	str.push_back(value);
	i = str_to_mgal(str);
	Matrix<Galois> Qinv = Q;
	Qinv.safe_inv();
	mgal h = hash(temp, true);
	int position = 0;
	while (true) {
		mgal util = h;
		util.hconcat(i);
		xs = private_key->decode(Qinv * hash(util, true));
		value++;
		if (value > 127) {
			str[position] = 127;
			str.push_back(value - 127);
			position++;
		}
		else {
			str[position] = value;
		}
		i = str_to_mgal(str);
	}
	output = mgal_to_str(i);
	Matrix<Galois> out2 = xs * P;
	output += mgal_to_str(out2);
}
bool CFS::verify() {
	Matrix<Galois> syndrom1 = H * xs.transpose();
	mgal h = str_to_mgal(message);
	h = hash(h, true);
	h.hconcat(i);
	Matrix<Galois> syndrom2 = hash(h, true);
	return syndrom1 == syndrom2;
}
CFS::~CFS() {

}
void CFS::generate_pair(string filename, Crypto_public_key** pub_k, Crypto_private_key** priv_k) {
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
	this->t = t;
	this->k = k;
	this->H = H;
}
void CFS::loadBCHTable(const string& filename) {
	ifstream fin(filename, ios_base::in);
	fin.exceptions(ios_base::badbit | ios_base::failbit);
	fin.seekg(ios_base::beg);
	BCHTable.resize(4);
	BCHTable[0].resize(27);
	BCHTable[1].resize(58);
	BCHTable[2].resize(121);
	BCHTable[3].resize(503);
	string str;
	while (!fin.eof()) {
		getline(fin, str);
		stringstream ss;
		ss << str;
		int val1 = 0;
		int val2 = 0;
		int val3 = 0;
		ss >> val1 >> val2 >> val3;
		switch (val1) {
		case 31:
			BCHTable[0][val2] = val3;
			break;
		case 63:
			BCHTable[1][val2] = val3;
			break;
		case 127:
			BCHTable[2][val2] = val3;
			break;
		case 511:
			BCHTable[3][val2] = val3;
			break;
		default:
			break;
		}
	}
	fin.close();
}
