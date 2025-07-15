#pragma once
#include <string> // Include for handling large numbers as strings
#include "FiniteField.h"
#include "ECC.h"
#include "BigInteger.h"

class SECP256K1 : private ECC {
private:
	SECP256K1IFET<BigInteger> x;
	SECP256K1IFET<BigInteger> y;
	//int prime = 2 ^ 256 - 2 ^ 32 - 997;
	BigInteger base = BigInteger(2);
	BigInteger prime = base.myPow(256) - base.myPow(32) - BigInteger(977);
	//int prime = (1ULL << 256) - (1ULL << 32) - 977;
	//int prime = 213;
public:
	
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
