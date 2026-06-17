//
//  main.cpp
//  OOP_Cipher_Project
//
//  Created by Maria Goncharuk on 14.06.2026.
//

#include <iostream>
#include <string>
#include <dlfcn.h>

int main(int argc, const char * argv[]) {
    void* handle = dlopen("./libcipher.dylib", RTLD_LAZY);
    if (!handle) {
        std::cerr << "Error with library" << dlerror() << std::endl;
        return 1;
    }
    typedef void* cipher_t;
    typedef cipher_t (*create_caesar_fn)(int);
    typedef cipher_t (*create_vigenere_fn)(const char*);
    typedef cipher_t (*create_atbash_fn)();
    typedef char* (*encrypt_fn)(cipher_t, const char*);
    typedef char* (*decrypt_fn)(cipher_t, const char*);
    typedef void (*destroy_fn)(cipher_t);
    typedef void (*free_fn)(char*);
    
    create_caesar_fn create_caesar = reinterpret_cast<create_caesar_fn>(dlsym(handle, "cipher_create_caesar"));
    encrypt_fn encrypt = reinterpret_cast<encrypt_fn>(dlsym(handle, "cipher_encrypt"));
    destroy_fn destroy = reinterpret_cast<destroy_fn>(dlsym(handle, "cipher_destroy"));
    free_fn free_str = (free_fn)dlsym(handle, "cipher_free");
    if (!create_caesar || !encrypt || !destroy) {
        std::cerr << "Error with library" << std::endl;
        dlclose(handle);
        return 1;
    }
    cipher_t cipher = create_caesar(3);
    const char* text = "Hello, World!";
    char* encryptedText = encrypt(cipher, text);
    std::cout << "New text: " << encryptedText << std::endl;
    free_str(encryptedText);
    destroy(cipher);
    dlclose(handle);
    return 0;
}
