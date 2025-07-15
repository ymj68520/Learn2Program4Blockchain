#ifndef BIGINT_H
#define BIGINT_H
#define MAXN 100000
#include <string.h>
#include <iostream>
#include <string>
typedef class BigInt {
private:
	int num[MAXN];
	int len;
	bool sign;
public:
	static int max_Big(int x, int y) {
		return x > y ? x : y;
	}
	BigInt(int x) { //初始化使得大整型能够表示整型
		memset(num, 0, sizeof(num));
		for (len = 1; x; len++) {
			num[len] = x % 10;
			x /= 10;
		}
		sign = 1;
		len--;//由于上述循环中len多加了一次，因此这里减去
	}
	BigInt() {
		memset(num, 0, sizeof(num));
		len = 0;
		sign = 1;
	}
	BigInt(std::string s) {
		len = s.length();
		int cnt = len - 1;
		for (int i = 1; i <= len; i++) {
			num[i] = s[cnt--] - '0';
		}
		sign = 1;
	}
	BigInt(char* s) {
		len = strlen(s);
		int cnt = 1;
		for (int i = len - 1; i >= 0; i--) num[cnt++] = s[i] - '0';
	}
	void flatten(int L) { //进行展平操作
		len = L;
		for (int i = 1; i <= len; i++) {
			num[i + 1] += num[i] / 10;
			num[i] %= 10;
		}
		while (!num[len]) { //删除多余位数
			len--;
		}
	}
	friend std::ostream& operator<<(std::ostream& iout, BigInt& I1);
	friend std::istream& operator>>(std::istream& iint, BigInt& I1);
	BigInt& operator = (const BigInt& I) {  //重载赋值运算符
		len = I.len;
		sign = I.sign;
		memset(num, 0, sizeof(num));
		for (int i = 1; i <= len; i++)
			num[i] = I.num[i];
		return *this;
	}
	friend BigInt operator+(BigInt& I1, BigInt& I2);
	friend BigInt operator-(BigInt& I1, BigInt& I2);
	friend BigInt operator*(BigInt& I1, BigInt& I2);
	friend BigInt operator/(BigInt I1, BigInt& I2);//大整型除以大整型-减法模拟除法
	friend BigInt operator/(BigInt& I1, int I2);//大整型除以整型-逐位试商法
	friend bool operator<(const BigInt& I1, const BigInt& I2);
	friend bool operator>(const BigInt& I1, const BigInt& I2);
	friend bool operator!=(const BigInt& I1, const BigInt& I2);
	friend bool operator==(const BigInt& I1, const BigInt& I2);
	friend bool operator<=(const BigInt& I1, const BigInt& I2);
	friend bool operator>=(const BigInt& I1, const BigInt& I2);
} BigInt;
bool operator<(const BigInt& I1, const BigInt& I2) {
	int len1 = I1.len, len2 = I2.len;
	if (len1 != len2) {
		return len1 < len2;
	}
	for (int i = len1; i >= 1; i--) {
		if (I1.num[i] != I2.num[i]) return I1.num[i] < I2.num[i];
	}
	return false;
}
bool operator<=(const BigInt& I1, const BigInt& I2) {
	int len1 = I1.len, len2 = I2.len;
	if (len1 != len2) {
		return len1 < len2;
	}
	for (int i = len1; i >= 1; i--) {
		if (I1.num[i] != I2.num[i]) return I1.num[i] < I2.num[i];
	}
	return true;
}
bool operator>(const BigInt& I1, const BigInt& I2) {
	int len1 = I1.len, len2 = I2.len;
	if (len1 != len2) {
		return len1 > len2;
	}
	for (int i = len1; i >= 1; i--) {
		if (I1.num[i] != I2.num[i]) return I1.num[i] > I2.num[i];
	}
	return false;
}
bool operator>=(const BigInt& I1, const BigInt& I2) {
	int len1 = I1.len, len2 = I2.len;
	if (len1 != len2) {
		return len1 > len2;
	}
	for (int i = len1; i >= 1; i--) {
		if (I1.num[i] != I2.num[i]) return I1.num[i] > I2.num[i];
	}
	return true;
}
bool operator==(const BigInt& I1, const BigInt& I2) {
	int len1 = I1.len, len2 = I2.len;
	if (len1 != len2) {
		return false;
	}
	for (int i = len1; i >= 1; i--) {
		if (I1.num[i] != I2.num[i]) return false;
	}
	return true;
}
bool operator!=(const BigInt& I1, const BigInt& I2) {
	int len1 = I1.len, len2 = I2.len;
	if (len1 != len2) {
		return true;
	}
	for (int i = len1; i >= 1; i--) {
		if (I1.num[i] != I2.num[i]) return true;
	}
	return false;
}
BigInt operator+(BigInt& I1, BigInt& I2) {
	int L = BigInt::max_Big(I1.len, I2.len) + 1;
	BigInt I3;
	for (int i = 1; i <= L; i++) {
		I3.num[i] = I1.num[i] + I2.num[i];
	}
	I3.flatten(L);
	return I3;
}
BigInt operator-(BigInt& I1, BigInt& I2) {
	int L = BigInt::max_Big(I1.len, I2.len);
	BigInt i1, i2, I3;
	if (I1 < I2) {
		i1 = I2;
		i2 = I1;
		I3.sign = 0;
	}
	else {
		i1 = I1;
		i2 = I2;
		I3.sign = 1;
	}
	for (int i = 1; i <= L; i++) {
		if (i1.num[i] < i2.num[i]) {
			i1.num[i + 1]--;
			i1.num[i] += 10;
		}
		I3.num[i] = i1.num[i] - i2.num[i];
	}
	while (I3.num[L] == 0 && L > 1) L--;
	I3.len = L;
	return I3;
}
BigInt operator*(BigInt& I1, BigInt& I2) {
	BigInt I3;
	int len1 = I1.len, len2 = I2.len;
	int L = len1 + len2;
	for (int i = 1; i <= len1; i++)
		for (int j = 1; j <= len2; j++) {
			I3.num[i + j - 1] += I1.num[i] * I2.num[j];
		}
	I3.flatten(L);
	return I3;
}
BigInt operator/(BigInt& I1, int I2) {
	BigInt I3;
	int x = 0;
	int len = I1.len;
	for (int i = len; i >= 1; i--) {
		I3.num[i] = (x * 10 + I1.num[i]) / I2;
		x = ((x * 10 + I1.num[i])) % I2;
	}
	I3.flatten(len);
	return I3;
}
BigInt operator/(BigInt I1, BigInt& I2) {
	BigInt I3;
	int len1 = I1.len, len2 = I2.len;
	int start = len1 - len2 + 1;
	for (int i = start; i >= 1; i--) {
		BigInt tmp1, tmp2(10);
		tmp1 = I2;
		while (tmp1.len < i) {
			tmp1 = tmp1 * tmp2;
		}
		while (I1 >= tmp1) {
			I3.num[i]++;
			I1 = I1 - tmp1;
		}
	}
	I3.flatten(start);
	return I3;
}
std::istream& operator>>(std::istream& iint, BigInt& I1) { //重载输入
	std::string s;
	iint >> s;
	I1.len = s.length();
	int cnt = I1.len - 1;
	for (int i = 1; i <= I1.len; i++) {
		I1.num[i] = s[cnt--] - '0';
	}
	return iint;
}
std::ostream& operator<<(std::ostream& iout, BigInt& I1) { //重载输出
	if (!I1.sign) std::cout << '-';
	for (int i = BigInt::max_Big(I1.len, 1); i >= 1; i--) {
		iout << I1.num[i];
	}
	return iout;
}

#endif