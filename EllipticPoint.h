#pragma once
#define _USE_ELLIPTIC_POINT_H 0x9991
#include <iostream>

template <typename T>
class Point {
private:
	static T modInverse(T value, int mod) {
		value = (value % mod + mod) % mod;
		T t = 0, newT = 1;
		int r = mod, newR = value;

		while (newR != 0) {
			int quotient = r / newR;
			T tempT = t;
			t = newT;
			newT = tempT - quotient * newT;

			int tempR = r;
			r = newR;
			newR = tempR - quotient * newR;
		}

		if (r > 1) throw std::invalid_argument("Value has no modular inverse");
		if (t < 0) t += mod;

		return t;
	}
public:
	T x; // x-coordinate
	T y; // y-coordinate
	int a; // Coefficient a of the elliptic curve equation
	int b; // Coefficient b of the elliptic curve equation
	int prime; // Prime number defining the finite field
	bool isInfinity; // Flag for point at infinity

	Point(T x, T y, int a, int b, int p) : x(x), y(y), a(a), b(b), prime(p), isInfinity(false) {
		// Check if the point satisfies the elliptic curve equation y^2 = x^3 + ax + b
		if ((y * y) % p != (x * x * x + a * x + b) % p) {
			throw std::invalid_argument("Point does not satisfy the elliptic curve equation");
		}
	} // Constructor for finite point
	Point(int a, int b, int p) : x(0), y(0), a(a), b(b), prime(p), isInfinity(true) {} // Constructor for point at infinity
	Point(T x, T y, int prime) : x(x), y(y), a(0), b(7), prime(prime), isInfinity(false) {
		// Check if the point satisfies the elliptic curve equation y^2 = x^3 + ax + b
		if ((y * y) % prime != (x * x * x + a * x + b) % prime) {
			throw std::invalid_argument("Point does not satisfy the elliptic curve equation");
		}
	} // Constructor for finite point with default a and b
	Point() : x(0), y(0), a(0), b(7), prime(23), isInfinity(true) {} // Default constructor of secp256k1
	// Copy constructor
	Point(const Point& other) : x(other.x), y(other.y), a(other.a), b(other.b), prime(other.prime), isInfinity(other.isInfinity) {}
	Point(const Point& other) : x(other.x), y(other.y), a(other.a), b(other.b), prime(other.prime) {}

	bool isPointAtInfinity() const { return isInfinity; } // Check if point is at infinity

	// Assignment Operator
	bool operator=(const Point& other) {
		if (this != &other) {
			x = other.x;
			y = other.y;
			a = other.a;
			b = other.b;
			prime = other.prime;
			isInfinity = other.isInfinity;
		}
		return *this;
	}
	// Overload the equality operator
	bool operator==(const Point& other) const {
		return (x == other.x && y == other.y && a == other.a && b == other.b
			&& prime == other.prime && isInfinity == other.isInfinity);
	}
	// Overload the inequality operator
	bool operator!=(const Point& other) const {
		return !(*this == other);
	}
	// Overload the addition operator
	Point operator+(const Point& other) const {
		// secure the point
		if (!checkCurve(other)) {
			throw std::invalid_argument("Points are not on the same curve");
		}

		// If both points are the same
		if (x == other.x && y == other.y) {
			if (y == 0) return Point(a, b, prime); // If y is 0, return point at infinity
			T slope = (3 * x * x + a) * modInverse(2 * y, prime) % prime; // Calculate slope
			T x3 = (slope * slope - 2 * x) % prime; // Calculate x-coordinate of the result
			T y3 = (slope * (x - x3) - y) % prime; // Calculate y-coordinate of the result
			return Point(x3, y3, a, b, prime); // Return the resulting point
		}

		// If the points are negatives of each other, return point at infinity
		if (y == (0 - other.y)) return Point(a, b, prime);

		// infinity point
		if (isInfinity) return other; // If this point is at infinity, return the other point
		if (other.isInfinity) return *this; // If the other point is at infinity, return this point

		// normally add two points
		T slope = (other.y - y) * modInverse(other.x - x, prime) % prime; // Calculate slope
		T x3 = (slope * slope - x - other.x) % prime; // Calculate x-coordinate of the result
		T y3 = (slope * (x - x3) - y) % prime; // Calculate y-coordinate of the result
		return Point(x3, y3, a, b, prime); // Return the resulting point
	}

	// Check if the points are on the same curve
	bool checkCurve(const Point& other) const {
		return (a == other.a && b == other.b && prime == other.prime);
	}
};