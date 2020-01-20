#include <iostream>
#include <fstream>

// ------ defining constants ------

// According to kattis assignment
// key always 16 bytes
static const int KEY_SIZE = 16;
// plaintext maximum size is 16 * 10^6 bytes
static const int PLAINTEXT_MAX_SIZE = (16 * 1000000);
// According to the specification: https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.197.pdf

// The length of input block, output block and state is 128 bits, Block size (Nb) is therefore always 4 32 bit words (number of columns in the state)
static const int Nb = 4;
// The length of the key (Nk) is always 128 bits, 4 32-bit words (number of columns in the key), for AES 128-bit
static const int Nk = 4;
// The number of rounds (Nr) is always 10 for AES 128-bit
static const int Nr = 10;

// Cipher key global variable
uint8_t *key = new uint8_t[KEY_SIZE];
// plaintext global variable
uint8_t *plaintext = new uint8_t[PLAINTEXT_MAX_SIZE];
// state global matrix
uint8_t state[Nb * 4];

// The expanded key contains the cipher key itself and 10 round keys, a total of Nb*(Nr+1) words, each containing 4 bytes
uint8_t *keySchedule = new uint8_t[4 * Nb * (Nr + 1)];

// The round constant word array. Only need to xor with the first byte in each word due to the other bytes are {00} and doesn't affect the result
static const uint8_t rcon[10] = {
    0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36};

