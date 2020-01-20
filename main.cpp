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

int main(int argc, char const *argv[])
{

    return 0;
}