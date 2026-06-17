//
//  cipher.cpp
//  OOP_Cipher_Project
//
//  Created by Maria Goncharuk on 16.06.2026.
//
#include "cipher.h"
#include "cipher_api.h"
#include <cctype>
#include <cstring>
#include <string>

CaesarCipher::CaesarCipher(int k) {
    key = (k % 26 + 26) % 26;
}
VigenereCipher::VigenereCipher(const std::string& k) {
    key = k;
}

AtbashCipher::AtbashCipher() {
}

std::string CaesarCipher::encrypt(const std::string& text) {
    std::string result = "";
    for (char c : text) {
        if (isalpha(c)) {
            char base;
            if (isupper(c)) {
                base = 'A';
            } else {
                base = 'a';
            }
            result += (char)((c - base + key) % 26 + base);
        } else {
            result += c;
        }
    }
    return result;
}

std::string CaesarCipher::decrypt(const std::string& text) {
    CaesarCipher reverseCipher(-this->key);
    return reverseCipher.encrypt(text);
}

std::string VigenereCipher::encrypt(const std::string& text) {
    std::string result = "";
    int keyIndex = 0;
    for (char c : text) {
        if (isalpha(c)) {
            char textBase;
            char keyBase;
            char currentKey = key[keyIndex % key.length()];
            if (isupper(c)) {
                textBase = 'A';
            } else {
                textBase = 'a';
            }
            if (isupper(currentKey)){
                keyBase = 'A';
            } else {
                keyBase = 'a';
            }
            int shiftAmount = currentKey - keyBase;
            char encryptedChar = (c - textBase + shiftAmount) % 26 + textBase;
            result += encryptedChar;
            keyIndex++;
        } else {
            result += c;
        }
    }
    return result;
}

std::string VigenereCipher::decrypt(const std::string& text) {
    std::string result = "";
    int keyIndex = 0;
    for (char c : text) {
        if (isalpha(c)) {
            char textBase;
            char keyBase;
            char currentKey = key[keyIndex % key.length()];
            if (isupper(c)) {
                textBase = 'A';
            } else {
                textBase = 'a';
            }
            if (isupper(currentKey)){
                keyBase = 'A';
            } else {
                keyBase = 'a';
            }
            int shiftAmount = currentKey - keyBase;
            char encryptedChar = (c - textBase - shiftAmount +26) % 26 + textBase;
            result += encryptedChar;
            keyIndex++;
        } else {
            result += c;
        }
    }
    return result;
}

std::string AtbashCipher::encrypt(const std::string& text) {
    std::string result = "";
    
    for (char c : text) {
        if (isalpha(c)) {
            if (isupper(c)) {
                result += 'Z' - (c - 'A');
            } else {
                result += 'z' - (c - 'a');
            }
        } else {
            result += c;
        }
    }
    return result;
}

std::string AtbashCipher::decrypt(const std::string& text) {
    return encrypt(text);
}

extern "C"{
    EXPORT cipher_t cipher_create_caesar(int key) {
        return new CaesarCipher(key);
    }
    EXPORT cipher_t cipher_create_vigenere(const char* key) {
        return new VigenereCipher(key);
    }

    EXPORT char* cipher_encrypt(cipher_t cipher, const char* text) {
        Cipher* c = static_cast<Cipher*>(cipher);
        std::string encryptedStr = c->encrypt(text);
        return strdup(encryptedStr.c_str());
    }
    EXPORT char* cipher_decrypt(cipher_t cipher, const char* text){
        Cipher* c = static_cast<Cipher*>(cipher);
        std::string decryptedStr = c->decrypt(text);
        return strdup(decryptedStr.c_str());
    }
    EXPORT void cipher_destroy(cipher_t cipher) {
        Cipher* c = static_cast<Cipher*>(cipher);
        delete c;
    }
    EXPORT cipher_t cipher_create_atbash() {
        return new AtbashCipher();
    }
}
