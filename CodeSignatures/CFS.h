#ifndef CFS_H
#define CFS_H
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <random>
#include <ctime>
#include <cmath>
#include <string>
#include <algorithm>
#include <thread>
#include "galois.hpp"
#include "matrix.hpp"
#include "crypto.hpp"
#include "bch.hpp"
#include "goppa.hpp"
#include "mceliece.hpp"
#include "simple_construction.hpp"
#include "diagonal_construction.hpp"
using namespace galois;
using namespace matrix;
using namespace crypto;
using namespace goppa;
using namespace mceliece;
using namespace simple_construction;
using namespace diagonal_construction;
typedef Galois gal;
typedef Matrix<gal> mgal;
class CFS {
private:
	int n;
	int k;
	int t;
	std::string message;
	std::string output;
	Matrix<Galois> H;
	Matrix<Galois> Q;
	Matrix<Galois> P;
	Matrix<Galois> xs;
	mgal i;
	Goppa_public* public_key;
	Goppa_private* private_key;
	void init();
	mgal hash(mgal& m, bool isRandom);
	void generate_pair(std::string filename, Crypto_public_key** pub_k, Crypto_private_key** priv_k);
	mgal str_to_mgal(std::string& s);
	std::string mgal_to_str(mgal& x);
	std::vector<std::vector<int>> BCHTable;
	void loadBCHTable(const std::string& filename);
public:
	CFS();
	void load(const std::string& filename);
	void save(const std::string& filename);
	void generateKey();
	void sign();
	bool verify();
	~CFS();
};
#endif