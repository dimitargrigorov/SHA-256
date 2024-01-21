
/**
*
* Solution to course project # 6	
* Introduction to programming course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2023/2024
*
* @author Dimitar Grigorov
* @idnumber 7MI8000058
* @compiler VC
*
* SHA256 algorithm
*
*/

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

const unsigned wordlength = 32;

size_t bitsSize = 32;

const int universalSize = 512;

unsigned long long k = 1;

const unsigned ks0 = 15;

const unsigned ks1 = 2;

constexpr int sizeOfHash = 65;

const size_t sizeOfBlocks = 64;

char finalHash[sizeOfHash];

constexpr int sizeOfConst = 8;

unsigned hashValue[sizeOfConst]{ 0x6a09e667,0xbb67ae85,0x3c6ef372,0xa54ff53a,0x510e527f,0x9b05688c,0x1f83d9ab,0x5be0cd19 };

unsigned ROUND_CONSTANTS[64] = { 0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
								 0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
								 0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
								 0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
								 0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
								 0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
								 0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
								 0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2, };
void fromDecimalToBinary(int message, bool* number, int index, size_t size) {
	int i = index;
	unsigned digit = 0;
	number[0] = 0;
	while (message != 0) {
		digit = message % 2;
		number[i] = digit;
		i--;
		message /= 2;
	}

}

void convertStringToChar(string str, char*& arr, size_t sizeOfString) {
	for (int i = 0; i < sizeOfString; i++) {
		arr[i] = str[i];
	}
	arr[sizeOfString] = '\0';
}

string convertCharToString(char* a, int size)
{
	int i;
	string s = "";
	for (i = 0; i < size; i++) {
		s = s + a[i];
	}
	return s;
}

char getSymbolFromIndex(int ind)
{
	if (ind >= 0 && ind <= 9)
		return ind + '0';

	if (ind >= 10)
		return (ind - 10) + 'a';

	return 0;
}






void fromDecimaToHexadecimal(unsigned n, char*& to, size_t size, unsigned k,unsigned steps) {
	while (steps!= 0) {
		int ind = n % k;
		char current = getSymbolFromIndex(ind);
		to[size] = current;
		size--;
		n /= k;
		steps--;
	}
}






unsigned myStrLen(const char* str) {
	if (!str) {
		return 0;
	}
	unsigned count = 0;
	while (*str) {
		count++;
		str++;
	}
	return count;
}

unsigned int fromBinaryToDecimal(bool*& padArr, unsigned steps) {
	
	unsigned int result = 0;
	unsigned int mult = 1;
	while (steps != 0)
	{
		result += ((*padArr) * mult);
		mult *= 2;
		steps--;
		padArr--;
	}
	return result;
}


void converPadToNumbers(bool* padArr, unsigned* block, size_t words, unsigned numberOfBlocks) {  
	unsigned const wordlength = 32;
	padArr += (universalSize * numberOfBlocks - 1);
	for (int i = words - 1; i >= 0; i--) {
		block[i] = fromBinaryToDecimal(padArr, wordlength);
	}
}

unsigned countingBits(int ch) {
	unsigned count = 0;
	while (ch != 0) {
		count++;
		ch /= 2;
	}
	return count;
}


unsigned rightRotate(unsigned int n, unsigned int d) {  
	return(n >> d) | (n << (bitsSize - d));
}
int lefthRotate(unsigned n, unsigned int d) { 
	return(n << d) | (n >> (bitsSize - d));
}

void modifiyZero(unsigned* block, size_t blockSize) {
	unsigned s0 = 0;
	unsigned s1 = 0;
	for (int i = 16; i < 64; i++) {
		s0 = (rightRotate(block[i - ks0], 7) ^ rightRotate(block[i - ks0], 18) ^ ((block[i - ks0]) >> 3));
		s1 = (rightRotate(block[i - ks1], 17) ^ rightRotate(block[i - ks1], 19) ^ ((block[i - ks1]) >> 10));
		block[i] = (block[i - 16] + s0 + block[i - 7] + s1) % (k << 32);

	}

}

