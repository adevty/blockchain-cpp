# Blockchain in C++

A simple, Proof-of-Work (PoW) blockchain implementation in C++ using OpenSSL for SHA-256 hashing.

---

## Features

- **Proof of Work (PoW)**: Mines blocks with configurable difficulty by finding valid hash prefixes.
- **SHA-256 Hashing**: Powered by OpenSSL (`EVP` API) for secure cryptographic hashing.
- **Merkle Tree / Merkle Root**: Hashes all transactions in a block into a single Merkle root for data integrity.
- **Transaction & Mempool**: Handles pending transactions with sender balance verification before adding to mempool.
- **Mining Rewards**: Automatically rewards miners with system-generated coins for each mined block.
- **Wallet Balance Calculation**: Tracks balances across mined blocks and unconfirmed transactions.
- **Chain Integrity Audit**: Verifies block hashes, previous hash linkage, Merkle roots, and PoW validity.

---

## Prerequisites

- C++17 or higher
- Clang++ or G++
- OpenSSL development libraries (`libssl-dev`)

### Installing OpenSSL

- **Ubuntu / Debian**:
  ```bash
  sudo apt update && sudo apt install libssl-dev
  ```
- **Arch Linux**:
  ```bash
  sudo pacman -S openssl
  ```
- **Fedora / RHEL**:
  ```bash
  sudo dnf install openssl-devel
  ```
- **macOS** (Homebrew):
  ```bash
  brew install openssl
  ```

---

## Compilation Guide (Linux)

### 1. Recommended: Optimized Build (Fast Mining)
Because mining calculates millions of hashes, using the `-O3` optimization flag significantly speeds up execution:

```bash
# Using Clang++
clang++ -std=c++17 -O3 blockchain.cpp -o blockchain -lcrypto

# Using G++
g++ -std=c++17 -O3 blockchain.cpp -o blockchain -lcrypto
```

### 2. Development / Debug Build
Enables compiler warnings and debugging symbols:

```bash
clang++ -std=c++17 -Wall -Wextra -g blockchain.cpp -o block -lcrypto
```

**Flag Breakdown:**
- `-std=c++17`: Uses the C++17 standard.
- `-O3`: Maximum compiler optimization for fast mining loops.
- `-Wall -Wextra`: Enables comprehensive compiler warnings.
- `-g`: Generates debug symbols for GDB / Valgrind.
- `-lcrypto`: Links OpenSSL's cryptography library (`libcrypto.so`).

---

## Using a Makefile (Optional)

You can use `make` to avoid re-typing commands. A basic `Makefile`:

```makefile
CXX = clang++
CXXFLAGS = -std=c++17 -Wall -Wextra -O3
LIBS = -lcrypto
TARGET = blockchain
SRC = blockchain.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) $(LIBS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
```

**Commands:**
- `make` — Compiles the binary.
- `make run` — Compiles and executes `./block`.
- `make clean` — Removes the executable.

---

## Running the Program

```bash
./blockchain
```

---

## Example Output

```text
Create a Genesis Block...
[MINED] Block #0 | Nonce: 64213 | Time: 0.3094s
        Hash: 000023c3157d6b6adfa7ea6ee149b128d45453db0dcc79307ee302b091cdc49b

[MEMPOOL] Transaction added: Founder -> Alice (250.0000 COIN)

Starting block mining #1...
[MINED] Block #1 | Nonce: 8437 | Time: 0.0484s
        Hash: 00000aff176fd3812bb013c16345f60e8488ed37fc45c179eab67c93b70e1e7a

[MEMPOOL] Transaction added: Alice -> Bob (150.0000 COIN)

Starting block mining #2...
[MINED] Block #2 | Nonce: 2990 | Time: 0.0170s
        Hash: 00002005acded1b4b5097c707783e59eaf7cda3e15327db5023d3061edd336f7


 === HISTORY BLOCKCHAIN ===
====================================================================
BLOCK #0
====================================================================
  Timestamp     : 2026-10-25 16:42:52
  Previous Hash : 0
  Merkle Root   : f1e5ce686e3dfef749543854fc60b718f4975bb8c80385c494e9240ae6a3c934
  Nonce         : 64213 (Difficulty: 4)
  Hash          : 000023c3157d6b6adfa7ea6ee149b128d45453db0dcc79307ee302b091cdc49b
  Transactions (1):
    [2026-10-25 16:42:52] NETWORK -> Founder : 1000.00 COIN
====================================================================

====================================================================
BLOCK #1
====================================================================
  Timestamp     : 2026-10-25 16:42:52
  Previous Hash : 000023c3157d6b6adfa7ea6ee149b128d45453db0dcc79307ee302b091cdc49b
  Merkle Root   : a59b032b647ed533207ed4ca970f49072c5ff50e6c87a32928c5402afca9de66
  Nonce         : 8437 (Difficulty: 4)
  Hash          : 00000aff176fd3812bb013c16345f60e8488ed37fc45c179eab67c93b70e1e7a
  Transactions (2):
    [2026-10-25 16:42:52] Founder -> Alice : 250.00 COIN
    [2026-10-25 16:42:52] SYSTEM -> First Miner : 50.00 COIN
====================================================================

====================================================================
BLOCK #2
====================================================================
  Timestamp     : 2026-10-25 16:42:52
  Previous Hash : 00000aff176fd3812bb013c16345f60e8488ed37fc45c179eab67c93b70e1e7a
  Merkle Root   : 684e79959a92e60291accccd13afbca809e316ab265897274790ca916dcb97d6
  Nonce         : 2990 (Difficulty: 4)
  Hash          : 00002005acded1b4b5097c707783e59eaf7cda3e15327db5023d3061edd336f7
  Transactions (2):
    [2026-10-25 16:42:52] Alice -> Bob : 150.00 COIN
    [2026-10-25 16:42:52] SYSTEM -> Second Miner : 50.00 COIN
====================================================================

=== BALANCE WALLET NOW ===
Bob        : 150.00 COIN
Alice      : 100.00 COIN
Founder    : 750.00 COIN

=== AUDIT SECURITY ===
Is Blockchain Valid? YES (SECURE)
```

---

## How It Works

1. **Genesis Block**: Created automatically with initial coin distribution to the Founder.
2. **Transactions**: Wallets can transfer coins. Transactions must be mined into a block to be confirmed.
3. **Mining**: The miner solves the Proof-of-Work puzzle (finding a hash starting with $N$ zeros) and receives a block reward.
4. **Validation**: The blockchain self-audits to ensure no blocks or transactions have been tampered with.
