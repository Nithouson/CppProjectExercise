/*
Filename: main.cpp
Author: Guo Hao
Date: 2018/5/26
Description: the main function of the program, process command line
*/

#include <iostream>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include "geometry.h"
#define LINEMAX 400
#pragma warning(disable:4996)//freopen
using namespace std;

int main()
{
	/*
	cout << "作者：郭浩（1600012426） 提交时间：2018/5/26" << endl;
	cout << "程序的目标:实现读取命令行模式下对点、线、多边形等几何对象的处理." << endl;
	cout << "程序亮点：" << endl;
	cout << "    1.类的继承：用geometry类管理几何对象共有的ID、名称属性;" << endl;
	cout << "    2.区别于有其它属性的point类，用position类存储坐标、作为各几何对象成员;" << endl;
	cout << "    3.通过流插入运算符重载实现自然的对象信息输出;" << endl;
	cout << "Press Enter key to continue..." << endl;
	getchar();
	cout << "Open File..." << endl;
	*/
	vector<Point> vecPoint;
	vector<Polyline> vecPolyline;
	vector<Ring> vecRing;
	vector<Polygon> vecPolygon;
	
	freopen("input.txt","r", stdin);
	freopen("output.txt","w", stdout);
	char buffer[LINEMAX];
	vector<string> word;
	const char * delim = " ,";
	char * c;
	while (cin.getline(buffer,LINEMAX))
	{
		//split command line to get words
		c = strtok(buffer, delim);
		while (c)
		{
			word.push_back(c);
			c = strtok(NULL, delim);
		}
		//Add
		if (word[0] == "add")
		{
			if (word[1] == "point")
			{
				//add point a1 5,6
				Point p(stod(word[3]), stod(word[4]));//C++11 required
				p.SetID(word[2]);
				vecPoint.push_back(p);
			}
			else if (word[1] == "polyline")
			{
				//add polyline a1 5,6,7,8,9,10
				Polyline l;
				l.SetID(word[2]);
				for (unsigned int i = 3; i < word.size(); i += 2)
				{
					l.point.push_back(Position(stod(word[i]), stod(word[i + 1])));
				}
				vecPolyline.push_back(l);
			}
			else if (word[1] == "ring")
			{
				//add ring a1 5,6,7,8,9,10
				Ring r;
				r.SetID(word[2]);
				for (unsigned int i = 3; i < word.size(); i += 2)
				{
					r.point.push_back(Position(stod(word[i]), stod(word[i + 1])));
				}
				vecRing.push_back(r);
			}
			else if (word[1] == "polygon")
			{
				//add polygon a1 outer 0, 2, 2, 2, 2, 0, 0, 0
				Polygon g;
				g.SetID(word[2]);
				for (unsigned int i = 4; i < word.size(); i += 2)
				{
					g.outer.push_back(Position(stod(word[i]), stod(word[i + 1])));
				}
				vecPolygon.push_back(g);
			}
		}
		else if (word[0] == "del")
		{
			if (word[1] == "point")
			{
				//del point a1
				for (unsigned int i = 0; i < vecPoint.size(); i++)
				{
					if (vecPoint[i].GetID()== word[2])
					{
						vecPoint.erase(vecPoint.begin() + i);
						break;
					}
				}
			}
			else if (word[1] == "polyline")
			{
				//del polyline a1
				for (unsigned int i = 0; i < vecPolyline.size(); i++)
				{
					if (vecPolyline[i].GetID() == word[2])
					{
						vecPolyline.erase(vecPolyline.begin() + i);
						break;
					}
				}
			}
			else if (word[1] == "ring")
			{
				//del ring a1
				for (unsigned int i = 0; i < vecRing.size(); i++)
				{
					if (vecRing[i].GetID() == word[2])
					{
						vecRing.erase(vecRing.begin() + i);
						break;
					}
				}
			}
			else if (word[1] == "polygon")
			{
				//del polygon a1
				for (unsigned int i = 0; i < vecPolygon.size(); i++)
				{
					if (vecPolygon[i].GetID() == word[2])
					{
						vecPolygon.erase(vecPolygon.begin() + i);
						break;
					}
				}
			}
		}
		else if (word[0] == "set")
		{
			if (word[1] == "point")
			{
				for (unsigned int i = 0; i < vecPoint.size(); i++)
				{
					if (vecPoint[i].GetID() == word[2])
					{
						if (word[3] == "set")
						{
							//set point a1 set 1,2
							vecPoint[i].p = Position(stod(word[4]), stod(word[5]));
						}
						else if (word[3] == "move")
						{
							//set point a1 move 1,-2
							vecPoint[i].p.Move(stod(word[4]), stod(word[5]));
						}
						else if (word[3] == "name")
						{
							//set point a1 name 北京
							vecPoint[i].SetName(word[4]);
						}
						break;
					}
				}
			}
			else if (word[1] == "polyline")
			{
				for (unsigned int i = 0; i < vecPolyline.size(); i++)
				{
					if (vecPolyline[i].GetID() == word[2])
					{
						if (word[3] == "set")
						{
							//set polyline a1 set 0 3, 5
							vecPolyline[i].point[stoi(word[4])]= Position(stod(word[5]), stod(word[6]));
						}
						else if (word[3] == "move")
						{
							//set polyline a1 move 0 3,-5
							vecPolyline[i].point[stoi(word[4])].Move(stod(word[5]), stod(word[6]));
						}
						else if (word[3] == "del")
						{
							//set polyline a1 del 0
							vecPolyline[i].point.erase(vecPolyline[i].point.begin() + stoi(word[4]));
						}
						else if (word[3] == "set_length")
						{
							//set polyline a1 set_length 1000米
							vecPolyline[i].SetLength(word[4]);
						}
						else if (word[3] == "name")
						{
							//set polyline a1 name 长安街
							vecPolyline[i].SetName(word[4]);
						}
						break;
					}
				}
			}
			else if (word[1] == "ring")
			{
				for (unsigned int i = 0; i < vecRing.size(); i++)
				{
					if (vecRing[i].GetID() == word[2])
					{
						if (word[3] == "set")
						{
							//set ring a1 set 0 3,5
							vecRing[i].point[stoi(word[4])] = Position(stod(word[5]), stod(word[6]));
						}
						else if (word[3] == "move")
						{
							//set ring a1 move 0 3,-5
							vecRing[i].point[stoi(word[4])].Move(stod(word[5]), stod(word[6]));
						}
						else if (word[3] == "del")
						{
							//set ring a1 del 0
							vecRing[i].point.erase(vecRing[i].point.begin() + stoi(word[4]));
						}
						else if (word[3] == "set_length")
						{
							//set ring a1 set_length 1000米
							vecRing[i].SetLength(word[4]);
						}
						else if (word[3] == "set_area")
						{
							//set ring a1 set_area 1000平方米
							vecRing[i].SetArea(word[4]);
						}
						else if (word[3] == "name")
						{
							//set ring a1 name 天安门广场
							vecRing[i].SetName(word[4]);
						}
						break;
					}
				}
			}
			else if (word[1] == "polygon")
			{
				for (unsigned int i = 0; i < vecPolygon.size(); i++)
				{
					if (vecPolygon[i].GetID() == word[2])
					{
						if (word[3] == "inner")
						{
							//set polygon a1 inner 0,1,1,1,1,0,0,0
							vecPolygon[i].inner.clear();
							for (unsigned int j = 4; j < word.size(); j += 2)
							{
								vecPolygon[i].inner.push_back(Position(stod(word[j]), stod(word[j + 1])));
							}
						}
						else if (word[3] == "set")
						{
							if(word[4]=="outer")
								//set polygon a1 set outer 0 3, 5
								vecPolygon[i].outer[stoi(word[5])] = Position(stod(word[6]), stod(word[7]));
							else if(word[4]=="inner")
								//set polygon a1 set inner 0 3,5
								vecPolygon[i].inner[stoi(word[5])] = Position(stod(word[6]), stod(word[7]));
						}
						else if (word[3] == "move")
						{
							if (word[4] == "outer")
								//set polygon a1 move outer 0 3, -5
								vecPolygon[i].outer[stoi(word[5])].Move(stod(word[6]), stod(word[7]));
							else if (word[4] == "inner")
								//set polygon a1 move inner 0 3,-5
								vecPolygon[i].inner[stoi(word[5])].Move(stod(word[6]), stod(word[7]));
						}
						else if (word[3] == "del")
						{
							if (word[4] == "outer")
								//set polygon a1 del outer 0
								vecPolygon[i].outer.erase(vecPolygon[i].outer.begin() + stoi(word[5]));
							else if (word[4] == "inner")
								//set polygon a1 del inner 0
								vecPolygon[i].inner.erase(vecPolygon[i].outer.begin() + stoi(word[5]));
						}
						else if (word[3] == "set_length")
						{
							//set polygon a1 set_length 1000米
							vecPolygon[i].SetLength(word[4]);
						}
						else if (word[3] == "set_area")
						{
							//set polygon a1 set_area 1000平方米
							vecPolygon[i].SetArea(word[4]);
						}
						else if (word[3] == "name")
						{
							//set polygon a1 name 天安门广场
							vecPolygon[i].SetName(word[4]);
						}
						break;
					}
				}
			}
		}
		else if (word[0] == "judge")
		{
			if (word[1] == "point")
			{
				//judge point a1 equal a2
				int apos, bpos;
				//find elements
				for (unsigned int i = 0; i < vecPoint.size(); i++)
				{
					if (vecPoint[i].GetID() == word[2])
					{
						apos = i;
					}
					else if (vecPoint[i].GetID() == word[4])
					{
						bpos = i;
					}
				}
				if (apos == bpos) cout << "True" << endl;
				else cout << "False" << endl;
			}
			else if (word[1] == "ring")
			{
				//judge ring a1 cover a2
				int rpos, ppos;
				for (unsigned int i = 0; i < vecRing.size(); i++)
				{
					if (vecRing[i].GetID() == word[2])
					{
						rpos = i;
						break;
					}	
				}
				for (unsigned int i = 0; i < vecPoint.size(); i++)
				{
					if (vecPoint[i].GetID() == word[4])
					{
						ppos = i;
						break;
					}
				}
				if (vecRing[rpos].cover(vecPoint[ppos]))
					cout << "True" << endl;
				else cout << "False" << endl;
			}
			else if (word[1] == "polygon")
			{
				//judge polygon a1 cover a2
				int gpos, ppos;
				for (unsigned int i = 0; i < vecPolygon.size(); i++)
				{
					if (vecPolygon[i].GetID() == word[2])
					{
						gpos = i;
						break;
					}
				}
				for (unsigned int i = 0; i < vecPoint.size(); i++)
				{
					if (vecPoint[i].GetID() == word[4])
					{
						ppos = i;
						break;
					}
				}
				if (vecPolygon[gpos].cover(vecPoint[ppos]))
					cout << "True" << endl;
				else cout << "False" << endl;
			}
		}
		word.clear();
	}
	
	sort(vecPoint.begin(), vecPoint.end());
	for (unsigned int i = 0; i < vecPoint.size(); i++)
	{
		cout << vecPoint[i] << endl;
	}
	sort(vecPolyline.begin(), vecPolyline.end());
	for (unsigned int i = 0; i < vecPolyline.size(); i++)
	{
		cout << vecPolyline[i] << endl;
	}
	sort(vecRing.begin(), vecRing.end());
	for (unsigned int i = 0; i < vecRing.size(); i++)
	{
		cout << vecRing[i] << endl;
	}
	sort(vecPolygon.begin(), vecPolygon.end());
	for (unsigned int i = 0; i < vecPolygon.size(); i++)
	{
		cout << vecPolygon[i] << endl;
	}
	fclose(stdin);
	fclose(stdout);
	//cout << "Finished" << endl;
	system("PAUSE");
}
