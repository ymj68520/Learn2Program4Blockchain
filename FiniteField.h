#pragma once
#define _USE_FINITE_FIELD_H 0x9992
#include <iostream>
#include <climits>

class FiniteFieldElement {
private:
	// The value of the element in the finite field
	int value;
	// The prime number that defines the finite field
	int prime;

public:
	// Constructor:
	FiniteFieldElement() : value(0), prime(1) {} // Default constructor
	// Constructor with prime:
	FiniteFieldElement(int prime) : value(0), prime(prime) {
		if (prime <= 1) {
			throw std::invalid_argument("prime must be greater than 1");
		}
	}
	// Creates a finite field element with the given value and prime.
	FiniteFieldElement(int num, int prime) {
		if (num < 0 || num >= prime) {
			throw std::invalid_argument("num must be in the range 0 to prime-1");
		}
		this->value = num;
		this->prime = prime;
	}
	// Destructor:
	~FiniteFieldElement() {
		value = INT_MIN;
		prime = 0;
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

	// Addition, subtraction, multiplication, and division operators:
	FiniteFieldElement operator+(const FiniteFieldElement& other) const {
		return FiniteFieldElement((value + other.getValue()) % prime, prime);
	}
	FiniteFieldElement operator+(const int scalar) const {
		return FiniteFieldElement((value + scalar) % prime, prime);
	}
	FiniteFieldElement operator-(const FiniteFieldElement& other) const {
		return FiniteFieldElement((value - other.getValue() + prime) % prime, prime);
	}
	FiniteFieldElement operator-(const int scalar) const {
		return FiniteFieldElement((value - scalar + prime) % prime, prime);
	}
	FiniteFieldElement operator*(const FiniteFieldElement& other) const {
		return FiniteFieldElement((value * other.getValue()) % prime, prime);
	}
	FiniteFieldElement operator*(const int scalar) const {
		return FiniteFieldElement((value * scalar) % prime, prime);
	}
	FiniteFieldElement operator/(const FiniteFieldElement& other) const {
		return *this * other.Inverse();
	}
	FiniteFieldElement operator/(const int scalar) const {
		return *this * FiniteFieldElement(scalar, prime).Inverse();
	}

	// modulus operator:
	FiniteFieldElement operator%(const int prime) const {
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

	// Power function:
	// C++ does not support overloading power operators.
	FiniteFieldElement Epow(int p) const {
		if (p < 0) {
			throw std::invalid_argument("Exponent must be non-negative");
		}
		int result = 1;
		for (int i = 0; i < p; ++i) {
			result = (result * value) % prime;
		}
		return FiniteFieldElement(result, prime);
	}
	// Power function with negative exponent:
	FiniteFieldElement pow(int p) const {
		int fp = p;
		if (fp < 0) {
			fp = 0 - fp;
		}
		auto result = Epow(fp);
		if (p != fp) {
			result = result.Inverse();
		}
		return result;
	}
	// Inverse Element Finder:
	FiniteFieldElement Inverse() const {
		for (int i = 1; i < prime; ++i) {
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
	int getValue() const {
		return value;
	}
	int getPrime() const {
		return prime;
	}
};
