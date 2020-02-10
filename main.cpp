#pragma once
#include <iostream>
#include <fstream>
#include "aes.cpp"

int main(int argc, char const *argv[])
{
    char *buf = new char[KEY_SIZE + PLAINTEXT_SIZE];

    // cin.read(char *buffer, int n): Reads n bytes (or until the end of the file) from the stream into the buffer.

    std::cin.read(buf, KEY_SIZE + PLAINTEXT_SIZE);
    int size = std::cin.gcount();
    if (size == 0)
        std::cout << "no input!" << std::endl;

    uint8_t *data = (uint8_t *)buf;
    for (int i = 0; i < KEY_SIZE; i++)
    {
        key[i] = data[i];
    }
    uint8_t *temp_plaintext = new uint8_t[16];
    for (int i = KEY_SIZE; i < size; i++)
    {
        temp_plaintext[i - KEY_SIZE] = data[i];
    }
    for (int i = 0; i < 16; i++)
    {
        plaintext[i] = temp_plaintext[i];
    }

    std::cout << "-------------------------------------------------" << std::endl;

    std::cout << "key: " << std::endl;
    for (int i = 0; i < KEY_SIZE; i++)
    {
        std::cout << std::hex << static_cast<int>(data[i]) << " ";
    }
    std::cout << "key: " << std::endl;

    print_block(key);

    std::cout << "\n-------------------------------------------------" << std::endl;

    std::cout << "plaintext: " << std::endl;
    for (int i = KEY_SIZE; i < size; i++)
    {
        std::cout << std::hex << static_cast<int>(data[i]) << " ";
    }
    std::cout << "plaintext: " << std::endl;

    print_block(plaintext);

    delete[] temp_plaintext;
    delete[] data;

    std::cout << "\n-------------------------------------------------" << std::endl;

    KeyExpansion();

    std::cout << "Expanded Key: " << std::endl;
    for (int i = 0; i < Nb * (Nr + 1); i++)
    {
        std::cout << std::hex << static_cast<int>(key_schedule[i]) << " ";
    }
    std::cout << "\n-------------------------------------------------" << std::endl;

    std::cout << "--------------- plaintext ---------------" << std::endl;
    uint8_t *c = Cipher(plaintext);
    std::cout << "--------------- cipher ---------------" << std::endl;
    print_block(c);
    delete[] c;
    delete[] state;
    delete[] key;
    delete[] key_schedule;
    delete[] plaintext;
    delete[] Rcon;
    delete[] S_box;

    return 0;
}