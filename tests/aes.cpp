#include <iostream>
#include <fstream>

// Define constants

// given key in the kattis assignment is 16 bytes long
static const int KEY_SIZE = 16;
// plaintext is at most 10e6 according to kattis
static const int PLAINTEXT_SIZE = 16 * 1000000;

// According to the specification: https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.197.pdf
// Number  of  columns  (32-bit  words)  comprising  the  State.  For  this  standard, Nb = 4.
static const int Nb = 4;
// Number  of  32-bit  words  comprising  the  Cipher  Key.  For  this  standard, Nk = 4
static const int Nk = 4;
// Number  of  rounds,  which  is  a  function  of  Nk and  Nb (which  is  fixed). For this standard, Nr = 10
static const int Nr = 10;

// original key
uint8_t *key = new uint8_t[KEY_SIZE];
// plaintext
uint8_t *plaintext = new uint8_t[PLAINTEXT_SIZE];

// round constant word array (found here: https://en.wikipedia.org/wiki/AES_key_schedule)
static const uint8_t *Rcon = new uint8_t[10]{
    0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1B, 0x36};

static const uint8_t *S_box = new uint8_t[16 * 16]{
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16};

// 11 keys, first key is the original key, then 10 round keys
uint8_t *key_schedule = new uint8_t[4 * Nb * (Nr + 1)];

/**
 * Prints the given word
 * */
void print_word(uint8_t const *const &word)
{
    std::cout << std::hex << static_cast<int>(word[0]) << " " << static_cast<int>(word[1]) << " " << static_cast<int>(word[2]) << " " << static_cast<int>(word[3]) << std::endl;
}

/**
 * Prints the given block
 * */
void print_block(uint8_t const *const &block)
{
    for (int i = 0; i < Nk; i++)
    {
        uint8_t *temp_word = new uint8_t[4]{block[i * 4], block[i * 4 + 1], block[i * 4 + 2], block[i * 4 + 3]};
        print_word(temp_word);
        delete[] temp_word;
    }
}

/**
 * Rotates a word left one step
 * */
uint8_t *const &RotWord(uint8_t *const &temp)
{
    uint8_t t = temp[0];
    temp[0] = temp[1];
    temp[1] = temp[2];
    temp[2] = temp[3];
    temp[3] = t;
    return temp;
}

/**
 * Substitutes each value in a word with the corresponding value in the S_box
 * */
uint8_t *const &SubWord(uint8_t *const &temp)
{
    for (int i = 0; i < Nk; i++)
    {
        uint8_t substituted_val = S_box[temp[i]];
        temp[i] = substituted_val;
    }
    return temp;
}

/**
 * Performs Key Expansion and fills out the key_schedule with keys for all rounds 
 * */
void KeyExpansion()
{
    uint8_t *temp_word = new uint8_t[Nk];

    int i = 0;

    // first key is the original key
    // i is the i:th word
    while (i < Nk)
    {
        key_schedule[(i * 4)] = key[i * 4];
        key_schedule[(i * 4) + 1] = key[(i * 4) + 1];
        key_schedule[(i * 4) + 2] = key[(i * 4) + 2];
        key_schedule[(i * 4) + 3] = key[(i * 4) + 3];
        i++;
    }

    // Perform the expansion for the 10 round keys
    i = Nk;
    while (i < (Nb * (Nr + 1)))
    {
        temp_word[0] = key_schedule[4 * (i - 1)];
        temp_word[1] = key_schedule[(4 * (i - 1)) + 1];
        temp_word[2] = key_schedule[(4 * (i - 1)) + 2];
        temp_word[3] = key_schedule[(4 * (i - 1)) + 3];
        if (i % Nk == 0)
        {
            // Rotate and substitute the word
            SubWord(RotWord(temp_word));
            // xor the word, since only the first element in the round constant is non-zero,
            // it's enough to xor the first element of temp_word with the the corresponding element in Rcon
            uint8_t t = temp_word[0];
            temp_word[0] = t ^ Rcon[(i / Nk) - 1];
        }
        key_schedule[4 * i] = key_schedule[(i - Nk) * 4] ^ temp_word[0];
        key_schedule[(4 * i) + 1] = key_schedule[((i - Nk) * 4) + 1] ^ temp_word[1];
        key_schedule[(4 * i) + 2] = key_schedule[((i - Nk) * 4) + 2] ^ temp_word[2];
        key_schedule[(4 * i) + 3] = key_schedule[((i - Nk) * 4) + 3] ^ temp_word[3];

        // go to the next word
        i++;
    }
    delete[] temp_word;
}

/**
 * xor's the given state with the current round key, column by column
 * */
