#pragma once

#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

const double EPS = 1e-6;
const double PI = 3.14159265;

struct Point {
  double x;
  double y;
  Point() : x(0.), y(0.) {}
  Point(double a, double b) : x(a), y(b) {}
  ~Point() = default;

  bool operator==(const Point &a) const {
    if (fabs(x - a.x) > EPS || fabs(y - a.y) > EPS) {
      return false;
    }
    return true;
  }

  bool operator!=(const Point &a) const {
    if (fabs(x - a.x) < EPS || fabs(y - a.y) < EPS) {
      return false;
    }
    return true;
  }

  double distance_between_points(const Point &a) const {
    return sqrt(pow(x - a.x, 2) + pow(y - a.y, 2));
  }
};


class Line {
private:
  Point a, b;

public:
  double angle;
  double shift;
  Line(const Point &a, const Point &b) : a(a), b(b) {
    angle = (a.y - b.y) / (a.x - b.x);
    shift = a.y - angle * a.x;
  }
  Line(double angle, double shift) : angle(angle), shift(shift) {}

  Line(Point &p, double angle) : a(p), angle(angle) {
    shift = p.y - angle * p.x;
  }
  bool operator==(const Line &l) const {
    return abs(shift - l.shift) < EPS && abs(angle - l.angle) < EPS;
  }
  bool operator!=(const Line &l) const {
    return !(l == *this);
  }
};

class Shape {
public:
  virtual double perimeter() const = 0;
  virtual double area() const = 0;
  virtual bool operator==(const Shape &another) const = 0;
  virtual void rotate(Point center, double angle) = 0;
  virtual void reflex(Point center) = 0;
  virtual void reflex(Line axis) = 0;
  virtual void scale(Point center, double coefficient) = 0;
  virtual ~Shape() = 0;
};

Shape::~Shape(){};

class Polygon : public Shape {
protected:
  std::vector<Point> vertices;

public:
  Polygon() = default;
  explicit Polygon(std::vector<Point> v) {
    vertices.assign(v.begin(), v.end());
  }
  Polygon(const Polygon &copy) {
    std::vector<Point> v = copy.vertices;
    vertices.assign(v.begin(), v.end());
  }

  int verticesCount() const {
    return vertices.size();
  }
  std::vector<Point> getVertices() const {
    return vertices;
  }
  bool operator==(const Shape &another) const override {
    const Polygon *polygon = dynamic_cast<const Polygon *>(&another);
    if (!polygon) {
      return false;
    }
    if (polygon->verticesCount() != verticesCount()) {
      return false;
    }
    for (size_t i = 0; i < polygon->verticesCount(); ++i) {
      size_t j;
      for (j = 0; j < verticesCount(); ++j)
        if (polygon->vertices[i] == vertices[j]) {
          break;
        }
      if (j == verticesCount()) {
        return false;
      }
      if (abs(area() - polygon->area()) > EPS) {
        return false;
      }
    }
    return true;
  }

  bool operator!=(const Polygon &polygon) const {
    return !(polygon == *this);
  }

  double perimeter() const override {
    double res = 0;
    int size = verticesCount();
    res += vertices[size - 1].distance_between_points(vertices[0]);
    for (size_t i = 0; i < size - 1; ++i) {
      res += vertices[i + 1].distance_between_points(vertices[i]);
    }
    return res;
  }

  double area() const override {
    double res = 0;
    for (size_t i = 0; i < vertices.size(); ++i) {
      Point point_1 = i ? vertices[i - 1] : vertices.back();
      Point point_2 = vertices[i];
      res += (point_1.x - point_2.x) * (point_1.y + point_2.y);
    }
    return fabs(res) / 2;
  }

  void rotate(Point center, double angle) override {
    angle = angle * PI / 180;
    for (size_t i = 0; i < vertices.size(); ++i) {
      double tmp_x = vertices[i].x;
      double tmp_y = vertices[i].y;
      vertices[i].x = center.x + cos(angle) * (tmp_x - center.x) - sin(angle) * (tmp_y - center.y);
      vertices[i].y = center.y + sin(angle) * (tmp_x - center.x) + cos(angle) * (tmp_y - center.y);
    }
  }

  void reflex(Point center) override {
    for (size_t i = 0; i < vertices.size(); ++i) {
      vertices[i].x = 2 * center.x - vertices[i].x;
      vertices[i].y = 2 * center.y - vertices[i].y;
    }
  }

