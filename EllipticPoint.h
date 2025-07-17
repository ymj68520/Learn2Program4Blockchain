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

		//if (r > 1) throw std::invalid_argument("Value has no modular inverse");
		if (r > 1) std::cout << "Warning: Value has no modular inverse" << std::endl;
		while (t < 0) t = t + mod;

		return t;
	}
public:
	T x; // x-coordinate
	T y; // y-coordinate
	int a; // Coefficient a of the elliptic curve equation
	int b; // Coefficient b of the elliptic curve equation
	T prime; // Prime number defining the finite field
	bool isInfinity; // Flag for point at infinity

	Point(T x, T y, int a, int b, T p) : x(x), y(y), a(a), b(b), prime(p), isInfinity(false) {
		// Check if the point satisfies the elliptic curve equation y^2 = x^3 + ax + b
		//try {
		if (x == 0 && y == 0) {
			isInfinity = true; // If x and y are both 0, it's a point at infinity
			return;
		}
		if ((y * y) % prime != (x * x * x + x * a + b) % prime) {
			isInfinity = false; // Otherwise, it's a finite point
			std::cout << "Warning: Point: (" << x << ", " << y << ") does not satisfy the elliptic curve equation" << std::endl;
		}
		//}
		/*catch (const std::invalid_argument& e) {
			std::cerr << "Exception caught: " << e.what() << std::endl;
		}*/
	} // Constructor for finite point
	//Point(T x, T y, int prime) : x(x), y(y), a(0), b(7), prime(prime), isInfinity(false) {
	//	// Check if the point satisfies the elliptic curve equation y^2 = x^3 + ax + b
	//	if ((y * y) % prime != (x * x * x + x * a + b) % prime) {
	//		//try {
	//		if ((y * y) % prime != (x * x * x + x * a + b) % prime) {
	//			std::cout << "Warning: Point: (" << x << ", " << y << ") does not satisfy the elliptic curve equation" << std::endl;
	//		}
	//		//}
	//		/*catch (const std::invalid_argument& e) {
	//			std::cerr << "Exception caught: " << e.what() << std::endl;
	//		}*/
	//	}
	//} // Constructor for finite point with default a and b
	Point() : x(0), y(0), a(0), b(7), prime(23), isInfinity(true) {} // Default constructor of secp256k1
	// Copy constructor
	Point(const Point& other) {
		x = other.x;
		y = other.y;
		a = other.a;
		b = other.b;
		prime = other.prime;
		isInfinity = other.isInfinity;
	}

	bool isPointAtInfinity() const { return isInfinity; } // Check if point is at infinity

	// Assignment Operator
	Point& operator=(const Point& other) {
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
	// Move Assignment Operator
	Point& operator=(Point&& other) noexcept {
		if (this != &other) {
			x = other.x;
			y = other.y;
			a = other.a;
			b = other.b;
			prime = other.prime;
			isInfinity = other.isInfinity;

			// ÇåÀí other µÄ×´Ì¬
			other.x = 0;
			other.y = 0;
			other.a = 0;
			other.b = 0;
			other.prime = 0;
			other.isInfinity = true;
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

		if (x == 0 && y == 0) return other; // If this point is at infinity, return the other point
		if (other.x == 0 && other.y == 0) return *this; // If the other point is at infinity, return this point

		// If both points are the same
		if (x == other.x && y == other.y) {
			if (y == 0) return Point(T(), T(), a, b, prime); // If y is 0, return point at infinity
			T slope = (3 * x * x + a) * modInverse(2 * y, prime) % prime; // Calculate slope
			T x3 = (slope * slope - 2 * x) % prime; // Calculate x-coordinate of the result
			T y3 = (slope * (x - x3) - y) % prime; // Calculate y-coordinate of the result
			// Check if the result is negative
			if (y3 < 0) y3 = y3 + prime; // Ensure y-coordinate is positive
			if (x3 < 0) x3 = x3 + prime; // Ensure x-coordinate is positive
			return Point(x3, y3, a, b, prime); // Return the resulting point
		}

		// If the points are negatives of each other, return point at infinity
		if (y == (0 - other.y)) return Point(T(), T(), a, b, prime);

		// infinity point
		if (isInfinity) return other; // If this point is at infinity, return the other point
		if (other.isInfinity) return *this; // If the other point is at infinity, return this point

		// normally add two points
		T slope = (other.y - y) * modInverse(other.x - x, prime) % prime; // Calculate slope
		T x3 = (slope * slope - x - other.x) % prime; // Calculate x-coordinate of the result
		T y3 = (slope * (x - x3) - y) % prime; // Calculate y-coordinate of the result
		// Check if the result is negative
		if (y3 < 0) y3 = y3 + prime; // Ensure y-coordinate is positive
		if (x3 < 0) x3 = x3 + prime; // Ensure x-coordinate is positive
		return Point(x3, y3, a, b, prime); // Return the resulting point
	}

	Point operator-(const Point& other) const {
		// secure the point
		/*if (!checkCurve(other)) {
			throw std::invalid_argument("Points are not on the same curve");
		}*/
		if (x == other.x && y == other.y) return Point(T(), T(), a, b, prime); // If both points are the same, return point at infinity

		return Point(other.x, (0 - other.y) % prime, a, b, prime); // Return the negation of the point
	}

	Point operator*(int scalar) const {
		if (scalar == 0) return Point(T(), T(), a, b, prime); // If scalar is 0, return point at infinity
		if (scalar == 1) return *this; // If scalar is 1, return the point itself
		//if (scalar < 0) return -(*this) * (-scalar); // If scalar is negative, negate the point and multiply by positive scalar
		Point result = Point(T(), T(), a, b, prime); // Start with the current point
		Point addend = *this; // Start with the current point as the addend
		while (scalar > 1) {
			if (scalar % 2 == 0) {
				addend = addend + addend; // Double the addend
				scalar /= 2; // Halve the scalar
			}
			else {
				result = result + addend; // Add the addend to the result
				scalar--; // Decrement the scalar
			}
		}
		return result + addend; // Return the final result
	}

	// Check if the points are on the same curve
	bool checkCurve(const Point& other) const {
		return (a == other.a && b == other.b && prime == other.prime);
	}
};