#include <iostream>

using std::cout;
using std::endl;

double const pi = 3.14;

class Point
{
	double x;
	double y;
public:
	Point(double x = 0, double y = 0) : x(x), y(y) {}
	double getX() const { return x; }
	double getY() const { return y; }
	void setX(const double& x) { Point::x = x; }
	void setY(const double& y) { Point::y = y; }
	void draw() const { cout << "Отрисовка точки " << endl; }
	bool operator==(const Point& two) const {
		return this->x == two.x && this->y == two.y;
	}
};

class SegmentLine
{
	Point A;
	Point B;
public:
	SegmentLine(Point a = (0, 0), Point b = (-1, -1)) : A(a), B(b) {
		if (A == B) throw "отрезок стягивается в точку!";
	}
	double getPlaneX() const { return abs(B.getX() - A.getX()); } // проекция на ось X
	double getPlaneY() const { return abs(B.getY() - A.getY()); } // проекция на ось Y
	double getLength() const { return sqrt(pow(getPlaneX(), 2) + pow(getPlaneY(), 2)); } // длина отрезка
	void draw() const { cout << "Отрисовка отрезка " << endl; }
};

class Figure
{
protected:
	Point a;
	Point b;
	Point c;
	SegmentLine diag;
	double minorAxis;
	double majorAxis;
public: 
	Figure(Point& A, Point& B, Point C = (0, 0)) : a(A), b(B), c(C), diag(A, B) {}
	Figure(SegmentLine& Diag) : diag(Diag) {}
	Figure(Point& center, double& min, double& maj) : a(center), minorAxis(min), majorAxis(maj) {}
	virtual double border() const = 0; // периметр
	virtual double square() const = 0; // площадь
	virtual void draw() const {} // отрисовка фигуры
	virtual ~Figure() = 0 {}
};

// Вариант реализации прямоугольника: 
// прямоугольник однозначно определяется диагональю
class Rectangle : public Figure
{
public:
	Rectangle(SegmentLine& ab) : Figure(ab) { // если имеется диагональ
		if (diag.getPlaneX() == 0 || diag.getPlaneY() == 0)
			throw "прямоугольник стягивается в отрезок, построение невозможно!";
	} 
	Rectangle(Point& a, Point& b) : Figure(a, b) { // если нет, то задаем концы диагонали
		if (diag.getPlaneX() == 0 || diag.getPlaneY() == 0)
			throw "прямоугольник стягивается в отрезок, построение невозможно!";
	}
	double border() const override { return 2 * (diag.getPlaneX() + diag.getPlaneY()); }
	double square() const override { return diag.getPlaneX() * diag.getPlaneY(); }
	double getLength() const { return diag.getPlaneX(); }
	double getWidth() const { return diag.getPlaneY(); }
	void draw() const override { cout << "Отрисовка прямоугольника" << endl; }
	~Rectangle() {}
};

class Ellipse : public Figure
{
public:
	Ellipse(Point& focus, double minor, double major) : Figure(focus, minor, major) {
		if (minor <= 0 || major <= 0) throw "полуоси эллипса неотрицательны!";
	}
	double border() const override { 
		return 2 * pi * sqrt((minorAxis * minorAxis + majorAxis * majorAxis) / 2); 
	}
	double square() const override { return pi * minorAxis * majorAxis; }
	void draw() const override { cout << "Отрисовка эллипса" << endl; }
	double focalDistance() const { return sqrt(pow(minorAxis, 2) + pow(majorAxis, 2)); }
	Point focus1() const {
		Point focus(a.getX() - this->focalDistance(), a.getY());
		return focus;
	}
	~Ellipse() {}
};

class Triangle : public Figure
{
public:
	Triangle(Point& A, Point& B, Point& C) : Figure(A, B, C) {
		if (A == B || A == C || B == C)
			throw "треугольник стягивается в точку!";
		else if (A.getX() == B.getX() && B.getX() == C.getX())
			throw "треугольник стягивается в вертикальный отрезок!";
		else if (A.getY() == B.getY() && B.getY() == C.getY())
			throw "треугольник стягивается в горизонтальный отрезок!";
		else if (abs(B.getY() - A.getY()) / abs(B.getX() - A.getX()) ==
			abs(C.getY() - B.getY()) / abs(C.getX() - B.getX()) &&
			abs(C.getY() - B.getY()) / abs(C.getX() - B.getX()) ==
			abs(C.getY() - A.getY()) / abs(C.getX() - A.getX()))
			throw "треугольник стягивается в отрезок, составляющий ненулевой угол с горизонталью!";
	}
	double border() const override {
		SegmentLine sideA(b, c), sideB(a, c), sideC(a, b);
		return sideA.getLength() + sideB.getLength() + sideC.getLength();
	}
	double square() const override {
		SegmentLine sideA(b, c), sideB(a, c), sideC(a, b);
		double a1 = sideA.getLength(); 
		double b1 = sideB.getLength();
		double c1 = sideC.getLength();
		double p = border() / 2;
		return sqrt((p - a1) * (p - b1) * (p - c1));
	}
	void draw() const override { cout << "Отрисовка треугольника" << endl; }
	~Triangle() {}
};

int main() {
	setlocale(LC_ALL, "Russian");

	Point A(10, -4);
	Point B(6, -4.1);
	try
	{
		cout << "Попытка создания объекта 'Отрезок'" << endl;
		SegmentLine diag(A, B);
		cout << "Объект создан успешно!" << endl << endl;

		cout << "Попытка создания объекта 'Прямоугольник'" << endl;
		Rectangle* rect1 = new Rectangle(diag);
		cout << "Объект создан успешно!" << endl << endl;
		cout << "Длина: " << rect1->getLength() << endl;
		cout << "Ширина: " << rect1->getWidth() << endl;
		cout << "Периметр: " << rect1->border() << endl;
		cout << "Площадь: " << rect1->square() << endl;
		rect1->draw();
		cout << endl;

		Point C(0, -1); Point D(-8, 7);
		cout << "Попытка создания объекта 'Прямоугольник'" << endl;
		Rectangle rect2(C, D);
		cout << "Объект создан успешно!" << endl << endl;
		cout << "Длина: " << rect2.getLength() << endl;
		cout << "Ширина: " << rect2.getWidth() << endl;
		cout << "Периметр: " << rect2.border() << endl;
		cout << "Площадь: " << rect2.square() << endl;
		rect2.draw();
		cout << endl;

		cout << "Попытка создания объекта 'Эллипс'" << endl;
		Ellipse oval1(A, 1.5, 8);
		cout << "Объект создан успешно!" << endl << endl;
		cout << "Периметр: " << oval1.border() << endl;
		cout << "Площадь: " << oval1.square() << endl;
		cout << "Фокальное расстояние: " << oval1.focalDistance() << endl;
		Point focus(oval1.focus1());
		cout << "Фокус x: " << focus.getX() << "  Фокус y: " << focus.getY() << endl;
		oval1.draw();
		cout << endl;

		rect1->~Rectangle(); // удаление прямоугольника rect1
		
		Point E(-1, -1); Point F(2, 2); Point G(-141.01, -141.01);
		cout << "Попытка создания объекта 'Треугольник'" << endl;
		Triangle trig1(E, G, F);
		cout << "Объект создан успешно!" << endl << endl;
		cout << "Периметр: " << trig1.border() << endl;
		cout << "Площадь: " << trig1.square() << endl;
		trig1.draw();
		cout << endl;
	}
	catch (const char * ex) { cout << "Ошибка: " << ex << endl << endl; }
	system("pause");
	return 0;
}
