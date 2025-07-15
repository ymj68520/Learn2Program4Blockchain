#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

class BigInteger {
public:
	enum class Sign {
		NEGATIVE = '-', ZERO = '0', POSITIVE = '+'
	};

private:
	std::string value; // Store the number as a string for arbitrary precision
	Sign sign;

private:
	static Sign signExpressSolve(const std::string& str); // Helper function to determine the sign of a string representation of a number
	static size_t signLastIndex(const std::string& str);
	void signChange() {
		if (sign == Sign::NEGATIVE) {
			sign = Sign::POSITIVE;
		}
		else if (sign == Sign::POSITIVE) {
			sign = Sign::NEGATIVE;
		}
	}
	static int absCompare(const BigInteger& lhs, const BigInteger& rhs) {
		if (lhs.value.length() < rhs.value.length()) return -1;
		if (lhs.value.length() > rhs.value.length()) return 1;
		if (lhs.value < rhs.value) return -1;
		if (lhs.value > rhs.value) return 1;
		return 0;
	}
	BigInteger _pow(const BigInteger& num1, const int& index);
	//void _parseHexToDecimal() {}; // 函数内部自解析函数

public:
	BigInteger(); // Default constructor for zero
	BigInteger(const long num); // Constructor from long
	BigInteger(const unsigned long num); // Constructor from unsigned long
	BigInteger(const long long num); // Constructor from long long
	BigInteger(const unsigned long long num); // Constructor from unsigned long long
	BigInteger(const int num); // Constructor from int
	BigInteger(const unsigned int num); // Constructor from unsigned int
	BigInteger(const short num); // Constructor from short
	BigInteger(const unsigned short num); // Constructor from unsigned short
	BigInteger(const float num); // Constructor from float
	BigInteger(const double num); // Constructor from double
	BigInteger(const long double num); // Constructor from long double
	BigInteger(const char* str); // Constructor from C-style string
	BigInteger(const std::string& str); // Constructor from string
	BigInteger(const BigInteger& other); // Copy constructor

	BigInteger& operator=(const BigInteger& other); // Copy assignment operator
	BigInteger& operator=(BigInteger&& other) noexcept; // Move assignment operator
	bool operator==(const BigInteger& other) const; // Equality operator
	bool operator!=(const BigInteger& other) const; // Inequality operator
	bool operator<(const BigInteger& other) const; // Less than operator
	bool operator<=(const BigInteger& other) const; // Less than or equal to operator
	bool operator>(const BigInteger& other) const; // Greater than operator
	bool operator>=(const BigInteger& other) const; // Greater than or equal to operator
	BigInteger operator+(const BigInteger& other) const; // Addition operator
	BigInteger operator+(const int& num) const;
	BigInteger operator+() const;
	BigInteger operator-(const BigInteger& other) const; // Subtraction operator
	BigInteger operator-(const int& num) const;
	BigInteger operator-() const;
	BigInteger operator*(const BigInteger& other) const; // Multiplication operator
	BigInteger operator/(const BigInteger& other) const; // Division operator
	BigInteger operator%(const BigInteger& other) const; // Modulus operator
	BigInteger& operator+=(const BigInteger& other); // Addition assignment operator
	BigInteger& operator-=(const BigInteger& other); // Subtraction assignment operator
	BigInteger& operator*=(const BigInteger& other); // Multiplication assignment operator
	BigInteger& operator/=(const BigInteger& other);
	BigInteger& operator%=(const BigInteger& other);
	BigInteger& operator++();	// Pre-increment operator
	BigInteger operator++(int); // Post-increment operator
	BigInteger& operator--();	// Pre-decrement operator
	BigInteger operator--(int); // Post-decrement operator
	friend std::ostream& operator<<(std::ostream& out, const BigInteger& num) {
		out << num.asString();
		return out;
	}
	friend std::istream& operator>>(std::istream& in, BigInteger& num) {
		std::string temp;
		in >> temp;
		num = BigInteger(temp);
		return in;
	}
	std::string asString() const; // Convert to string representation
	BigInteger __parseHexToDecimal(const std::string& hexStr) const; // Convert hexadecimal string to decimal BigInteger
	void parseHexToDecimal(); // 函数内部自解析函数
	BigInteger myPow(const int& index) const; // Power function

	void signSetter(Sign s) { sign = s; }
	void valueSetter(std::string s) { value = s; }
	Sign signGetter() { return sign; }
	std::string valueGetter() { return value; }
};