#include "OGRPolygon.h"

char* OGRPolygon::GetGeometryType()
{
	return "OGRPolygon";
}

MBR OGRPolygon::GetMBR()
{
	double xmax = rings[0][0].first, xmin = rings[0][0].first;
	double ymax = rings[0][0].second, ymin = rings[0][0].second;
	for (int i = 1; i < rings[0].size(); i++)
	{
		xmax = max(xmax, rings[0][i].first);
		xmin = min(xmin, rings[0][i].first);
		ymax = max(ymax, rings[0][i].second);
		ymin = min(ymin, rings[0][i].second);
	}
	return make_pair(make_pair(xmin, ymin), make_pair(xmax, ymax));
}

vector<Line>& OGRPolygon::GetRings()
{
	return rings;
}

double OGRPolygon::GetPerimeter()
{
	double l = 0;
	for (int j = 0; j < rings.size(); j++)
	{
		for (int i = 0; i < rings[j].size() - 1; i++)
		{
			l += Point_dist(rings[j][i], rings[j][i + 1]);
		}
		l += Point_dist(rings[j][rings[j].size() - 1], rings[j][0]);
	}
	return l;
}

double OGRPolygon::GetArea()
{
	double a = 0;
	a += Ring_area(rings[0]);
	for (int j = 1; j < rings.size(); j++)
	{
		a -= Ring_area(rings[j]);
	}
	return a;
}


void OGRPolygon::Copy(OGRPolygon& obj)
{
	rings = obj.rings;
}

bool OGRPolygon::Equals(OGRPolygon& obj)
{
	//多边形几何相等，需考虑内环的索引次序不同情形，用标记数组记录匹配
	int t = rings.size();
	if (t != obj.rings.size()) return false;
	if (!Ring_equal(rings[0], obj.rings[0])) return false;
	short * marked = new short[t];
	memset(marked, 0, t*sizeof(short));
	bool equal = true;
	for (int i = 1; i < t; i++)
	{
		int j;
		for (j = 1; j < t; j++)
		{
			if (marked[j]) continue;
			if (Ring_equal(rings[i], obj.rings[j])) 
			{
				marked[j] = 1; 
				break;
			}
		}
		if (j == t) 
		{
			equal = false; 
			break;
		}
	}
	delete[]marked;
	return equal;
}

bool OGRPolygon::Contains(OGRPoint& obj)
{
	//点在多边形内，即在外环之内、所有内环之外
	if (!Ring_cover(rings[0], obj.GetPoint())) return false;
	for (int j = 1; j < rings.size(); j++)
	{
		if (Ring_cover(rings[j], obj.GetPoint())) return false;
	}
	return true;
}

bool OGRPolygon::Contains(OGRLineString& obj)
{
	//折线在凸多边形内，即折线每个点在多边形内（*this有内环时可能出错）
	for (int i = 0; i < obj.GetPoints().size(); i++)
	{
		if (!Ring_cover(rings[0], (obj.GetPoints())[i])) return false;
		for (int j = 1; j < rings.size(); j++)
		{
			if (Ring_cover(rings[j], (obj.GetPoints())[i])) return false;
		}
	}
	return true;
}

bool OGRPolygon::Contains(OGRPolygon& obj)
{
	//多边形在凸多边形内，即多边形每个点在多边形内（*this有内环时可能出错）
	for (int jj = 0; jj < obj.rings.size(); jj++)
	{
		for (int i = 0; i < obj.rings[jj].size(); i++)
		{
			if (!Ring_cover(rings[0], obj.rings[jj][i])) return false;
			for (int j = 1; j < rings.size(); j++)
			{
				if (Ring_cover(rings[j], obj.rings[jj][i])) return false;
			}
		}
	}
	return true;
}

void OGRPolygon::ImportFromWkt(string& filepath)
{
	if (freopen(filepath.c_str(), "r", stdin) == NULL) return;
	char buffer[LINEMAX],buffmod[LINEMAX];
	vector<string> word;
	char * c;
	cin.clear();
	cin.getline(buffer, LINEMAX);
	//预处理，右括号左右加空格，使右括号可以被分割开
	int p = 0, q = 0;
	while (buffer[p])
	{
		if (buffer[p] == ')')
		{
			buffmod[q++] = ' ';
			buffmod[q++] = ')';
			buffmod[q++] = ' ';
		}
		else buffmod[q++] = buffer[p];
		p++;
	}
	buffmod[q] = '\0';

	const char * delim = " (,";
	c = strtok(buffmod, delim);
	while (c)
	{
		word.push_back(c);
		if (*(word.end() - 1) == "") word.erase(word.end() - 1);
		c = strtok(NULL, delim);
	}
	word.erase(word.end() - 1);//删去最后一个右括号
	//字符串分割,保留"Polygon",x,y坐标以及右括号
	p = 1;
	int r = 0;
	rings.clear();
	rings.push_back(*new Line());
	while (p < word.size())
	{
		rings[r].push_back(make_pair(stod(word[p]), stod(word[p + 1])));
		p += 2;
		//根据右括号判断进入新的环或新的多边形
		if (p < word.size() && word[p] == ")") 
		{
			rings.push_back(*new Line());
			p++; r++;
		}
	}
	rings.erase(rings.end() - 1);
	fclose(stdin);
}

void OGRPolygon::ExportToWkt(string& filepath)
{
	freopen(filepath.c_str(), "w", stdout);
	cout << "Polygon (";
	for (int r = 0; r < rings.size(); r++)
	{
		if (r != 0) cout << ", ";
		cout << "(";
		for (int i = 0; i < rings[r].size(); i++)
		{
			if (i != 0) cout << ", ";
			cout << rings[r][i].first << " " << rings[r][i].second;
		}
		cout << ")";
	}
	cout << ")" << endl;
	fclose(stdout);
}