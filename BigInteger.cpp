#include "BigInteger.h"

BigInteger::BigInteger() : value("0"), sign(Sign::ZERO) {}
BigInteger::BigInteger(const long num) : BigInteger(static_cast<long long>(num)) {}
BigInteger::BigInteger(const unsigned long num) : BigInteger(static_cast<unsigned long long>(num)) {}
BigInteger::BigInteger(const int num) : BigInteger(static_cast<long long>(num)) {}
BigInteger::BigInteger(const unsigned int num) : BigInteger(static_cast<unsigned long long>(num)) {}
BigInteger::BigInteger(const short num) : BigInteger(static_cast<long long>(num)) {}
BigInteger::BigInteger(const unsigned short num) : BigInteger(static_cast<unsigned long long>(num)) {}
BigInteger::BigInteger(const float num) : BigInteger(static_cast<long double>(num)) {}
BigInteger::BigInteger(const double num) : BigInteger(static_cast<long double>(num)) {}
BigInteger::BigInteger(const char* str) : BigInteger(std::string(str)) {}
BigInteger::BigInteger(const long long num) : value(std::to_string(num)), sign(Sign::POSITIVE) {
	if (num < 0) {
		sign = Sign::NEGATIVE;
		value = value.substr(1); // Remove the negative sign
	}
	else if (num == 0) {
		sign = Sign::ZERO;
		value = "0";
	}
}
BigInteger::BigInteger(unsigned long long num) : value(std::to_string(num)), sign(Sign::POSITIVE) {
	if (num < 0) {
		sign = Sign::NEGATIVE;
		value = value.substr(1); // Remove the negative sign
	}
	else if (num == 0) {
		sign = Sign::ZERO;
		value = "0";
	}
} // Constructor from unsigned long long

BigInteger::BigInteger(const long double num) : value(std::to_string(num)), sign(Sign::POSITIVE) {
	if (num < 0) {
		sign = Sign::NEGATIVE;
		value = value.substr(1); // Remove the negative sign
	}
	else if (num == 0) {
		sign = Sign::ZERO;
		value = "0";
	}
}

BigInteger::BigInteger(const std::string& str) : value(str) {
	if (str.empty() || str == "0") {
		sign = Sign::ZERO;
		value = "0";
	}
	else if (str[0] == '-') {
		sign = Sign::NEGATIVE;
		value = str.substr(1);
	}
	else {
		sign = Sign::POSITIVE;
		value = str;
	}
}

BigInteger::BigInteger(const BigInteger& other) : value(other.value), sign(other.sign) {}
BigInteger& BigInteger::operator=(const BigInteger& other) {
	if (this != &other) {
		value = other.value;
		sign = other.sign;
	}
	return *this;
}
BigInteger& BigInteger::operator=(BigInteger&& other) noexcept {
	if (this != &other) {
		value = std::move(other.value);
		sign = other.sign;
		other.sign = Sign::ZERO; // Reset the moved-from object
	}
	return *this;
}

