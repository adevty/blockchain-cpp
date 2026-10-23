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