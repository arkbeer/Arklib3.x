#pragma once
#define SHAPE_HEADER
#include<iostream>
#include<array>
#include<cmath>
namespace ark {
	struct Vec2 {
		double x, y;
		Vec2(const double x_, const double y_) :x(x_), y(y_) {}
		Vec2() :Vec2(0, 0) {}
		Vec2 operator+(const Vec2& v2) {
			Vec2 v(x, y);
			v.x += v2.x;
			v.y += v2.y;
			return v;
		}
		Vec2 operator-(const Vec2& v2) {
			Vec2 v(x, y);
			v.x -= v2.x;
			v.y -= v2.y;
			return v;
		}
		Vec2& operator+=(const Vec2& v2) {
			x += v2.x, y += v2.y;
			return *this;
		}
		Vec2& operator-=(const Vec2& v2) {
			x -= v2.x, y -= v2.y;
			return *this;
		}
		Vec2& operator/=(const double n) {
			x /= n, y /= n;
			return *this;
		}
		Vec2& operator=(const Vec2& v2) {
			x = v2.x, y = v2.y;
			return *this;
		}
		bool operator==(const Vec2& v2) const {
			return (x == v2.x && y == v2.y);
		}
		bool operator!=(const Vec2& v2) const {
			return !(*this == v2);
		}
	};
	std::ostream& operator<<(std::ostream& ost, const Vec2& v2) {
		ost << "(" << v2.x << "," << v2.y << ")";
		return ost;
	}
	namespace Shape2D {
		template<int N>
		struct Shape {
			std::array<Vec2, N> Vertex;
			Shape(const std::array<Vec2, N>& v2) :Vertex(v2) {}
		};
		template<int N>
		std::ostream& operator<<(std::ostream& ost, const Shape<N>& sp) {
			ost << "Vertex:";
			for (const auto& v : sp.Vertex) {
				ost << v << std::endl;
			}
			return ost;

		}
		struct Ellipse :public Shape<1> {
			double xRadius;
			double yRadius;
			Ellipse(const Vec2& v2, const double xr, const double yr) :Shape<1>({ v2 }), xRadius(xr), yRadius(yr) {}
			Ellipse(const Vec2& v2, const double r) :Ellipse(v2, r, r) {}
		};
		struct Rectangle :public Shape<4> {
			Rectangle(const Vec2& v2, const double width, const double height) :Shape<4>({ v2,Vec2(v2.x + width,v2.y),Vec2(v2.x + width,v2.y + height),Vec2(v2.x,v2.y + height) }) {}
		};
		typedef Shape<2> Line;
		typedef Shape<3> Triangle;
		typedef Shape<4> Square;
	}
}
