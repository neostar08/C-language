#include <iostream>
#include <bitset>

using namespace std;

// S-Box lookup table
const int sbox[16] = { 0x0, 0x6, 0xC, 0xA, 0xB, 0x5, 0x9, 0xE, 0x1, 0x7, 0xD, 0xF, 0x8, 0x4, 0x2, 0x3 };

// Permutation lookup table
const int permutation[16] = { 0x0, 0x4, 0x8, 0xC, 0x1, 0x5, 0x9, 0xD, 0x2, 0x6, 0xA, 0xE, 0x3, 0x7, 0xB, 0xF };

// Round key generation
void generate_round_keys(bitset<32> master_key, bitset<16> round_keys[5]) {
    for (int i = 0; i < 5; i++) {
        round_keys[i] = bitset<16>(master_key.to_string().substr(i * 8, 8));
    }
}

// Round function
bitset<16> round_function(bitset<16> input, bitset<16> round_key) {
    // Round key addition
    input ^= round_key;

    // S-Box substitution
    bitset<16> output;
    for (int i = 0; i < 4; i++) {
        output |= bitset<16>(sbox[(input >> i * 4).to_ulong()] << i * 4);
    }

    // Permutation
    bitset<16> temp;
    for (int i = 0; i < 16; i++) {
        temp[i] = output[permutation[i]];
    }
    output = temp;

    return output;
}

// Encryption function
bitset<16> encrypt(bitset<16> plaintext, bitset<32> master_key) {
    bitset<16> round_keys[5];
    generate_round_keys(master_key, round_keys);

    bitset<16> ciphertext = plaintext;
    for (int i = 0; i < 4; i++) {
        ciphertext = round_function(ciphertext, round_keys[i]);
    }
    ciphertext ^= round_keys[4];

    return ciphertext;
}

int main() {
    // Set master key and plaintext
    bitset<32> master_key("01000100010101010110111001000101"); // 0x44554E45 in hex
    bitset<16> plaintext("0001001000110100"); // 0x1234 in hex

    // Encrypt plaintext
    bitset<16> ciphertext = encrypt(plaintext, master_key);

    // Print results
    cout << "Master key = " << hex << master_key.to_ulong() << endl;
    cout << "Plaintext = " << hex << plaintext.to_ulong() << endl;
    cout << "Ciphertext = " << hex << ciphertext.to_ulong() << endl;

    // Print intermediate values after first round
    bitset<16> round_keys[5];
    generate_round_keys(master_key, round_keys);
    bitset<16> temp = round_function(plaintext, round_keys[0]);
    cout << "Intermediate values after first round:" << endl;
    cout << "After Round Key Addition: " << hex << (plaintext ^ round_keys[0]).to_ulong() << endl;
}
