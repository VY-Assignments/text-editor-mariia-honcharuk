//
//  cipher.h
//  OOP_Cipher_Project
//
//  Created by Maria Goncharuk on 14.06.2026.
//
#ifndef CIPHER_H
#define CIPHER_H

#include <string>

class Cipher {
public:
    virtual ~Cipher() = default;
    
    virtual std::string encrypt(const std::string& text) = 0;
    virtual std::string decrypt(const std::string& text) = 0;
};

class CaesarCipher : public Cipher {
    int key;
public:
    CaesarCipher(int key);
    std::string encrypt(const std::string& text) override;
    std::string decrypt(const std::string& text) override;
};

class VigenereCipher : public Cipher {
    std::string key;
public:
    VigenereCipher(const std::string& key);
    std::string encrypt(const std::string& text) override;
    std::string decrypt(const std::string& text) override;
};

class AtbashCipher : public Cipher {
public:
    AtbashCipher();
    std::string encrypt(const std::string& text) override;
    std::string decrypt(const std::string& text) override;
};

#endif // CIPHER_H
