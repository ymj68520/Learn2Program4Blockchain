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
	// 循环群的阶
	const static BigInteger N;
	SECP256K1IFET<BigInteger> x, y;
	Point<SECP256K1IFET<BigInteger>> P;

public:
	SECP256K1():x(Gx), y(Gy){
		// Initialize the base point P with Gx and Gy
		P = Point<SECP256K1IFET<BigInteger>>(x, y, a, b, prime);
	}
	// Assignment operator
	SECP256K1& operator=(const SECP256K1& other) {
		if (this != &other) {
			ECC::operator=(other);
			x = other.x;
			y = other.y;
			P = other.P;
		}
		return *this;
	}
};

// 重构FiniteFieldElement，弃用此类
//template<typename T>
//class SECP256K1IFET : private FiniteFieldElement {
//private:
//	T value;
//	T prime;
//
//public:
//	FiniteFieldElement(const T& num, const T& prime) : value(num), prime(prime) {
//		if (num < 0 || num >= prime) {
//			throw std::invalid_argument("num must be in the range 0 to prime-1");
//		}
//	}
//};