  void reflex(Line axis) override {
    double k = axis.angle;
    double b = axis.shift;
    for (size_t i = 0; i < vertices.size(); ++i) {
      double x_0 = (k * vertices[i].y + vertices[i].x - k * b) / (k * k + 1);
      double y_0 = k * x_0 + b;
      vertices[i].x = 2 * x_0 - vertices[i].x;
      vertices[i].y = 2 * y_0 - vertices[i].y;
    }
  }

  void scale(Point center, double coefficient) override {
    for (size_t i = 0; i < vertices.size(); ++i) {
      vertices[i].x = center.x + coefficient * (vertices[i].x - center.x);
      vertices[i].y = center.y + coefficient * (vertices[i].y - center.y);
    }
  }
};


class Ellipse : public Shape {
protected:
  Point focus_1, focus_2;
  double distance;

public:
  Ellipse() = default;

  Ellipse(Point f1, Point f2, double d) : focus_1(f1), focus_2(f2), distance(d) {}

  Point center() const {
    return Point((focus_1.x + focus_2.x) / 2, (focus_1.y + focus_2.y) / 2);
  }

  double focal_distance() const {
    return focus_1.distance_between_points(focus_2);
  }

  double eccentricity() const {
    double res = focal_distance() / distance;
    return res;
  }

  std::pair<Point, Point> focuses() {
    return {focus_1, focus_2};
  }

  double perimeter() const override {

    double a = distance / 2;
    double b = (sqrt(pow(a, 2) - pow(focal_distance() / 2, 2)));
    double res = PI * (a + b) * (1 + 3 * pow((a - b) / (a + b), 2) / (10 + sqrt(4 - 3 * pow((a - b) / (a + b), 2))));
    return res;
  }

  double area() const override {
    double big_semi_axe = distance / 2;
    double small_semi_axe = (sqrt(pow(big_semi_axe, 2) - pow(focal_distance() / 2, 2)));
    return PI * big_semi_axe * small_semi_axe;
  }

  bool operator==(const Shape &another) const override {
    const Ellipse *ellps = dynamic_cast<const Ellipse *>(&another);
    if (!ellps) {
      return false;
    }
    if (fabs(distance - ellps->distance) > EPS) {
      return false;
    }
    if (focus_1.distance_between_points(ellps->focus_1) > EPS || focus_2.distance_between_points(ellps->focus_2) > EPS) {
      return false;
    }
    return true;
  }

  void rotate(Point center, double angle) override {
    angle = angle * PI / 180;
    double tmp_x = focus_1.x;
    focus_1.x = center.x + cos(angle) * (tmp_x - center.x) - sin(angle) * (focus_1.y - center.y);
    focus_1.y = center.y + sin(angle) * (tmp_x - center.x) + cos(angle) * (focus_1.y - center.y);
    double tmp_2_x = focus_2.x;
    focus_2.x = center.x + cos(angle) * (tmp_2_x - center.x) - sin(angle) * (focus_2.y - center.y);
    focus_2.y = center.y + sin(angle) * (tmp_2_x - center.x) + cos(angle) * (focus_2.y - center.y);
  }

  void reflex(Point center) override {
    focus_1.x = 2 * center.x - focus_1.x;
    focus_1.y = 2 * center.y - focus_1.y;
    focus_2.x = 2 * center.x - focus_1.x;
    focus_2.y = 2 * center.y - focus_2.y;
  }

  void reflex(Line axis) override {
    double k = axis.angle;
    double b = axis.shift;
    double x_1 = (k * focus_1.y + focus_1.x - k * b) / (k * k + 1);
    double y_1 = k * x_1 + b;
    focus_1.x = 2 * x_1 - focus_1.x;
    focus_1.y = 2 * y_1 - focus_1.y;
    double x_2 = (k * focus_1.y + focus_1.x - k * b) / (k * k + 1);
    double y_2 = k * x_2 + b;
    focus_2.x = 2 * x_2 - focus_2.x;
    focus_2.y = 2 * y_2 - focus_2.y;
  }

  void scale(Point center, double coefficient) override {
    focus_1.x = center.x + coefficient * (focus_1.x - center.x);
    focus_1.y = center.y + coefficient * (focus_1.y - center.y);
    focus_2.x = center.x + coefficient * (focus_2.x - center.x);
    focus_2.y = center.y + coefficient * (focus_2.y - center.y);
    distance = coefficient * distance;
  }
};

class Circle : public Ellipse {

public:
  Circle(Point c, double r) : Ellipse(c, c, 2 * r) {}

  double radius() const {
    return distance / 2;
  }

  double perimeter() const override {
    return 2 * PI * radius();
  }

  double area() const override {
    return PI * radius() * radius();
  }
};

