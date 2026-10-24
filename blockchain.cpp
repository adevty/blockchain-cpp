#include <iostream>
#include <sstream>
#include <ctime>
#include <chrono>
#include <string>
#include <vector>
#include <iomanip>
#include <openssl/evp.h>

using namespace std;

string calc_sha256(const string& str) {
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int lengthOfHash = 0;

    EVP_MD_CTX* context = EVP_MD_CTX_new();
    if (context != nullptr) {
        if (EVP_DigestInit_ex(context, EVP_sha256(), nullptr)) {
            if (EVP_DigestUpdate(context, str.c_str(), str.size())) {
                EVP_DigestFinal_ex(context, hash, &lengthOfHash);
            }
        }
        EVP_MD_CTX_free(context);
    }

    stringstream ss;
    for (unsigned int i = 0; i < lengthOfHash; ++i) {
        ss << hex << setw(2) << setfill('0') << static_cast<int>(hash[i]);
    }
    return ss.str();
}

string get_current_time() {
    auto now = chrono::system_clock::now();
    time_t now_c = chrono::system_clock::to_time_t(now);
    stringstream ss;
    ss << put_time(localtime(&now_c), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

struct Transaction {
    string sender;
    string receiver;
    double amount;
    string timestamp;

    Transaction(const string& s, const string& r, double a)
        : sender(s), receiver(r), amount(a), timestamp(get_current_time()) {}

    string to_string_hash() const {
        stringstream ss;
        ss << sender << receiver << fixed <<setprecision(2) << amount << timestamp;
        return ss.str();
    }
};

class Block {
public:
    int index;
    string timestamp;
    vector<Transaction> transactions;
    string previous_hash;
    string merkle_root;
    string hash;
    uint64_t nonce;
    int difficulty;

    Block(int idx, const vector<Transaction>& txs, const string& prev_hash, int diff = 4)
        : index(idx), timestamp(get_current_time()), transactions(txs),
          previous_hash(prev_hash), nonce(0), difficulty(diff) {
        merkle_root = calc_merkle_root();
        mine_block();
    }

    string calc_merkle_root() const {
        if (transactions.empty()) return calc_sha256("EMPTY_BLOCK");

        vector<string> tree_layer;
        for (const auto& tx : transactions) {
            tree_layer.push_back(calc_sha256(tx.to_string_hash()));
        }

        while (tree_layer.size() > 1) {
            if (tree_layer.size() % 2 != 0) {
                tree_layer.push_back(tree_layer.back());
            }
            vector<string> next_layer;
            for (size_t i = 0; i < tree_layer.size(); i += 2) {
                next_layer.push_back(calc_sha256(tree_layer[i] + tree_layer[i + 1]));
            }
            tree_layer = next_layer;
        }
        return tree_layer.front();
    }

    string calc_hash() const {
        stringstream ss;
        ss << index << timestamp << merkle_root << previous_hash << nonce << difficulty;
        return calc_sha256(ss.str());
    }

    void mine_block() {
        string target(difficulty, '0');
        auto start = chrono::high_resolution_clock::now();

        do {
            nonce++;
            hash = calc_hash();
        } while (hash.substr(0, difficulty) != target);

        auto end =  chrono::high_resolution_clock::now();
        chrono::duration<double> elapsed = end - start;

        cout << "[MINED] Block #" << index << " | Nonce: " << nonce
             << " | Time: " << fixed << setprecision(4) << elapsed.count() << "s\n"
             << "        Hash: " << hash << "\n\n";
    }

    void print_block() const {
        cout << "====================================================================\n";
        cout << "BLOCK #" << index << "\n";
        cout << "====================================================================\n";
        cout << "  Timestamp     : " << timestamp << "\n";
        cout << "  Previous Hash : " << previous_hash << "\n";
        cout << "  Merkle Root   : " << merkle_root << "\n";
        cout << "  Nonce         : " << nonce << " (Difficulty: " << difficulty << ")\n";
        cout << "  Hash          : " << hash << "\n";
        cout << "  Transactions (" << transactions.size() << "):\n";
        for (const auto& tx : transactions) {
            cout << "    [" << tx.timestamp << "] " << tx.sender << " -> " << tx.receiver
                 << " : " << fixed << setprecision(2) << tx.amount << " COIN\n";
        }
        cout << "====================================================================\n\n";
    }

};