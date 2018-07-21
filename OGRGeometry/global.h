#pragma once
#include <vector>
#include <algorithm>
#include <iostream>
#include <string>
#define LINEMAX 500 //读取文本时一行最大长度
#pragma warning(disable:4996) //为使用freopen函数
#pragma warning(disable:4018) //此为无符号数(vector.size())与有符号数比较，已检查
//如要消除，需改动类的定义，因而不再更改
#pragma warning(disable:4703) //指针变量可能未初始化，由main产生
using namespace std;

typedef pair<double, double> Point;
typedef vector<Point> Line;
typedef pair<Point, Point> MBR;

const double eps = 1e-5; //位置容错

//各几何类多次调用的全局函数
bool Point_equal(const Point&, const Point&);//坐标对几何相等
bool Line_equal(const Line&,const Line&);//坐标序列（线）几何相等
bool Ring_equal(const Line&,const Line&);//环几何相等
double Point_dist(const Point&, const Point&);//两点欧氏距离
inline double Triangle_area(const Point&, const Point&, const Point&);//三角形面积
double Ring_area(const Line&);//环的面积
bool Ring_cover(const Line&, const Point&);//环覆盖点