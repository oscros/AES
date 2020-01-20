#include <iostream>
#include <fstream>

// Define constants

// given key in the kattis assignment is 16 bytes long
static const int KEY_SIZE = 16;
// plaintext is at most 10e6 according to kattis
static const int PLAINTEXT_SIZE = 16 * 10 ^ 6;

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
// state matrix
uint8_t *state = new uint8_t[4 * Nb];

// round constant word array (found here: https://en.wikipedia.org/wiki/AES_key_schedule)
static const uint8_t *Rcon = new uint8_t[10] {
    0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1B, 0x36
};

static const uint8_t *S_box = new uint8_t[16 * 16] {};


int main(int argc, char const *argv[])
{

    return 0;
}