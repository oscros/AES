#define CATCH_CONFIG_MAIN // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "../aes.cpp"

// TEST_CASE("RotWord")
// {
//     uint8_t *word = new uint8_t[4]{0, 1, 2, 3};
//     uint8_t *rotated_word = RotWord(word);
//     REQUIRE(word[0] == rotated_word[0]);
//     REQUIRE(word[1] == rotated_word[1]);
//     REQUIRE(word[2] == rotated_word[2]);
//     REQUIRE(word[3] == rotated_word[3]);
//     REQUIRE(1 == rotated_word[0]);
//     REQUIRE(2 == rotated_word[1]);
//     REQUIRE(3 == rotated_word[2]);
//     REQUIRE(0 == rotated_word[3]);
// }

// TEST_CASE("SubWord")
// {
//     // Test case from https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.197.pdf (page: 27)
//     uint8_t *word = new uint8_t[4]{0xcf, 0x4f, 0x3c, 0x09};
//     SubWord(word);
//     REQUIRE(0x8a == word[0]);
//     REQUIRE(0x84 == word[1]);
//     REQUIRE(0xeb == word[2]);
//     REQUIRE(0x01 == word[3]);
// }

// TEST_CASE("KeyExpansion")
// {
//     // Test case from https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.197.pdf (page: 27)
//     key[0] = 0x2b;
//     key[1] = 0x7e;
//     key[2] = 0x15;
//     key[3] = 0x16;

//     key[4] = 0x28;
//     key[5] = 0xae;
//     key[6] = 0xd2;
//     key[7] = 0xa6;

//     key[8] = 0xab;
//     key[9] = 0xf7;
//     key[10] = 0x15;
//     key[11] = 0x88;

//     key[12] = 0x09;
//     key[13] = 0xcf;
//     key[14] = 0x4f;
//     key[15] = 0x3c;

//     KeyExpansion();

//     // std::cout << "Expanded Key: " << std::endl;
//     // for (int i = 0; i < 4 * Nb * (Nr + 1); i++)
//     // {
//     //     std::cout << std::hex << static_cast<int>(key_schedule[i]) << " ";
//     //     std::cout << std::hex << static_cast<int>(key_schedule[i+1]) << " ";
//     //     std::cout << std::hex << static_cast<int>(key_schedule[i+2]) << " ";
//     //     std::cout << std::hex << static_cast<int>(key_schedule[i+3]) << " " << std::endl;
//     //     i = i + 3;
//     // }
//     // std::cout << "\n-------------------------------------------------" << std::endl;
//     // SubWord(word);
//     // REQUIRE(0x8a == word[0]);
//     // REQUIRE(0x84 == word[1]);
//     // REQUIRE(0xeb == word[2]);
//     // REQUIRE(0x01 == word[3]);
// }

// TEST_CASE("multiply_in_GF1")
// {

//     uint8_t a = 0xd4;
//     uint8_t b = 0x02;
//     uint8_t p = multiply_in_GF(a, b);
//     std::cout << "product: " << std::hex << static_cast<int>(p) << std::endl;
//     REQUIRE(p == 0xb3);
// }
// TEST_CASE("multiply_in_GF2")
// {

//     uint8_t a = 0xbf;
//     uint8_t b = 0x03;
//     uint8_t p = multiply_in_GF(a, b);
//     std::cout << "product: " << std::hex << static_cast<int>(p) << std::endl;
//     REQUIRE(p == 0xda);
// }
// TEST_CASE("multiply_in_GF3")
// {

//     uint8_t a = 0x5d;
//     uint8_t b = 0x01;
//     uint8_t p = multiply_in_GF(a, b);
//     std::cout << "product: " << std::hex << static_cast<int>(p) << std::endl;
//     REQUIRE(p == 0x5d);
// }

// TEST_CASE("multiply_in_GF4")
// {

//     uint8_t a = 0x30;
//     uint8_t b = 0x01;
//     uint8_t p = multiply_in_GF(a, b);
//     std::cout << "product: " << std::hex << static_cast<int>(p) << std::endl;
//     REQUIRE(p == 0x30);
// }

// TEST_CASE("multiply_in_GF5")
// {

//     uint8_t p = 0xb3 ^ 0xda ^ 0x5d ^ 0x30;
//     std::cout << "product: " << std::hex << static_cast<int>(p) << std::endl;
//     REQUIRE(p == 0x04);
// }

// TEST_CASE("multiply_in_GF6")
// {

//     uint8_t p = multiply_in_GF(0xd4, 0x03) ^ 0xbf ^ 0x5d ^ multiply_in_GF(0x30, 0x02);
//     std::cout << "product: " << std::hex << static_cast<int>(p) << std::endl;
//     REQUIRE(p == 0xe5);
// }
// TEST_CASE("Cipher")
// {

//     key[0] = 0x2b;
//     key[1] = 0x7e;
//     key[2] = 0x15;
//     key[3] = 0x16;

//     key[4] = 0x28;
//     key[5] = 0xae;
//     key[6] = 0xd2;
//     key[7] = 0xa6;

//     key[8] = 0xab;
//     key[9] = 0xf7;
//     key[10] = 0x15;
//     key[11] = 0x88;

//     key[12] = 0x09;
//     key[13] = 0xcf;
//     key[14] = 0x4f;
//     key[15] = 0x3c;

