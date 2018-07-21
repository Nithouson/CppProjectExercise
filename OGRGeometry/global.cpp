#include "global.h"

inline bool Point_equal(const Point& a,const Point& b)
{
	return fabs(a.first - b.first) < eps && fabs(a.second - b.second) < eps;
}

bool Line_equal(const Line& a,const Line& b)
{
	//线几何相等，考虑坐标次序相同、相反两种情形
	int i,t = a.size();
	if (t!= b.size()) return false;
	for (i = 0; i < t; i++)
	{
		if (!Point_equal(a[i], b[i])) break;
	}
	if (i == t) return true;
	for (i = 0; i < t; i++)
	{
		if (!Point_equal(a[i], b[t-1-i])) break;
	}
	if (i == t) return true;
	return false;
}

bool Ring_equal(const Line& a, const Line& b)
{
	//环几何相等，需遍历寻找匹配的起点，考虑顺逆时针相同与相反
	int i, t = a.size();
	if (t != b.size()) return false;
	for (int s = 0; s < t; s++)
	{
		for (i = 0; i < t; i++)
		{
			if (!Point_equal(a[i], b[(s+i)%t])) break;
		}
		if (i == t) return true;
		for (i = 0; i < t; i++)
		{
			if (!Point_equal(a[i], b[(t+s-i)%t])) break;
		}
		if (i == t) return true;
	}
	return false;
}

double Point_dist(const Point& a,const Point& b)
{
	return sqrt((a.first - b.first)*(a.first - b.first) + (a.second - b.second)*(a.second - b.second));
}

inline double Triangle_area(const Point& a,const Point&b ,const Point& c)
{
	return 0.5*fabs(b.first*c.second - c.first*b.second + a.second*c.first - a.first*c.second + a.first*b.second - a.second*b.first);
}

double Ring_area(const Line& v)
{
	double s = 0;
	for (unsigned int i = 0; i < v.size() - 1; i++)
	{
		s += v[i].second*v[i + 1].first - v[i].first*v[i + 1].second;
	}
	s += v[v.size() - 1].second * v[0].first - v[v.size() - 1].first * v[0].second;
	s = 0.5*fabs(s);
	return s;
}

bool Ring_cover(const Line& v, const Point& p)
{
	//面积法，只对凸多边形有效
	double s = 0;
	for (unsigned int i = 0; i < v.size() - 1; i++)
	{
		s += Triangle_area(p, v[i], v[i + 1]);
	}
	s += Triangle_area(p,v[v.size() - 1], v[0]);

	if (fabs(s - Ring_area(v)) < eps)
	{
		return true;
	}
	else return false;
}