# USC CSCI531 HW3

## Compile

type `make` or `make hw3` to generate executable.
type `make clean` to clear up *.o file and executable.

## Command

### generate key file: ./hw3 keygen -p=pphrase -t=period
Generate a keyfile with a specified period to be used by the full Vigenère cipher (with polyalphabetic substitution). The ASCII string pphrase contains the passphrase (which must not be empty) for key generation

### crypt: ./hw3 crypt -k=keyfile [file]
Encrypts (or decrypts) an input ASCII text file with the keys specified in keyfile. The output is an ASCII text file encrypted (or decrypted) using a full Vigenère cipher (with polyalphabetic substitution)

### inverse permutation key: ./hw3 invkey keyfile
Create an inverse permutation key file from keyfile.

### produce a histogram: ./hw3 histo -t=period -i=which [file]
Produce a histogram of ciphertext character frequencies assuming the ciphertext is produced by a polyalphabetic substitution cipher with period equals period. which specifies a one-based index for the alphabets. The output must be sorted according to the frequencies of the ciphertext characters in descending order. Please do not consider any characters that is not a lowercase English character (i.e., between 'a' and 'z', inclusive).

### solve: ./hw3 solve -l=max_t file
Apply the Method of Kasiski and use Index of Coincidence in order to determine the period of a ciphertext encrypted by a full Vigenère cipher (with polyalphabetic substitution).

## Cryptanalysis report

### mystery1.ct

### mystery2.ct
