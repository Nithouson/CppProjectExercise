/*
Filename: geometry.h
Author: Guo Hao
Date: 2018/5/26
*/
#pragma once
#include <vector>
#include <string>
using namespace std;

class geometry
{
protected:
	string name,id;
public:
	void SetName(string&);
	void SetID(string&);
	string GetID(void);
	bool operator<(geometry&);
};

class Position //coordinates
{
public:
	double x, y;
	Position();
	Position(double, double);
	void Move(double, double);
};

class Point:public geometry
{
public:
	Position p;
	Point(double, double);
	bool operator==(const Point& p);
	friend ostream& operator<<(ostream&, const Point&);
};

class Polyline :public geometry
{
	string length;
public:
	vector<Position> point;
	void SetLength(string&);
	friend ostream& operator<<(ostream&, const Polyline&);
};

class Ring :public geometry
{
	string length,area;
public:
	vector<Position> point;
	void SetLength(string&);
	void SetArea(string&);
	bool cover(const Point&);
	friend ostream& operator<<(ostream&, const Ring&);
};

class Polygon:public geometry
{
	string length, area;
public:
	vector<Position> outer,inner;
	void SetLength(string&);
	void SetArea(string&);
	bool cover(const Point&);
	friend ostream& operator<<(ostream&, const Polygon&);
};

inline double Triangle_Area(Position, Position, Position);
inline double Ring_Area(const vector<Position>);