// sbox stored as constant global variable to be used when substituting bytes in subBytes()
static const uint8_t sbox[256] = {
    //0     1    2      3     4    5     6     7      8    9     A      B    C     D     E     F
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

static const uint8_t mixConstant[16] = {
    0x02, 0x03, 0x01, 0x01,
    0x01, 0x02, 0x03, 0x01,
    0x01, 0x01, 0x02, 0x03,
    0x03, 0x01, 0x01, 0x02};

// Forward declare functions
uint8_t *cipher(uint8_t *in);
uint8_t *subWord(uint8_t *word);
uint8_t *rotWord(uint8_t *word);
void keyExpansion();

void subBytes();
void shiftRows();
void mixColumns();
void addRoundKey(int round);

/**
 * Function that rotates a 4 byte word to the left once.
 * */
uint8_t *rotWord(uint8_t *word)
{
    uint8_t *temp = new uint8_t[4];
    temp[0] = word[1];
    temp[1] = word[2];
    temp[2] = word[3];
    temp[3] = word[0];
    return temp;
}

/**
 * Function that substitutes each byte in a 4 byte word according to the sbox
 * */
uint8_t *subWord(uint8_t *word)
{
    uint8_t *temp = new uint8_t[4];

    for (int i = 0; i < 4; i++)
    {
        temp[i] = sbox[word[i]];
    }
    return temp;
}

/**
 * Function that performs key expansion and generates key schedule containing the round keys
 * */
void keyExpansion()
{
    uint8_t *temp = new uint8_t[4];

    // first key in the keyschedule is the cipher key.
    // i denotes the word
    int i = 0;
    while (i < Nk)
    {
        keySchedule[(i * 4)] = key[(i * 4)];
        keySchedule[(i * 4) + 1] = key[(i * 4) + 1];
        keySchedule[(i * 4) + 2] = key[(i * 4) + 2];
        keySchedule[(i * 4) + 3] = key[(i * 4) + 3];
        i++;
    }
    i = Nk;
    while (i < Nb * (Nr + 1))
    {
        temp[0] = keySchedule[(i - 1) * 4];
        temp[1] = keySchedule[((i - 1) * 4) + 1];
        temp[2] = keySchedule[((i - 1) * 4) + 2];
        temp[3] = keySchedule[((i - 1) * 4) + 3];

        if (i % Nk == 0)
        {
            // should perform subWord and rotword on the word
            temp = subWord(rotWord(temp));
            // xor with "random constant word array" value.
            temp[0] = temp[0] ^ rcon[(i / Nk) - 1];
        }

        // xor temp with previous word and save as the new word
        keySchedule[(i * 4)] = keySchedule[(i - Nk) * 4] ^ temp[0];
        keySchedule[((i * 4)) + 1] = keySchedule[((i - Nk) * 4) + 1] ^ temp[1];
        keySchedule[((i * 4)) + 2] = keySchedule[((i - Nk) * 4) + 2] ^ temp[2];
        keySchedule[((i * 4)) + 3] = keySchedule[((i - Nk) * 4) + 3] ^ temp[3];
        i++;
    }
}

/**
 * Function that substitutes each byte in the state array according to the sbox
 * */
void subBytes()
{
    std::cout << "---------------------------- sub bytes ----------------------" << std::endl;

    for (size_t i = 0; i < 16; i++)
    {
        std::cout << "state " << std::hex << static_cast<int>(state[i]) << std::endl;
    }

    for (int i = 0; i < Nb * 4; i++)
    {
        state[i] = sbox[state[i]];
    }
    std::cout << "--------------------------------------------------" << std::endl;
    for (size_t i = 0; i < 16; i++)
    {
        std::cout << "state " << std::hex << static_cast<int>(state[i]) << std::endl;
    }
    std::cout << "---------------------------- /sub bytes ----------------------" << std::endl;
}

/**
 * Function that shifts each row i steps, where i is the row rumber (0 <= i < 4)
 * 
 * */
void shiftRows()
{
    std::cout << "------------------------ shift rows --------------------------" << std::endl;
    for (size_t i = 0; i < 4; i++)
    {
        std::cout << std::hex << static_cast<int>(state[i * Nb]) << " " << static_cast<int>(state[(i * Nb) + 1]) << " " << static_cast<int>(state[(i * Nb) + 2]) << " " << static_cast<int>(state[(i * Nb) + 3]) << std::endl;
    }
    for (int i = 1; i < Nb; i++)
    {
        uint8_t *temp = new uint8_t[4];
        temp[0] = state[(i * Nb)];
        temp[1] = state[(i * Nb) + 1];
        temp[2] = state[(i * Nb) + 2];
        temp[3] = state[(i * Nb) + 3];
        state[(i * Nb)] = temp[i];
        state[(i * Nb) + 1] = temp[(i + 1) % Nb];
        state[(i * Nb) + 2] = temp[(i + 2) % Nb];
        state[(i * Nb) + 3] = temp[(i + 3) % Nb];
    }
    std::cout << "--------------------------------------------------" << std::endl;
    for (size_t i = 0; i < 4; i++)
    {
        std::cout << std::hex << static_cast<int>(state[i * Nb]) << " " << static_cast<int>(state[(i * Nb) + 1]) << " " << static_cast<int>(state[(i * Nb) + 2]) << " " << static_cast<int>(state[(i * Nb) + 3]) << std::endl;
    }
    std::cout << "------------------------ /shift rows --------------------------" << std::endl;
}

/**
 * Function that multiplies each column with mixConstant
 * 
 * may be wrong
 * */
void mixColumns()
{
    std::cout << "--------------------------- mix columns -----------------------" << std::endl;
    for (size_t i = 0; i < 4; i++)
    {
        std::cout << std::hex << static_cast<int>(state[i * Nb]) << " " << static_cast<int>(state[(i * Nb) + 1]) << " " << static_cast<int>(state[(i * Nb) + 2]) << " " << static_cast<int>(state[(i * Nb) + 3]) << std::endl;
    }
    for (int i = 0; i < Nb; i++)
    {
        uint8_t *temp = new uint8_t[4];
        temp[0] = state[(i)];
        temp[1] = state[(i + (1 * Nb))];
        temp[2] = state[(i + (2 * Nb))];
        temp[3] = state[(i + (3 * Nb))];

        for (int x = 0; x < Nb; x++)
        {
            uint8_t *temp_constant = new uint8_t[4];
            temp_constant[0] = mixConstant[(x * Nb)];
            temp_constant[1] = mixConstant[(x * Nb) + 1];
            temp_constant[2] = mixConstant[(x * Nb) + 2];
            temp_constant[3] = mixConstant[(x * Nb) + 3];
            state[(x * Nb) + i] = temp[0] * temp_constant[0] + temp[1] * temp_constant[1] + temp[2] * temp_constant[2] + temp[3] * temp_constant[3];
        }
    }
    std::cout << "--------------------------------------------------" << std::endl;
    for (size_t i = 0; i < 4; i++)
    {
        std::cout << std::hex << static_cast<int>(state[i * Nb]) << " " << static_cast<int>(state[(i * Nb) + 1]) << " " << static_cast<int>(state[(i * Nb) + 2]) << " " << static_cast<int>(state[(i * Nb) + 3]) << std::endl;
    }
    std::cout << "--------------------------- /mix columns -----------------------" << std::endl;
}

/**
 * Function that xor the round key stored in keySchedule, with the state
 * 
 * */
void addRoundKey(int round)
{
    std::cout << "-------------------------- addRoundKey ------------------------" << std::endl;
    for (size_t i = 0; i < 4; i++)
    {
        std::cout << std::hex << static_cast<int>(state[i * Nb]) << " " << static_cast<int>(state[(i * Nb) + 1]) << " " << static_cast<int>(state[(i * Nb) + 2]) << " " << static_cast<int>(state[(i * Nb) + 3]) << std::endl;
    }

    std::cout << "--------------------------------------------------" << std::endl;
    int state_incrementer = 0;
    for (int i = round * 16; i < ((round + 1) * 16); i++)
    {

        std::cout << "xor: " << std::hex << static_cast<int>(state[state_incrementer]) << " with " << std::hex << static_cast<int>(keySchedule[i]) << " = " << std::hex << static_cast<int>(state[state_incrementer] ^ keySchedule[i]) << std::endl;
        state[state_incrementer] = state[state_incrementer] ^ keySchedule[i];
        state_incrementer++;
    }
    std::cout << "--------------------------------------------------" << std::endl;
    for (size_t i = 0; i < 4; i++)
    {
        std::cout << std::hex << static_cast<int>(state[i * Nb]) << " " << static_cast<int>(state[(i * Nb) + 1]) << " " << static_cast<int>(state[(i * Nb) + 2]) << " " << static_cast<int>(state[(i * Nb) + 3]) << std::endl;
    }
    std::cout << "-------------------------- /addRoundKey ------------------------" << std::endl;
}

uint8_t *cipher(uint8_t *in)
{
    // copy plaintext to state variable
    for (int i = 0; i < Nb * 4; i++)
    {
        state[i] = in[i];
    }

    // mixColumns();

    // add round key first time outside of loop
    addRoundKey(0);
    // addRoundKey(1);
    // addRoundKey(2);
    // addRoundKey(3);
    // addRoundKey(4);

    for (int i = 1; i < Nr; i++)
    {
        // std::cout << "-------------------------- state before transformation " << i << " ------------------------" << std::endl;
        // for (size_t i = 0; i < 4; i++)
        // {
        //     std::cout << std::hex << static_cast<int>(state[i * Nb]) << " " << static_cast<int>(state[(i * Nb) + 1]) << " " << static_cast<int>(state[(i * Nb) + 2]) << " " << static_cast<int>(state[(i * Nb) + 3]) << std::endl;
        // }
        subBytes();
        shiftRows();
        mixColumns();
        addRoundKey(i);
    }
    subBytes();
    shiftRows();
    addRoundKey(Nr);
    return state;
}
int main(int argc, char **argv)
{
    char *buf = new char[KEY_SIZE + PLAINTEXT_MAX_SIZE];
    // cipher(argv[1]);

    // std::ifstream myfile("aes_sample.in");
    // myfile.read(buf, DEFAULT_BUF_LENGTH);
    // int size = myfile.gcount();

    std::cin.read(buf, KEY_SIZE + PLAINTEXT_MAX_SIZE);
    int size = std::cin.gcount();
    if (size == 0)
        std::cout << "hello aes" << std::endl;

    uint8_t *data = (uint8_t *)buf;
    for (int i = 0; i < KEY_SIZE / 4; i++)
    {
        key[i] = data[(i)*Nb];
        key[i + (1 * Nb)] = data[(i * Nb) + 1];
        key[i + (2 * Nb)] = data[(i * Nb) + 2];
        key[i + (3 * Nb)] = data[(i * Nb) + 3];
    }
    uint8_t *temp_plaintext = new uint8_t[16];
    for (int i = KEY_SIZE; i < size / sizeof(uint8_t); i++)
    {
        temp_plaintext[i - KEY_SIZE] = data[i];
    }
    for (int i = 0; i < 4; i++)
    {
        plaintext[i] = temp_plaintext[(i)*Nb];
        plaintext[i + (1 * Nb)] = temp_plaintext[(i * Nb) + 1];
        plaintext[i + (2 * Nb)] = temp_plaintext[(i * Nb) + 2];
        plaintext[i + (3 * Nb)] = temp_plaintext[(i * Nb) + 3];
    }

    std::cout << "-------------------------------------------------" << std::endl;

    std::cout << "key: " << std::endl;
    for (int i = 0; i < KEY_SIZE; i++)
    {
        std::cout << std::hex << static_cast<int>(data[i]) << " ";
    }

    std::cout << "\n-------------------------------------------------" << std::endl;

    std::cout << "plaintext: " << std::endl;
    for (int i = KEY_SIZE; i < size / sizeof(uint8_t) / sizeof(uint8_t); i++)
    {
        std::cout << std::hex << static_cast<int>(data[i]) << " ";
    }
    std::cout << "\n-------------------------------------------------" << std::endl;

    std::cout << "key: " << std::endl;
    for (size_t i = 0; i < 4; i++)
    {
        std::cout << std::hex << static_cast<int>(key[i * Nb]) << " " << static_cast<int>(key[(i * Nb) + 1]) << " " << static_cast<int>(key[(i * Nb) + 2]) << " " << static_cast<int>(key[(i * Nb) + 3]) << std::endl;
    }

    std::cout << "\n-------------------------------------------------" << std::endl;

    std::cout << "plaintext: " << std::endl;
    for (size_t i = 0; i < 4; i++)
    {
        std::cout << std::hex << static_cast<int>(plaintext[i * Nb]) << " " << static_cast<int>(plaintext[(i * Nb) + 1]) << " " << static_cast<int>(plaintext[(i * Nb) + 2]) << " " << static_cast<int>(plaintext[(i * Nb) + 3]) << std::endl;
    }
    std::cout << "\n-------------------------------------------------" << std::endl;

    std::cout << "performs key expansion" << std::endl;
    keyExpansion();

    std::cout << " KEY: " << std::endl;
    for (int x = 0; x < 4; x++)
    {
        for (int i = 0; i < Nb * (Nr + 1); i++)
        {
            std::cout << std::hex << static_cast<int>(keySchedule[i + (Nb * (Nr + 1)) * x]) << " ";
        }
        std::cout << "\n";
    }
    std::cout << " KEY: " << std::endl;

    for (int i = 0; i < 4*(Nb * (Nr + 1)); i++)
    {
        std::cout << std::hex << static_cast<int>(keySchedule[i]) << " ";
    }

    std::cout << "calulate cipher" << std::endl;
    uint8_t *ciph = cipher(plaintext);
    std::cout << "\n----------------------- cipher --------------------------" << std::endl;

    for (int i = 0; i < 16; i++)
    {
        std::cout << std::hex << static_cast<int>(ciph[i]) << " ";
    }
    std::cout << "\n-------------------------------------------------" << std::endl;
}