void AddRoundKey(uint8_t *const &state, int round_key)
{
    for (int i = 0; i < Nk; i++)
    {
        state[i] = state[i] ^ key_schedule[(round_key * KEY_SIZE) + (i * 4)];
        state[i + 4] = state[i + 4] ^ key_schedule[(round_key * KEY_SIZE) + (i * 4) + 1];
        state[i + 8] = state[i + 8] ^ key_schedule[(round_key * KEY_SIZE) + (i * 4) + 2];
        state[i + 12] = state[i + 12] ^ key_schedule[(round_key * KEY_SIZE) + (i * 4) + 3];
    }
}

/**
 * Substitutes each value in the given state with its corresponding value in the substitutaion box
 * */
void SubBytes(uint8_t *const &state)
{
    for (int i = 0; i < 4 * Nk; i++)
    {
        state[i] = S_box[state[i]];
    }
}

/**
 * Shifts each row 0, 1, 2 or 3 positions to the left.
 *  - row 0, shifts 0 positions to left
 *  - row 1, shifts 1 positions to left
 *  - row 2, shifts 2 positions to left
 *  - row 3, shifts 3 positions to left
 * */
void ShiftRows(uint8_t *const &state)
{
    for (int i = 0; i < Nk; i++)
    {
        for (int j = 0; j < i; j++)
        {
            uint8_t *word = new uint8_t[4]{
                state[i * 4],
                state[i * 4 + 1],
                state[i * 4 + 2],
                state[i * 4 + 3]};
            word = RotWord(word);
            state[i * 4] = word[0];
            state[i * 4 + 1] = word[1];
            state[i * 4 + 2] = word[2];
            state[i * 4 + 3] = word[3];
            delete[] word;
        }
    }
}

/**
 * Multiplies a and b in Galois field GF(2^8). 
 * Based on pseudocode from https://en.wikipedia.org/wiki/Finite_field_arithmetic#Multiplication
 * */
uint8_t multiply_in_GF(uint8_t a, uint8_t b)
{
    uint8_t product = 0;

    for (int i = 0; i < 8; i++)
    {
        if (b & 1)
        {
            product = product ^ a;
        }
        b = b >> 1;

        // carry is the leftmost bit in a
        uint8_t carry = a & 0x80;
        a = a << 1;
        if (carry)
        {
            a = a ^ 0x1b;
        }
    }
    return product;
}

/**
 * Mix the columns in the state by performing multiplication in Galois field GF(2^8) 
 * between the state each column and the following matrix
 * 
 * 02 03 01 01 
 * 01 02 03 01 
 * 01 01 02 03
 * 03 01 01 02
 * 
 * */
void MixColumns(uint8_t *const &state)
{
    for (int i = 0; i < Nk; i++)
    {
        uint8_t s0 = multiply_in_GF(state[i], 0x02) ^ multiply_in_GF(state[i + 4], 0x03) ^ state[i + 8] ^ state[i + 12];
        uint8_t s1 = state[i] ^ multiply_in_GF(state[i + 4], 0x02) ^ multiply_in_GF(state[i + 8], 0x03) ^ state[i + 12];
        uint8_t s2 = state[i] ^ state[i + 4] ^ multiply_in_GF(state[i + 8], 0x02) ^ multiply_in_GF(state[i + 12], 0x03);
        uint8_t s3 = multiply_in_GF(state[i], 0x03) ^ state[i + 4] ^ state[i + 8] ^ multiply_in_GF(state[i + 12], 0x02);

        state[i] = s0;
        state[i + 4] = s1;
        state[i + 8] = s2;
        state[i + 12] = s3;
    }
}

/**
 * Transpose the given block
 * Assumes the input is a 4 * 4 matrix
 * */
uint8_t *Transpose(uint8_t const *const &block)
{
    uint8_t *transpose = new uint8_t[16];

    transpose[0] = block[0];
    transpose[1] = block[4];
    transpose[2] = block[8];
    transpose[3] = block[12];

    transpose[4] = block[1];
    transpose[5] = block[5];
    transpose[6] = block[9];
    transpose[7] = block[13];

    transpose[8] = block[2];
    transpose[9] = block[6];
    transpose[10] = block[10];
    transpose[11] = block[14];

    transpose[12] = block[3];
    transpose[13] = block[7];
    transpose[14] = block[11];
    transpose[15] = block[15];

    return transpose;
}

/**
 * Takes a block as input, encrypts it and returns the ciphertext 
 * */
uint8_t *Cipher(uint8_t *in)
{
    uint8_t *state = Transpose(in);

    AddRoundKey(state, 0);

    for (int i = 1; i < Nr; i++)
    {
        SubBytes(state);
        ShiftRows(state);
        MixColumns(state);
        AddRoundKey(state, i);
    }

    SubBytes(state);
    ShiftRows(state);
    AddRoundKey(state, Nr);
    return state;
}