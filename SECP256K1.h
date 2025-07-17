#pragma once
#include <string> // Include for handling large numbers as strings
#include "FiniteField.h"
#include "ECC.h"
#include "BigInteger.h"

class SECP256K1 : private ECC {
private:
	const static SECP256K1IFET<BigInteger> Gx;
	const static SECP256K1IFET<BigInteger> Gy;
	const static BigInteger baseX;
	const static BigInteger baseY;
	// int base = 2;
	const static BigInteger base;
	const int a = 0;
	const int b = 7;

public:
	//int prime = 2 ^ 256 - 2 ^ 32 - 997;
	const static BigInteger prime;
	// Ñ­»·ÈºµÄ½×
	const static BigInteger N;
};

template<typename T>
class SECP256K1IFET : private FiniteFieldElement {
private:
	T value;
	T prime;
public:

	FiniteFieldElement(T num, T prime) {
		if (num < 0 || num >= prime) {
			throw std::invalid_argument("num must be in the range 0 to prime-1");
		}
		this->value = num;
		this->prime = prime;
	}
};
