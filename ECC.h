#pragma once

#ifndef _USE_ELLIPTIC_POINT_H
#include "EllipticPoint.h"
#endif // !_USE_ELLIPTIC_POINT_H

#ifndef _USE_FINITE_FIELD_H
#include "FiniteField.h"
#endif // !_USE_FINITE_FIELD_H

class ECC {
private:
	FiniteFieldElement x;
	FiniteFieldElement y;
	int a = 0;
	int b = 7;
	int prime = 223;
public:
	Point<FiniteFieldElement> gPoint;
	int order = 0; // Order of the point
	ECC() :x(0, prime), y(0, prime), a(0), b(7), prime(223) {
		gPoint = Point<FiniteFieldElement>(x, y, a, b, prime);
	}
	ECC(const FiniteFieldElement& x, const FiniteFieldElement& y, int a, int b, int prime)
		: x(x), y(y), a(a), b(b), prime(prime) {
		if ((y * y) % prime != (x * x * x + a * x + b) % prime) {
			//throw std::invalid_argument("Point does not satisfy the elliptic curve equation");
			std::cout << "Warning: Point: (" << x.getValue() << ", " << y.getValue()
				<< ") does not satisfy the elliptic curve equation" << std::endl;
		}
		gPoint = Point<FiniteFieldElement>(x, y, a, b, prime);
		order = calOrder();
	}
	ECC(const ECC& other) : x(other.x), y(other.y), a(other.a), b(other.b), prime(other.prime),
		gPoint(other.gPoint), order(other.order) {
	}

	ECC& operator=(const ECC& other) {
		if (this != &other) {
			x = other.x;
			y = other.y;
			a = other.a;
			b = other.b;
			prime = other.prime;
			gPoint = other.gPoint;
			order = other.order;
		}
		return *this;
	}

	int calOrder() {
		int order = 1;
		Point<FiniteFieldElement> point = gPoint;
		while (point.x != 0 || point.y != 0) {
			point = point + gPoint;
			order++;
		}
		return order;
	}
};

#include <string> // Include for handling large numbers as strings
// 无法使用，推荐替换int为大整数类型
class SECP256K1 : private ECC {
private:
	FiniteFieldElement x;
	FiniteFieldElement y;
	//int prime = 2 ^ 256 - 2 ^ 32 - 997;
	//int prime = (1ULL << 256) - (1ULL << 32) - 977;
	int prime = 213;
public:
	int order = parseHexToDecimal("fffffffffffffffffffffffffffffffebaaedce6af48a03bbfd25e8cd0364141");
	SECP256K1()
		: x(FiniteFieldElement(
			parseHexToDecimal("79be667ef9dcbbac55a06295ce870b07029bfcdb2dce28d959f2815b16f81798"),
			prime)),
		y(FiniteFieldElement(
			parseHexToDecimal("483ada7726a3c4655da4fbfc0e1108a8fd17b448a68554199c47d08ffb10d4b8"),
			prime))
	{
	}

	// Helper function to parse large hexadecimal numbers into decimal
	static int parseHexToDecimal(const std::string& hex) {
		int decimalValue = 0;
		for (char c : hex) {
			decimalValue = decimalValue * 16 + (isdigit(c) ? c - '0' : toupper(c) - 'A' + 10);
		}
		return decimalValue;
	}
};