class Rectangle : public Polygon {
  double relation_of_sides;

public:
  Rectangle(Point &vert_1, Point &vert_3, double rel) : relation_of_sides(rel) {
    vertices = std::vector<Point>(4);
    if (vert_1.y > vert_3.y) {
      swap(vert_1, vert_3);
    }
    vertices[0] = vert_1;
    vertices[2] = vert_3;
    double diag_length = vert_1.distance_between_points(vert_3);
    double h = diag_length / sqrt(pow(relation_of_sides, 2) + 1);
    double w = relation_of_sides * h;
    double alpha = atan((vert_3.y - vert_1.y) / (vert_3.x - vert_1.x)) - atan(h / w);
    double vert_4_x = vert_1.x + w * cos(alpha);
    double vert_4_y = vert_1.y + w * sin(alpha);
    vertices[3] = Point(vert_4_x, vert_4_y);
    vertices[1] = Point(vert_1.x + vert_3.x - vert_4_x, vert_1.y + vert_3.y - vert_4_y);
  }

  Point center() {
    return Point((vertices[0].x + vertices[2].x) / 2, (vertices[0].y + vertices[2].y) / 2);
  }

  std::pair<Line, Line> diagonals() {
    Line l_1 = Line(vertices[0], vertices[2]);
    Line l_2 = Line(vertices[1], vertices[3]);
    return {l_1, l_2};
  }
};

class Square : public Rectangle {
public:
  Square(Point vert_1, Point vert_3) : Rectangle(vert_1, vert_3, 1) {}

  Circle circumscribedCircle() {
    Point c = center();
    double rad = vertices[0].distance_between_points(vertices[2]) / 2;
    return Circle(c, rad);
  }

  Circle inscribedCircle() {
    Point c = center();
    double rad = vertices[0].distance_between_points(vertices[1]) / 2;
    return Circle(c, rad);
  }
};

class Triangle : public Polygon {
  Point a, b, c;
  double side_1 = a.distance_between_points(b);
  double side_2 = b.distance_between_points(c);
  double side_3 = c.distance_between_points(a);

public:
  Triangle(Point &a, Point &b, Point &c) : Polygon({a, b, c}), a(a), b(b), c(c) {}

  double perimeter() const override {
    return side_1 + side_2 + side_3;
  }

  double area() const override {
    double p = perimeter() / 2;
    double res = sqrt(p * (p - side_1) * (p - side_2) * (p - side_3));
    return res;
  }

  Circle circumscribedCircle() const {
    double r = side_1 * side_2 * side_3 / (4 * area());
    double d = 2 * (a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y));
    double center_x = -((b.x * b.x + b.y * b.y - c.x * c.x - c.y * c.y) * a.y + (c.x * c.x + c.y * c.y - a.x * a.x - a.y * a.y) * b.y +
                        (a.x * a.x + a.y * a.y - b.x * b.x - b.y * b.y) * c.y) /
                      d;
    double center_y = ((b.x * b.x + b.y * b.y - c.x * c.x - c.y * c.y) * a.x + (c.x * c.x + c.y * c.y - a.x * a.x - a.y * a.y) * b.x +
                       (a.x * a.x + a.y * a.y - b.x * b.x - b.y * b.y) * c.x) /
                      d;
    Point cntr(center_x, center_y);
    return Circle(cntr, r);
  }

  Circle inscribedCircle() const {
    double r = 2 * area() / perimeter();
    double center_y = (side_2 * a.y + side_3 * b.y + side_1 * c.y) / perimeter();
    double center_x = (side_2 * a.x + side_3 * b.x + side_1 * c.x) / perimeter();
    Point cntr(center_x, center_y);
    return Circle(cntr, r);
  }

  Point centroid() const {
    return Point((a.x + b.x + c.x) / 3, (a.y + b.y + c.y) / 3);
  }

  Point orthocenter() {
    Line l_1 = Line(a, b);
    double k_1 = l_1.angle;
    Line l_2 = Line(a, c);
    double k_2 = l_2.angle;
    double ort_x = (k_1 * k_2 * (b.y - c.y) + k_1 * b.x - k_2 * c.x) / (k_1 - k_2);
    double ort_y = c.y + (c.x - ort_x) / k_1;
    return Point(ort_x, ort_y);
  }

  Line EulerLine() {
    return Line(circumscribedCircle().center(), orthocenter());
  }

  Circle ninePointsCircle() {
    double r = circumscribedCircle().radius() / 2;
    double center_x = (orthocenter().x + circumscribedCircle().center().x) / 2;
    double center_y = (orthocenter().y + circumscribedCircle().center().y) / 2;
    Point cntr(center_x, center_y);
    return Circle(cntr, r);
  }
};
