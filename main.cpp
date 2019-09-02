#include <iostream>
#include <cmath>
#include <cstdio>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

typedef pair<double, double> point;
typedef pair<point, double> line;

double read_num(const string & name) {
	double num;
	/*while (!(cin >> num) || cin.peek() != '\n') {
		cin.clear();
		cout << "Error " << name << ", try again";
		while (cin.get() != '\n');
	}*/
	while (true) {
		cout << "Input " << name << " > ";
		if (cin >> num && cin.peek() == '\n') break;
		else {
			cin.clear();
			cout << "Error. Try again" << "\n";
			while (cin.get() != '\n');
		}
	}
	return num;
}

point read_point(int index) {
	cout << "Input coordinates of vertex #" << index << ": \n";
	double x = read_num("x");
	double y = read_num("y");
	return make_pair(x, y);
}

double vec_length(point & p) {
	return sqrt(p.first * p.first + p.second * p.second);
}

double det(double a, double b, double c, double d) {
	return a * d - b * c;
}

double vec_mult(point & a, point & b) {
	return det(a.first, a.second, b.first, b.second);
}

point mult(point & p, double k) {
	return make_pair(p.first * k, p.second * k);
}

point move(const point & p, const point & v) {
	return make_pair(p.first + v.first, p.second + v.second);
}

point cross(line & a, line & b) {
	// Ax + By = -C
	// first.first - A, first.second - B, second - C
	double d = det(a.first.first, a.first.second, b.first.first, b.first.second);
	double dx = det(-a.second, a.first.second, -b.second, b.first.second);
	double dy = det(a.first.first, -a.second, b.first.first, -b.second);
	return make_pair(dx / d, dy / d);
}

int main() {
	bool cont = false;
	bool failcont = false;
	cout << "Incircle of triangle" << endl;
	do {

		int n = 3;

		vector<point> points(n);
		for (int i = 0; i < n; ++i) {
			points[i] = read_point(i + 1);
		}

		vector<point> vectors(n);
		for (int i = 0; i < n; ++i) {
			int j = (i + 1) % n;
			vectors[i] = move(points[j], mult(points[i], -1));
		}

		vector<double> sizes(n);
		for (int i = 0; i < n; ++i) {
			sizes[i] = vec_length(vectors[i]);
		}

		vector<point> bisectors(n);
		for (int i = 0; i < n; ++i) {
			// i -> (i + 1)
			// bisector from points[i] to bisector[i]
			// p[i + 1] + v[i + 1] / s[i + 1] * x
			// x / s[i] == (s[i + 1] - x) / s[i - 1]
			// x * s[i - 1] == s[i + 1] * s[i - 1] - x * s[i]
			// x * (s[i] + s[i - 1]) == s[i + 1] * s[i]
			// x = s[i + 1] * s[i] / (s[i - 1] + s[i])
			// b[i] = p[i + 1] + v[i + 1] / s[i + 1] * x = p[i + 1] + v[i + 1] * s[i] / (s[i - 1] + s[i])

			int j = (i + 1) % n, k = (i + n - 1) % n;
			double x = sizes[i] / (sizes[k] + sizes[i]);
			bisectors[i] = move(points[j], mult(vectors[j], x));
		}

		// Ax + By + C = 0
		// (A, B) - vector-normal to line (perpendicular to line)
		vector<line> lines(n);
		for (int i = 0; i < n; ++i) {
			// (x - x1) / (x2 - x1) == (y - y1) / (y2 - y1)
			double a = bisectors[i].second - points[i].second;
			double b = points[i].first - bisectors[i].first;
			double c = -a * points[i].first - b * points[i].second;
			lines[i] = make_pair(make_pair(a, b), c);
		}

		point center = cross(lines[0], lines[1]);
		cout << "Centre: (" << center.first << " " << center.second << ")" << endl;

		double half_perimeter = 0;
		for (double size : sizes) half_perimeter += size;
		half_perimeter /= 2;

		double square = 0.5 * abs(vec_mult(vectors[0], vectors[1]));

		double radius = square / half_perimeter;
		cout << "Radius: " << radius << endl;

		string answer;
		do {
			cont = false;
			failcont = false;
			cout << "Continue? (Y/N)" << endl;

			cin.clear();

			do {
				getline(cin, answer);
			} while (answer == "");

			if (answer == "Y" || answer == "y") {
				cont = true;
			}
			else if (answer == "N" || answer == "n") {
				cont = false;
			}
			else {
				failcont = true;
			}
		} while (failcont == true);
	} while (cont == true);

	system("pause");

	return 0;
}