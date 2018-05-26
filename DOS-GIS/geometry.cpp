/*
Filename: geometry.cpp
Author: Guo Hao
Date: 2018/5/26
Description: implementation of geometry classes
*/
#include <vector>
#include <string>
#include <cstdlib>
#include "geometry.h"
const double eps = 1e-5;//LogicZero

//geometry
void geometry::SetName(string& str)
{
	name = str;
}

void geometry::SetID(string&  str)
{
	id = str;
}

string geometry::GetID(void)
{
	return id;
}

bool geometry::operator<(geometry& arg)
{
	return id < arg.GetID();
}

//Position
void Position::Move(double dx, double dy)
{
	x += dx;
	y += dy;
}

Position::Position() {}
Position::Position(double _x, double _y) :x(_x), y(_y) {}

//Point
Point::Point(double x, double y)
{
	p.x = x;
	p.y = y;
}

bool Point::operator==(const Point& arg)
{
	if (fabs(p.x - arg.p.x) < eps && fabs(p.y - arg.p.y) < eps)
		return true;
	else return false;
}

void Polyline::SetLength(string& str) 
{
	length =str;
}

//Ring
void Ring::SetLength(string& str)
{
	length = str;
}

void Ring::SetArea(string& str)
{
	area = str;
}

bool Ring::cover(const Point& arg)
{
	//area method,if inside,total area of triangles equals the area of polygon
	//if outside, it will exceed the area of the polygon
	//only valid in convex cases
	double s = 0;
	for (unsigned int i = 0; i < point.size()-1; i++)
	{
		s += Triangle_Area(arg.p, point[i], point[i + 1]);
	}
	s += Triangle_Area(arg.p, point[point.size() - 1], point[0]);

	if (fabs(s - Ring_Area(point))< eps)
		return true;
	else return false;
}

//Polygon
void Polygon::SetLength(string& str)
{
	length = str;
}

void Polygon::SetArea(string& str)
{
	area = str;
}

bool Polygon::cover(const Point& arg)
{
	//area method,if inside,total area of triangles equals the area of polygon
	//if outside, it will exceed the area of the polygon
	//only valid in convex cases

	//point inside polygon: inside outer ring && outside inner ring
	double s = 0;
	for (unsigned int i = 0; i < outer.size() - 1; i++)
	{
		s += Triangle_Area(arg.p, outer[i], outer[i + 1]);
	}
	s += Triangle_Area(arg.p, outer[outer.size() - 1], outer[0]);

	if (fabs(s - Ring_Area(outer)) < eps)
	{
		if (inner.size()> 0)
		{
			s = 0;
			for (unsigned int i = 0; i < inner.size() - 1; i++)
			{
				s += Triangle_Area(arg.p, inner[i], inner[i + 1]);
			}
			s += Triangle_Area(arg.p, inner[inner.size() - 1], inner[0]);
			if (fabs(s - Ring_Area(inner)) < eps) return false;
		}
		return true;
	}
	else return false;
}

//output module
ostream& operator<<(ostream& o, const Point& arg)
{
	o << "point " << arg.id << " [" << arg.p.x << "," << arg.p.y << "], name=";
	if(arg.name.empty());
	else o << arg.name ;
	return o;
}

ostream& operator<<(ostream& o, const Polyline& arg)
{
	o << "polyline " << arg.id << " [";
	for (unsigned int i = 0; i < arg.point.size(); i++)
	{
		o << "[" << arg.point[i].x << "," << arg.point[i].y << "]";
		if (i != arg.point.size() - 1) o << ", ";
	}
	o << "], name=";
	if (arg.name.empty());
	else o << arg.name;
	o << ", length=";
	if (arg.length.empty()) o << "NULL";
	else o << arg.length;
	return o;
}

ostream& operator<<(ostream& o, const Ring& arg)
{
	o << "ring " << arg.id << " [";
	for (unsigned int i = 0; i < arg.point.size(); i++)
	{
		o << "[" << arg.point[i].x << "," << arg.point[i].y << "], ";
	}
	o <<"[" << arg.point[0].x << "," << arg.point[0].y << "]], name=";
	if (arg.name.empty());
	else o << arg.name;
	o << ", length=";
	if (arg.length.empty()) o << "NULL";
	else o << arg.length;
	o << ", area=";
	if (arg.area.empty()) o << "NULL";
	else o << arg.area;
	return o;
}

ostream& operator<<(ostream& o, const Polygon& arg)
{
	o << "polygon " << arg.id << " outer=[";
	for (unsigned int i = 0; i < arg.outer.size(); i++)
	{
		o << "[" << arg.outer[i].x << "," << arg.outer[i].y << "], ";
	}
	o << "[" << arg.outer[0].x << "," << arg.outer[0].y << "]], inner=[";
	if (arg.inner.size()> 0)
	{
		for (unsigned int j = 0; j < arg.inner.size(); j++)
		{
			o << "[" << arg.inner[j].x << "," << arg.inner[j].y << "], ";
		}
		o << "[" << arg.inner[0].x << "," << arg.inner[0].y << "]";
	}
	o << "], name=";
	if (arg.name.empty());
	else o << arg.name;
	o << ", length=";
	if (arg.length.empty()) o << "NULL";
	else o << arg.length;
	o << ", area=";
	if (arg.area.empty()) o << "NULL";
	else o << arg.area;
	return o;
}

//used in cover judgements
inline double Triangle_Area(Position a, Position b, Position c)
{
	return 0.5*fabs(b.x*c.y - c.x*b.y + a.y*c.x - a.x*c.y + a.x*b.y - a.y*b.x);
}

inline double Ring_Area(const vector<Position> v)
{
	double s = 0;
	for (unsigned int i = 0; i < v.size()- 1; i++)
	{
		s += v[i].y*v[i + 1].x - v[i].x*v[i + 1].y;
	}
	s += v[v.size() - 1].y * v[0].x - v[v.size() - 1].x * v[0].y;
	s = 0.5*fabs(s);
	return s;
}