bool BigInteger::operator==(const BigInteger& other) const {
	if (sign != other.sign) return false;
	return value == other.value;
}
bool BigInteger::operator!=(const BigInteger& other) const {
	return !(*this == other);
}
bool BigInteger::operator<(const BigInteger& other) const {
	if (sign == Sign::NEGATIVE && other.sign == Sign::POSITIVE) return true;
	if (sign == Sign::POSITIVE && other.sign == Sign::NEGATIVE) return false;

	if (value.length() != other.value.length()) {
		return (sign == Sign::POSITIVE) ? value.length() < other.value.length() : value.length() > other.value.length();
	}
	return (sign == Sign::POSITIVE) ? value < other.value : value > other.value;
}
bool BigInteger::operator<=(const BigInteger& other) const {
	return *this < other || *this == other;
}
bool BigInteger::operator>(const BigInteger& other) const {
	return !(*this <= other);
}
bool BigInteger::operator>=(const BigInteger& other) const {
	return !(*this < other);
}
BigInteger& BigInteger::operator+=(const BigInteger& other) {
	if (sign == Sign::ZERO) {
		*this = other;
		return *this;
	}
	if (sign == Sign::NEGATIVE && other.sign == Sign::POSITIVE) {
		signChange();
		*this -= other;
		signChange();
		return *this;
	}
	if (sign == Sign::POSITIVE && other.sign == Sign::NEGATIVE) {
		BigInteger temp(other);
		temp.signChange();
		*this -= temp;
		return *this;
	}

	int len1 = value.length() - 1;
	int len2 = other.value.length() - 1;
	int carry = 0;

	std::string result;
	result.reserve((len1 > len2 ? len1 : len2) + 5); // Reserve space for the result

	while (len1 >= 0 || len2 >= 0 || carry > 0) {
		int num1 = len1 >= 0 ? value[len1--] - '0' : 0;
		int num2 = len2 >= 0 ? other.value[len2--] - '0' : 0;
		int res = num1 + num2 + carry;

		carry = res / 10;
		res = res % 10;

		result += char(res + '0');
	}
	std::reverse(result.begin(), result.end());
	value = result;
	return *this;
}
BigInteger& BigInteger::operator++() { // Pre-increment operator
	return *this += BigInteger(1);
}
BigInteger BigInteger::operator++(int) { // Post-increment operator
	BigInteger temp(*this);
	++(*this);
	return temp;
}
BigInteger BigInteger::operator+(const BigInteger& other) const {
	BigInteger result(*this);
	result += other;
	return result;
}
BigInteger BigInteger::operator+(const int& num) const {
	return *this + BigInteger(num);
}
BigInteger BigInteger::operator+()const {
	return *this;
}
BigInteger& BigInteger::operator-=(const BigInteger& other) {
	if (sign == Sign::ZERO) {
		*this = other;
		signChange();
		return *this;
	}
	if (sign == Sign::NEGATIVE && other.sign == Sign::POSITIVE) {
		signChange();
		*this += other;
		signChange();
		return *this;
	}
	if (sign == Sign::POSITIVE && other.sign == Sign::NEGATIVE) {
		BigInteger temp(other);
		temp.signChange();
		*this += temp;
		return *this;
	}
	int changeSign = 1;
	int compare = absCompare(*this, other);
	if (compare < 0) {
		changeSign = -1;
	}
	else if (compare == 0) {
		value = "0";
		sign = Sign::ZERO;
		return *this;
	}

	int len1 = value.length() - 1;
	int len2 = other.value.length() - 1;
	int borrow = 0;

	std::string result;
	result.reserve((len1 > len2 ? len1 : len2) + 5); // Reserve space for the result

	while (len1 >= 0 || len2 >= 0) {
		int num1 = len1 >= 0 ? value[len1--] - '0' : 0;
		int num2 = len2 >= 0 ? other.value[len2--] - '0' : 0;
		int res = changeSign * (num1 - num2) + borrow;
		if (res < 0) {
			borrow = -1;
			res += 10;
		}
		else {
			borrow = 0;
		}
		result += char(res + '0');
	}

	// remove zeros from high bits
	while (result.size() > 1 && result[result.size() - 1] == '0')
		result.pop_back();

	if (changeSign == -1)
		signChange();

	std::reverse(result.begin(), result.end());
	value = result;
	return *this;
}
BigInteger& BigInteger::operator--() {
	return (*this) -= BigInteger(1);
}
BigInteger BigInteger::operator--(int) {
	BigInteger temp(*this);
	--(*this);
	return temp;
}
BigInteger BigInteger::operator-(const BigInteger& other) const {
	return BigInteger(*this) -= other;
}
BigInteger BigInteger::operator-(const int& num) const {
	return BigInteger(*this) -= BigInteger(num);
}
BigInteger BigInteger::operator-() const {
	BigInteger temp(*this);
	temp.signChange();
	return temp;
}
// cal first
BigInteger& BigInteger::operator*=(const BigInteger& other) {
	if (*this == 0 || other == 0) return *this = 0;

	if (sign == Sign::NEGATIVE && other.sign == Sign::NEGATIVE)
		sign = Sign::POSITIVE;
	if (sign == Sign::POSITIVE && other.sign == Sign::NEGATIVE)
		sign = Sign::NEGATIVE;
	/*
	std::vector<int> result(value.size() + other.value.size(), 0);
	for (int i = value.size() - 1; i >= 0; --i) {
		int num1 = value[i] - '0';
		for (int j = other.value.size() - 1; j >= 0; --j) {
			int num2 = other.value[j] - '0';
			result[i + j + 1] += num1 * num2;
		}
	}
	// 处理进位
	for (int i = result.size() - 1; i > 0; --i) {
		result[i - 1] += result[i] / 10;
		result[i] %= 10;
	}
	*/

	int len1 = value.size() - 1;
	int len2 = other.value.size() - 1;
	int maxN = len1 + len2 + 2;
	int index = maxN - 1;
	std::vector<int> result(maxN + 5, 0);

	while (len1 >= 0) {
		int i = maxN - (index--) - 1;
		int num1 = len1 >= 0 ? value[len1--] - '0' : 0;
		for (int line = len2; line >= 0; --line) {
			int num2 = other.value[line] - '0';
			result[i++] += num1 * num2;
		}
	}
	// carrys
	for (int i = 0; i < result.size(); ++i) {
		if (result[i] > 9) {
			result[i + 1] += result[i] / 10;
			result[i] %= 10;
		}
		result[i] += '0';
	}

	// remove zeros
	for (int i = result.size() - 1; i > 0; --i) {
		if (result[i] == '0') {
			result.pop_back();
		}
		else {
			break;
		}
	}
	/*if (result[result.size() - 1] == 0) {
		result.pop_back();
	}*/
	std::reverse(result.begin(), result.end());
	value = std::string(result.begin(), result.end());
	return *this;	// 结果的符号与this保持一致
}
//// cal while carrys
//BigInteger& BigInteger::operator*=(const BigInteger& other) {
//
//}
BigInteger BigInteger::operator*(const BigInteger& other) const {
	return BigInteger(*this) *= other;
}
BigInteger& BigInteger::operator/=(const BigInteger& other) {
	if (other == 0) {
		throw std::invalid_argument("The dividend cannot be zero!");
	}
	if (absCompare(*this, other) < 0) return *this = 0;

	Sign sign1 = sign;
	Sign sign2 = other.sign;

	int len1 = value.size() - 1;
	int len2 = other.value.size() - 1;
	int condition = len1 - len2;
	int count = 0;

	std::string result;
	result.reserve(len1);

	while (len1 - len2 - count >= 0) {
		int left = -1;
		int right = 10;
		while (left + 1 < right) { // 使用二分法试乘
			int mid = left + (right - left) / 2;
			BigInteger midValue = BigInteger(mid) * other * _pow(10, condition);
			//if (absCompare(*this, other * mid * _pow(10, condition)) >= 0)
			if (absCompare(*this, midValue) >= 0) 
				left = mid;

			else
				right = mid;
		}
		if (left != 0) {
			if (sign == other.sign)
				*this -= (BigInteger(left) * other) * _pow(10, condition);
			else
				*this += (BigInteger(left) * other) * _pow(10, condition);
		}
		result += (left + '0');
		++count;
		--condition;
	}
	if (sign1 == Sign::NEGATIVE && sign2 == Sign::NEGATIVE)
		sign = Sign::POSITIVE;
	else if (sign1 == Sign::POSITIVE && sign2 == Sign::NEGATIVE)
		sign = Sign::NEGATIVE;
	else
		sign = sign1;
	if (result[0] == '0') result.erase(result.begin());
	value = std::move(result);
	return *this;
}
BigInteger BigInteger::operator/(const BigInteger& other) const {
	return BigInteger(*this) /= other;
}
BigInteger& BigInteger::operator%=(const BigInteger& other) {
	BigInteger temp = *this;
	temp /= other;
	return *this -= temp * other;
}
BigInteger BigInteger::operator%(const BigInteger& other) const {
	return BigInteger(*this) %= other;
}
BigInteger::Sign BigInteger::signExpressSolve(const std::string& str) {
	/*if (str.empty() || str == "0") return Sign::ZERO;
	if (str[0] == '-') return Sign::NEGATIVE;
	return Sign::POSITIVE;*/
	signed char sign = 1;
	for (auto e : str)
	{
		if (e == static_cast<char>(Sign::NEGATIVE))
		{
			sign = -sign;
		}
		else if (e == static_cast<char>(Sign::POSITIVE))
		{
			;
		}
		else
		{
			break;
		}
	}
	return sign == 1 ? Sign::POSITIVE : Sign::NEGATIVE;
}
std::string BigInteger::asString() const {
	if (sign == Sign::ZERO)	return "0";
	else if (sign == Sign::NEGATIVE) return "-" + value;
	else if (sign == Sign::POSITIVE) return value;
	//return std::to_string(char(sign)) + value;
}
size_t BigInteger::signLastIndex(const std::string& str) {
	size_t index = -1;
	for (auto e : str)
	{
		if (e == static_cast<char>(Sign::NEGATIVE) || e == static_cast<char>(Sign::POSITIVE))
		{
			++index;
		}
		else
		{
			break;
		}
	}
	return index;
}
BigInteger BigInteger::_pow(const BigInteger& num, const int& index) {
	if (index == 0) return 1;
	BigInteger temp = _pow(num, index / 2);
	if (index % 2)
		return temp * temp * num;
	else
		return temp * temp;
}

//std::ostream& operator<<(std::ostream& out, const BigInteger& num) {
//	out << num.asString();
//	return out;
//}
//
//std::istream& operator>>(std::istream& in, BigInteger& num) {
//	std::string temp;
//	in >> temp;
//	num = BigInteger(temp);
//	return in;
//}