#include "ECC.h"
#include <iostream>
#include <vector>

using namespace std;

void eccTest() {
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

	cout << "p1 + p2 = (" << p3.x << ", " << p3.y << ")" << endl << "²¿¼þ²âÊÔ½áÊø" << endl;

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
}

int main() {
	eccTest();
	return 0;
}