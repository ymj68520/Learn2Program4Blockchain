#include "BigInteger.h"
#include "ECC.h"
#include <iostream>
#include <vector>

using namespace std;
int eccTest();
int bigIntegerTest();

int Test_ECC() {
	return eccTest();
}

int Test_BigInteger() {
	return bigIntegerTest();
}

int main() {
	//cout << Test_ECC();
	cout << Test_BigInteger();
	return 0;
}

int eccTest() {
	int prime = 223;
	// Example usage of the FiniteFieldElement class
	FiniteFieldElement x(47, prime);
	FiniteFieldElement y(71, prime);
	FiniteFieldElement c = x + y;
	cout << "x + y = " << c.getValue() << endl;
	// Example usage of the Point class
	Point<int> p1 = Point<int>(192, 105, 0, 7, prime);
	Point<int> p2 = Point<int>(17, 56, 0, 7, prime);
	Point<int> p4 = Point<int>(47, 71, 0, 7, prime);
	Point<int> p3 = p1 + p2;
	Point<FiniteFieldElement> p5 = Point<FiniteFieldElement>(x, y, 0, 7, prime);
	cout << "N * p4:                         N * p5: " << endl;
	for (int i = 1; i < 21; i++) {
		cout << i << " * p4 = " << "(" << (p4 * i).x << ", " << (p4 * i).y << ")" << " \tendl \t";
		cout << i << " * p5 = " << "(" << (p5 * i).x.getValue() << ", " << (p5 * i).y.getValue() << ")" << endl;
	}

	cout << "p1 + p2 = (" << p3.x << ", " << p3.y << ")" << endl << "�������Խ���" << endl;

	// Example usage of the ECC class
	vector<vector<int>> valid_point = {
		{192, 105},
		{17, 56},
		{1, 193},
		{0,0}
	};
	/*vector<vector<int>> invalid_point = {
		{200, 119},
		{42, 99}
	};*/
	vector<Point<FiniteFieldElement>> points;
	for (const auto& point : valid_point) {
		if (point[0] == 0 && point[1] == 0) {
			points.push_back(Point<FiniteFieldElement>(FiniteFieldElement(0, prime), FiniteFieldElement(0, prime), 0, 7, prime));
			continue;
		}
		auto x = FiniteFieldElement(point[0], prime);
		auto y = FiniteFieldElement(point[1], prime);
		points.push_back(Point<FiniteFieldElement>(x, y, 0, 7, prime));
	}
	/*for (const auto& point : invalid_point) {
		auto x = FiniteFieldElement(point[0], prime);
		auto y = FiniteFieldElement(point[1], prime);
		points.push_back(Point<FiniteFieldElement>(x, y, prime));
	}*/

	for (const auto& point : points) {
		if (point.isPointAtInfinity()) {
			cout << "Point at infinity" << endl;
		}
		else {
			cout << "Point: (" << point.x.getValue() << ", " << point.y.getValue() << ")" << endl;
		}
	}
	ECC ecc = ECC(FiniteFieldElement(15, prime), FiniteFieldElement(86, prime), 0, 7, prime);
	cout << "ECC Order: " << ecc.order << endl;

	//SECP256K1 secp256k1 = SECP256K1();

	return 0;
}

int bigIntegerTest() {
	cout << "BigInteger ���Կ�ʼ" << endl;

	// ���캯������
	BigInteger a("123456789012345678901234567890");
	BigInteger b("987654321098765432109876543210");
	BigInteger c(123456789);
	BigInteger d = a; // ��������
	BigInteger e(123);
	BigInteger f(-456);
	cout << "a = " << a << endl;
	cout << "b = " << b << endl;
	cout << "c = " << c << endl;
	cout << "d = " << d << endl;
	cout << "e = " << e << endl;
	cout << "f = " << f << endl;

	// ��ֵ���������
	d = b;
	cout << "d = b �� d = " << d << endl;

	// �ӷ�����
	BigInteger sum = a + b;
	cout << "a + b = " << sum << endl;
	sum = e + f; // ���Ը����ӷ�
	cout << "e + f = " << sum << endl;

	// ��������
	BigInteger diff = b - a;
	cout << "b - a = " << diff << endl;
	diff = f - e; // ���Ը�������
	cout << "f - e = " << diff << endl;

	// �˷�����
	BigInteger prod = a * c;
	cout << "a * c = " << prod << endl;
	prod = e * f; // ���Ը����˷�
	cout << "e * f = " << prod << endl;

	//// �ڲ����������
	//BigInteger power = a._pow(10, 21); // ����ƽ��
	//cout << "10 ^ 21 = " << power << endl;

	// ��������
	BigInteger quot = b / c;
	cout << "b / c = " << quot << endl;
	prod = f / e; // ���Ը�������
	cout << "f / e = " << prod << endl;

	// ȡģ����
	BigInteger mod = b % c;
	cout << "b % c = " << mod << endl;
	mod = f % e; // ���Ը���ȡģ
	cout << "f % e = " << mod << endl;

	// �Ƚ����������
	cout << "a == b? " << (a == b) << endl;
	cout << "a != b? " << (a != b) << endl;
	cout << "a < b? " << (a < b) << endl;
	cout << "a > b? " << (a > b) << endl;
	cout << "a <= b? " << (a <= b) << endl;
	cout << "a >= b? " << (a >= b) << endl;

	// ��������
	BigInteger neg("-12345678901234567890");
	cout << "neg = " << neg << endl;
	cout << "a + neg = " << (a + neg) << endl;
	cout << "neg - a = " << (neg - a) << endl;

	// ���һ�Ĳ���
	BigInteger zero(0);
	BigInteger one(1);
	cout << "zero = " << zero << endl;
	cout << "one = " << one << endl;
	cout << "a + zero = " << (a + zero) << endl;
	cout << "a * one = " << (a * one) << endl;

	// �����Լ�����
	BigInteger inc = a;
	++inc;
	cout << "++a = " << inc << endl;
	--inc;
	cout << "--a = " << inc << endl;
	inc++;
	inc--;
	cout << "a-- = " << inc << endl;

	// �������������
	/*cout << "������һ��������: ";
	BigInteger input;
	cin >> input;
	cout << "���������: " << input << endl;*/

	cout << "BigInteger ���Խ���" << endl;
	return 0;
}