#include "ECC.h"
#include <iostream>
#include <vector>

using namespace std;

void eccTest() {
	// Example usage of the FiniteFieldElement class
	FiniteFieldElement a(3, 7);
	FiniteFieldElement b(5, 7);
	FiniteFieldElement c = a + b;
	cout << "a + b = " << c.getValue() << endl;
	// Example usage of the Point class
	Point<int> p1 = Point<int>(3, 6, 2, 3, 7);
	Point<int> p2 = Point<int>(2, 1, 2, 3, 7);
	Point<int> p4 = Point<int>(47, 71, 0, 7, 223);
	Point<int> p3 = p1 + p2;
	for (int i = 1; i < 21; i++) {
		cout << i << " * p4 = " << "(" << (p4 * i).x << ", " << (p4 * i).y << ")" << endl;
	}
	cout << "p1 + p2 = (" << p3.x << ", " << p3.y << ")" << endl << "²¿¼þ²âÊÔ½áÊø" << endl;

	// Example usage of the ECC class
	int prime = 223;
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
			points.push_back(Point<FiniteFieldElement>(FiniteFieldElement(prime), FiniteFieldElement(prime), 0, 7, prime));
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
}

int main() {
	eccTest();
	return 0;
}