#define CATCH_CONFIG_MAIN // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "../aes.cpp"

TEST_CASE("RotWord")
{
    uint8_t *word = new uint8_t[4]{0, 1, 2, 3};
    uint8_t *rotated_word = RotWord(word);
    REQUIRE(word[0] == rotated_word[0]);
    REQUIRE(word[1] == rotated_word[1]);
    REQUIRE(word[2] == rotated_word[2]);
    REQUIRE(word[3] == rotated_word[3]);
    REQUIRE(1 == rotated_word[0]);
    REQUIRE(2 == rotated_word[1]);
    REQUIRE(3 == rotated_word[2]);
    REQUIRE(0 == rotated_word[3]);
}

TEST_CASE("SubWord")
{
    // Test case from https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.197.pdf (page: 27)
    uint8_t *word = new uint8_t[4]{0xcf, 0x4f, 0x3c, 0x09};
    SubWord(word);
    REQUIRE(0x8a == word[0]);
    REQUIRE(0x84 == word[1]);
    REQUIRE(0xeb == word[2]);
    REQUIRE(0x01 == word[3]);
}

TEST_CASE("KeyExpansion")
{
    // Test case from https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.197.pdf (page: 27)
    key[0] = 0x2b;
    key[1] = 0x7e;
    key[2] = 0x15;
    key[3] = 0x16;

    key[4] = 0x28;
    key[5] = 0xae;
    key[6] = 0xd2;
    key[7] = 0xa6;

    key[8] = 0xab;
    key[9] = 0xf7;
    key[10] = 0x15;
    key[11] = 0x88;

    key[12] = 0x09;
    key[13] = 0xcf;
    key[14] = 0x4f;
    key[15] = 0x3c;

    KeyExpansion();

    // std::cout << "Expanded Key: " << std::endl;
    // for (int i = 0; i < 4 * Nb * (Nr + 1); i++)
    // {
    //     std::cout << std::hex << static_cast<int>(key_schedule[i]) << " ";
    //     std::cout << std::hex << static_cast<int>(key_schedule[i+1]) << " ";
    //     std::cout << std::hex << static_cast<int>(key_schedule[i+2]) << " ";
    //     std::cout << std::hex << static_cast<int>(key_schedule[i+3]) << " " << std::endl;
    //     i = i + 3;
    // }
    // std::cout << "\n-------------------------------------------------" << std::endl;
    // SubWord(word);
    // REQUIRE(0x8a == word[0]);
    // REQUIRE(0x84 == word[1]);
    // REQUIRE(0xeb == word[2]);
    // REQUIRE(0x01 == word[3]);
}

TEST_CASE("AddRoundKey")
{
    // declare state and round key

    // run addroundkey

    // check that they are the same
}