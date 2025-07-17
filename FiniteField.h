#pragma once
#ifndef _USE_FINITE_FIELD_
#define _USE_FINITE_FIELD_H 0x9992
#endif // !_USE_FINITE_FIELD_

#include <iostream>
#include <climits>
#include "BigInteger.h"

class FiniteFieldElement {
private:
	// The value of the element in the finite field
	BigInteger value;
	// The prime number that defines the finite field
	BigInteger prime = 223;

public:
	// Constructor:
	FiniteFieldElement() : value(0) {} // Default constructor
	// Constructor with prime:
	FiniteFieldElement(BigInteger value) : value(value) {
		while (value < 0) value += prime;
		while (value >= prime) value -= prime;
	}
	// Creates a finite field element with the given value and prime.
	FiniteFieldElement(BigInteger num, BigInteger prime) {
		if (num < 0 || num >= prime) {
			throw std::invalid_argument("num must be in the range 0 to prime-1");
		}
		this->value = num;
		this->prime = prime;
	}
	// Copy constructor:
	FiniteFieldElement(const FiniteFieldElement& other) {
		value = other.value;
		prime = other.prime;
	}
	// Copy assignment operator:
	FiniteFieldElement& operator=(const FiniteFieldElement& other) {
		if (this != &other) {
			value = other.value;
			prime = other.prime;
		}
		return *this;
	}
	FiniteFieldElement& operator= (const int scalar) {
		value = scalar;

		while (value < 0) value += prime;
		while (value >= prime) value -= prime;

		return *this;
	}
	operator int() const {
		return value.toInt();
	}

	// Addition, subtraction, multiplication, and division operators:
	FiniteFieldElement operator+(const FiniteFieldElement& other) const {
		return FiniteFieldElement((value + other.getValue()) % prime, prime);
	}
	FiniteFieldElement operator+(const int scalar) const {
		return FiniteFieldElement((value + scalar) % prime, prime);
	}
	friend FiniteFieldElement operator+(const int scalar, const FiniteFieldElement& other) {
		return FiniteFieldElement((scalar + other.getValue()) % other.getPrime(), other.getPrime());
	}
	FiniteFieldElement operator-(const FiniteFieldElement& other) const {
		return FiniteFieldElement((value - other.getValue() + prime) % prime, prime);
	}
	FiniteFieldElement operator-(const int scalar) const {
		return FiniteFieldElement((value - scalar + prime) % prime, prime);
	}
	friend FiniteFieldElement operator-(const int scalar, const FiniteFieldElement& other) {
		return FiniteFieldElement((scalar - other.getValue() + other.getPrime()) % other.getPrime(), other.getPrime());
	}
	FiniteFieldElement operator*(const FiniteFieldElement& other) const {
		return FiniteFieldElement((value * other.getValue()) % prime, prime);
	}
	FiniteFieldElement operator*(const int scalar) const {
		return FiniteFieldElement((value * scalar) % prime, prime);
	}
	friend FiniteFieldElement operator*(const int scalar, const FiniteFieldElement& other) {
		return FiniteFieldElement((scalar * other.getValue()) % other.getPrime(), other.getPrime());
	}
	FiniteFieldElement operator/(const FiniteFieldElement& other) const {
		return *this * other.Inverse();
	}
	FiniteFieldElement operator/(const int scalar) const {
		return *this * FiniteFieldElement(scalar, prime).Inverse();
	}

	// modulus operator:
	FiniteFieldElement operator%(const BigInteger prime) const {
		return FiniteFieldElement(value % prime, prime);
	}

	// Comparison operators:
	bool operator<(const FiniteFieldElement& other) const {
		return value < other.getValue();
	}
	bool operator<(const int scalar) const {
		return value < scalar;
	}
	bool operator>(const FiniteFieldElement& other) const {
		return value > other.getValue();
	}
	bool operator>(const int scalar) const {
		return value > scalar;
	}
	bool operator==(const FiniteFieldElement& other) const {
		// Lambda function to check equality of two FiniteFieldElement objects.
		// It compares both the value and the prime of the current object (*this)
		// with the corresponding attributes of the other object.
		auto __eq__ = [this](const FiniteFieldElement& other) -> bool {
			return value == other.getValue() && prime == other.getPrime();
			};
		return __eq__(other);
	}
	bool operator==(const int scalar) const {
		return value == BigInteger(scalar);
	}
	// Inequality operator:
	bool operator!=(const FiniteFieldElement& other) const {
		// Lambda function to check inequality of two FiniteFieldElement objects.
		// It compares both the value and the prime of the current object (*this)
		// with the corresponding attributes of the other object.
		auto __ne__ = [this](const FiniteFieldElement& other) -> bool {
			return value != other.getValue() || prime != other.getPrime();
			};
		return __ne__(other);
	}
	bool operator!=(const int scalar) const {
		return value != BigInteger(scalar);
	}

	// Power function:
	// C++ does not support overloading power operators.
	FiniteFieldElement Epow(BigInteger p) const {
		if (p < 0) {
			throw std::invalid_argument("Exponent must be non-negative");
		}
		BigInteger result = 1;
		for (BigInteger i = 0; i < p; ++i) {
			result = (result * value) % prime;
		}
		return FiniteFieldElement(result, prime);
	}
	// Power function with negative exponent:
	FiniteFieldElement pow(BigInteger p) const {
		BigInteger fp = p;
		if (fp < 0) {
			fp = - fp;
		}
		auto result = Epow(fp);
		if (p != fp) {
			result = result.Inverse();
		}
		return result;
	}
	// Inverse Element Finder:
	FiniteFieldElement Inverse() const {
		for (BigInteger i = 1; i < prime; ++i) {
			if ((value * i) % prime == 1) {
				return FiniteFieldElement(i, prime);
			}
		}
		throw std::runtime_error("No inverse exists");
	}
	// Fermat's Little Theorem states that if p is prime and a is not divisible by p,
	// then a^(p-1) กิ 1 (mod p). Thus, a^(p-2) กิ a^(-1) (mod p).
	FiniteFieldElement FematsInverse() const {
		return Epow(prime - 2);
	}
	// Output Stream Operator:
	// Use friend to allow access to private members of FiniteFieldElement
	friend std::ostream& operator<<(std::ostream& os, const FiniteFieldElement& element) {
		os << element.getValue();
		return os;
	}
	// getters:
	BigInteger getValue() const {
		return value;
	}
	BigInteger getPrime() const {
		return prime;
	}
};
