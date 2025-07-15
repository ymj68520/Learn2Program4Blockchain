#pragma once

#ifndef _USE_ELLIPTIC_POINT_H
#include "EllipticPoint.h"
#endif // !_USE_ELLIPTIC_POINT_H

#ifndef _USE_FINITE_FIELD_H
#include "FiniteField.h"
#endif // !_USE_FINITE_FIELD_H

#ifndef _USE_ECC_H
#define _USE_ECC_H 0x9993
#endif // !_USE_ECC_H

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