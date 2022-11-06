# Assignment 5 - Public Key Cryptography

This set of programs implements the RSA public-key cryptography, consisting of a key generator, an encryptor, and a decryptor. The key generator generates an RSA private key file as well as a public key file. The encryptor encrypts files using the public key, and the decryptor decrypts the encrypted file using the private key. These programs work together to encrypt and decrypt messages.

## Files
1. Decrypt.c - This contains the decrypt program's implementation and main()function.
2. Encrypt.c - This contains the implementation and main()function for the encrypt program.
3. Keygen.c - This contains the keygen program's implementation and main()function.
4. Numtheory.c - This contains the implementations of the number theory functions.
5. Numtheory.h - This specifies the interface for the number theory functions.
6. Randstate.h - This specifies the interface for initializing and clearing the random state.
7. Randstate.c - This contains the implementation of the random state interface for the RSA library and number theory functions. 
8. Rsa.c - This contains the implementation of the RSA library. 
9. Rsa.h - This specifies the interface for the RSA library. 
10. Makefile - Produces corresponding executables for keygen,encrypt, decrypt 
11. README.md - Description of the program usage
12. DESIGN.pdf - Describe design and design process of the program

## Building the program

```bash
make
```

## Usage

```python
./keygen
./encrypt -i in.txt -o out.txt
./decrypt -i out.txt -o in.txt
```
This generates the public and private key pair into some two files as default rsa.pub and rsa.priv files. Encrypting a file, requires a public key and take rsa.pub as default, which will encrypt it producing ciphertext in out.txt. Decrypting needs a private and public key it will take psa.priv as default, and will take the ciphertext, decrypt it back to its original state, and become a message.

```
./keygen -h
./encrypt -h
./decrypt -h
```
-h will print the help documentation of the three programs