void compression(unsigned* block) {
	unsigned a = hashValue[0], b = hashValue[1], c = hashValue[2], d = hashValue[3],
			 e = hashValue[4], f = hashValue[5], g = hashValue[6], h = hashValue[7];

	unsigned S1 = 0;
	unsigned ch = 0;
	unsigned temp1 = 0;
	unsigned temp2 = 0;
	unsigned maj = 0;
	unsigned S0 = 0;
	for (int i = 0; i < 64; i++) {
		S1 = (rightRotate(e, 6) ^ rightRotate(e, 11) ^ rightRotate(e, 25));
		ch = (e & f) ^ ((~e) & g);
		temp1 = (h + S1 + ch + ROUND_CONSTANTS[i] + block[i]) % (k << 32);
		S0 = (rightRotate(a, 2) ^ rightRotate(a, 13) ^ rightRotate(a, 22));
		maj = (a & b) ^ (a & c) ^ (b & c);
		temp2 = (S0 + maj) % (k << 32);
		h = g;
		g = f;
		f = e;
		e = (d + temp1) % (k << 32);
		d = c;
		c = b;
		b = a;
		a = (temp1 + temp2) % (k << 32);
    }

	hashValue[0] = (hashValue[0] + a) % (k << 32);
	hashValue[1] = (hashValue[1] + b) % (k << 32);
	hashValue[2] = (hashValue[2] + c) % (k << 32);
	hashValue[3] = (hashValue[3] + d) % (k << 32);
	hashValue[4] = (hashValue[4] + e) % (k << 32);
	hashValue[5] = (hashValue[5] + f) % (k << 32);
	hashValue[6] = (hashValue[6] + g) % (k << 32);
	hashValue[7] = (hashValue[7] + h) % (k << 32);
}

void concatenateFinaleCash(unsigned* hashValue, char* finalHash) {
	unsigned startIndex = 7;
	unsigned steps = 8;
	unsigned toHexadecimal = 16;
	for (int i = 0; i < sizeOfConst; i++) {
		fromDecimaToHexadecimal(hashValue[i], finalHash, startIndex, toHexadecimal, steps);
		startIndex += 8;

	}
	finalHash[64] = '\0';
	string hash;
	hash = convertCharToString(finalHash,64);

	fstream myfile;
	myfile.open ("sha - 256.txt");
	if (myfile.is_open()) {
		myfile << hash << endl;
	}
	myfile.close();


}

void createBlocks(unsigned* padBlock, size_t sizeOfPadBlock, unsigned* newBlock, unsigned& currentIndex) {
	for (int i = 0; i < sizeOfPadBlock; i++) {
		newBlock[i] = padBlock[currentIndex];
		currentIndex++;
	}
}

void padingBits(const bool* message, bool* padArr, size_t messageSize, unsigned numberOfBlock) {
	size_t numberOfBits = countingBits(messageSize);
	unsigned numberOfZeros = universalSize * numberOfBlock - messageSize - 1 - 64;
	bool* bitsOfSize = new bool[numberOfBits] {0};

	fromDecimalToBinary(messageSize, bitsOfSize, numberOfBits - 1, numberOfBits);
	for (int i = 0; i < messageSize; i++) {
		padArr[i] = message[i];
	}
	padArr += messageSize;
	*padArr = 1;
	padArr += (numberOfZeros + 1 + 64 - numberOfBits);
	for (int i = 0; i < numberOfBits; i++) {
		*padArr = bitsOfSize[i];
		padArr++;
	}
	padArr -= universalSize * numberOfBlock - 1;
	delete[]bitsOfSize;
}
void hashing(bool* mesage, size_t messageSize) {

	size_t numberOfBlocks = (messageSize + 1 + 64) / 512 + 1;
	size_t  padSize = numberOfBlocks * universalSize;
	size_t sizeOfBlock = padSize / wordlength;

	unsigned* transformBlock = new unsigned[sizeOfBlock];
	bool* padArr = new bool[padSize] {0};
	unsigned numberOfWord = padSize / wordlength;

	padingBits(mesage, padArr, messageSize, numberOfBlocks);
	converPadToNumbers(padArr, transformBlock, numberOfWord, numberOfBlocks);
	unsigned currentIndex = 0;
	for (int i = 0; i < numberOfBlocks; i++) {
		unsigned* block = new unsigned[64] {0};
		createBlocks(transformBlock, 16, block, currentIndex);
		modifiyZero(block, 64);
		compression(block);
		delete[]block;

	}
	concatenateFinaleCash(hashValue, finalHash);


	delete[]transformBlock;
	delete[]padArr;
}
void sha(char* text) {
	if (!text) {
		return;
	}
	unsigned size = myStrLen(text) * 8;
	bool* bits = new bool[size] {0};
	int index = 7;
	while (*text) {
		fromDecimalToBinary(*text, bits, index, size);
		index += 8;
		text++;
	}
	hashing(bits, size);
}

void validHash(string inputHash) {
	string hash;
	ifstream myfile;
	myfile.open("sha - 256.txt");
	if (myfile.is_open()) {
		myfile >> hash;
	}
	myfile.close();
	for (int i = 0; i < sizeOfHash; i++) {
		if (inputHash[i] != hash[i]) {
			cout << "Invalid Hash!";
			return;
		}
	}
	cout << "Hash is valid";
}
int main()
{

	string str;
	string result;
	string inputHash;
	ifstream myfile;
	myfile.open("sha - 256.txt");
	if (myfile.is_open()) {
		myfile >> str;
	}
	myfile.close ();
	std::cin >> inputHash;
	size_t sizeOfString = str.length();
	char* arr = new char[sizeOfString + 1];
	convertStringToChar(str, arr, sizeOfString);
	sha(arr);
	validHash(inputHash);


}