//     KeyExpansion();

//     plaintext[0] = 0x32;
//     plaintext[1] = 0x43;
//     plaintext[2] = 0xf6;
//     plaintext[3] = 0xa8;

//     plaintext[4] = 0x88;
//     plaintext[5] = 0x5a;
//     plaintext[6] = 0x30;
//     plaintext[7] = 0x8d;

//     plaintext[8] = 0x31;
//     plaintext[9] = 0x31;
//     plaintext[10] = 0x98;
//     plaintext[11] = 0xa2;

//     plaintext[12] = 0xe0;
//     plaintext[13] = 0x37;
//     plaintext[14] = 0x07;
//     plaintext[15] = 0x34;
//     std::cout << "------------- plaintext -------------" << std::endl;

//     print_block(plaintext);
//     uint8_t *state = Transpose(plaintext);
//     std::cout << "------------- start state -------------" << std::endl;

//     print_block(state);
//     // transpose the plaintext matrix as input
//     std::cout << "------------- START -------------" << std::endl;
//     AddRoundKey(state, 0);
//     print_block(state);
//     for (int i = 1; i < Nr; i++)
//     {
//         SubBytes(state);
//         ShiftRows(state);
//         MixColumns(state);
//         AddRoundKey(state, i);
//     }

//     SubBytes(state);
//     ShiftRows(state);
//     AddRoundKey(state, Nr);

//     std::cout << "------------- RESULT -------------" << std::endl;
//     print_block(state);
// }

// TEST_CASE("Cipher2")
// {

//     key[0] = 0x00;
//     key[1] = 0x01;
//     key[2] = 0x02;
//     key[3] = 0x03;

//     key[4] = 0x04;
//     key[5] = 0x05;
//     key[6] = 0x06;
//     key[7] = 0x07;

//     key[8] = 0x08;
//     key[9] = 0x09;
//     key[10] = 0x0a;
//     key[11] = 0x0b;

//     key[12] = 0x0c;
//     key[13] = 0x0d;
//     key[14] = 0x0e;
//     key[15] = 0x0f;

//     KeyExpansion();

//     plaintext[0] = 0x00;
//     plaintext[1] = 0x11;
//     plaintext[2] = 0x22;
//     plaintext[3] = 0x33;

//     plaintext[4] = 0x44;
//     plaintext[5] = 0x55;
//     plaintext[6] = 0x66;
//     plaintext[7] = 0x77;

//     plaintext[8] = 0x88;
//     plaintext[9] = 0x99;
//     plaintext[10] = 0xaa;
//     plaintext[11] = 0xbb;

//     plaintext[12] = 0xcc;
//     plaintext[13] = 0xdd;
//     plaintext[14] = 0xee;
//     plaintext[15] = 0xff;
//     std::cout << "------------- plaintext -------------" << std::endl;

//     print_block(plaintext);
//     uint8_t *state = Transpose(plaintext);
//     std::cout << "------------- start state -------------" << std::endl;

//     print_block(state);
//     // transpose the plaintext matrix as input
//     std::cout << "------------- START -------------" << std::endl;
//     AddRoundKey(state, 0);
//     print_block(state);
//     for (int i = 1; i < Nr; i++)
//     {
//         SubBytes(state);
//         ShiftRows(state);
//         MixColumns(state);
//         AddRoundKey(state, i);
//     }

//     SubBytes(state);
//     ShiftRows(state);
//     AddRoundKey(state, Nr);

//     std::cout << "------------- RESULT -------------" << std::endl;
//     print_block(state);
// }

TEST_CASE("Cipher3")
{

    key[0] = 0xF4;
    key[1] = 0xC0;
    key[2] = 0x20;
    key[3] = 0xA0;
    
    key[4] = 0xA1;
    key[5] = 0xF6;
    key[6] = 0x04;
    key[7] = 0xFD;

    key[8] = 0x34;
    key[9] = 0x3F;
    key[10] = 0xAC;
    key[11] = 0x6A;

    key[12] = 0x7E;
    key[13] = 0x6A;
    key[14] = 0xE0;
    key[15] = 0xF9;

    print_block(key);

    KeyExpansion();

    plaintext[0] = 0xF2;
    plaintext[1] = 0x95;
    plaintext[2] = 0xB9;
    plaintext[3] = 0x31;

    plaintext[4] = 0x8B;
    plaintext[5] = 0x99;
    plaintext[6] = 0x44;
    plaintext[7] = 0x34;

    plaintext[8] = 0xD9;
    plaintext[9] = 0x3D;
    plaintext[10] = 0x98;
    plaintext[11] = 0xA4;

    plaintext[12] = 0xE4;
    plaintext[13] = 0x49;
    plaintext[14] = 0xAf;
    plaintext[15] = 0xD8;
    std::cout << "------------- plaintext -------------" << std::endl;

    print_block(plaintext);
    uint8_t *state = Transpose(plaintext);
    std::cout << "------------- start state -------------" << std::endl;

    print_block(state);
    // transpose the plaintext matrix as input
    std::cout << "------------- START -------------" << std::endl;
    AddRoundKey(state, 0);
    print_block(state);
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

    std::cout << "------------- RESULT -------------" << std::endl;
    print_block(state);
}