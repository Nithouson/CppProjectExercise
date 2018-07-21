#include "OGRMultiPolygon.h"

char* OGRMultiPolygon::GetGeometryType()
{
	return "OGRMultiPolygon";
}

MBR OGRMultiPolygon::GetMBR()
{
	double xmax = polygons[0][0][0].first, xmin = polygons[0][0][0].first;
	double ymax = polygons[0][0][0].second, ymin = polygons[0][0][0].second;
	for (unsigned int j = 0; j < polygons.size(); j++)
	{
		for (unsigned int i = 0; i < polygons[j][0].size(); i++)
		{
			xmax = max(xmax, polygons[j][0][i].first);
			xmin = min(xmin, polygons[j][0][i].first);
			ymax = max(ymax, polygons[j][0][i].second);
			ymin = min(ymin, polygons[j][0][i].second);
		}
	}
	return make_pair(make_pair(xmin, ymin), make_pair(xmax, ymax));
}

double OGRMultiPolygon::GetPerimeter()
{
	double l = 0;
	for (unsigned int k = 0; k < polygons.size(); k++)
	{
		for (unsigned int j = 0; j < polygons[k].size(); j++)
		{
			for (unsigned int i = 0; i <polygons[k][j].size() - 1; i++)
			{
				l += Point_dist(polygons[k][j][i], polygons[k][j][i + 1]);
			}
			l += Point_dist(polygons[k][j][polygons[k][j].size() - 1], polygons[k][j][0]);
		}
	}
	return l;
}

double OGRMultiPolygon::GetArea()
{
	double a = 0;
	for (unsigned int k = 0; k < polygons.size(); k++)
	{
		a += Ring_area(polygons[k][0]);
		for (unsigned int j = 1; j <polygons[k].size(); j++)
		{
			a -= Ring_area(polygons[k][j]);
		}
	}
	return a;
}

void OGRMultiPolygon::Copy(OGRMultiPolygon& obj)
{
	polygons = obj.polygons;
}

bool OGRMultiPolygon::Equals(OGRMultiPolygon& obj)
{
	//多个多边形几何相等，需考虑多边形的索引次序不同，
	//以及每个多边形内环索引次序不同的情况。
	//这里用标记法记录匹配情况。
	unsigned int t = polygons.size();
	if (t != obj.polygons.size()) return false;
	short * pmarked = new short[t];
	memset(pmarked, 0, t * sizeof(short));
	bool mequal = true;
	for (int i = 0; i < t; i++)
	{
		unsigned int s = polygons[i].size();
		int j;
		for (j = 0; j < t; j++)
		{
			if (pmarked[j]) continue;
			if (s!= obj.polygons[j].size()) continue;
			if (!Ring_equal(polygons[i][0], obj.polygons[j][0])) continue;
			short * rmarked = new short[s];
			memset(rmarked, 0, s * sizeof(short));
			bool pequal = true;
			for (int k = 1; k < s; k++)
			{
				int l;
				for (l = 1; l < s; l++)
				{
					if (rmarked[l]) continue;
					if (Ring_equal(polygons[i][k], obj.polygons[j][l]))
					{
						rmarked[l] = 1;
						break;
					}
				}
				if (l == s)
				{
					pequal = false;
					break;
				}
			}
			delete[]rmarked;
			if (pequal)
			{
				pmarked[j] = 1;
				break;
			}
		}
		if (j == t)
		{
			mequal = false;
			break;
		}
	}
	delete[]pmarked;
	return mequal;
}

bool OGRMultiPolygon::Contains(OGRPoint& obj)
{
	//点在多个多边形内，即在其中一个多边形内
	for (unsigned int k = 0; k < polygons.size(); k++)
	{
		if (!Ring_cover(polygons[k][0], obj.GetPoint())) break;
		int j;
		for (j = 1; j < polygons[k].size(); j++)
		{
			if (Ring_cover(polygons[k][j], obj.GetPoint())) break;
		}
		if(j==polygons[k].size()) return true;
	}
	return false;
}

bool OGRMultiPolygon::Contains(OGRLineString& obj)
{
	//折线在多个多边形内，即在其中一个多边形内
	for (unsigned int k = 0; k < polygons.size(); k++)
	{
		bool cover = true;
		for (unsigned int i = 0; i < obj.GetPoints().size(); i++)
		{
			if (!Ring_cover(polygons[k][0], (obj.GetPoints())[i]))
			{
				cover = false;
				break;
			}
			for (unsigned int j = 1; j < polygons[k].size(); j++)
			{
				if (Ring_cover(polygons[k][j], (obj.GetPoints())[i]))
				{
					cover = false;
					break;
				}
			}
			if (!cover) break;
		}
		if (cover) return true;
	}
	return false;
}

bool OGRMultiPolygon::Contains(OGRPolygon& obj)
{
	//多边形在多个多边形内，即在其中一个多边形内
	for (unsigned int k = 0; k < polygons.size(); k++)
	{
		bool cover = true;
		for (unsigned int jj = 0; jj < obj.GetRings().size(); jj++)
		{
			for (unsigned int i = 0; i < obj.GetRings()[jj].size(); i++)
			{
				if (!Ring_cover(polygons[k][0], (obj.GetRings())[jj][i]))
				{
					cover = false;
					break;
				}
				for (unsigned int j = 1; j < polygons[k].size(); j++)
				{
					if (Ring_cover(polygons[k][j], (obj.GetRings())[jj][i]))
					{
						cover = false;
						break;
					}
				}
				if (!cover) break;
			}
		}
		if (cover) return true;
	}
	return false;
}

void OGRMultiPolygon::AddGeometry(vector<Line>& obj)
{
	polygons.push_back(obj);
}

void OGRMultiPolygon::UpdateGeometry(int pos, vector<Line>& obj)
{
	if (pos < polygons.size())
	{
		polygons[pos] = obj;
	}
}

void OGRMultiPolygon::RemoveGeometry(int pos)
{
	if (pos < polygons.size())
	{
		polygons.erase(polygons.begin() + pos);
	}
}

vector<Line>& OGRMultiPolygon::GetGeometry(int pos)
{
	if (pos < polygons.size())
	{
		return polygons[pos];
	}
	else return *(polygons.end() - 1);
}

int OGRMultiPolygon::GetNumGeometries()
{
	return polygons.size();
}

void OGRMultiPolygon::ImportFromWkt(string& filepath)
{
	freopen(filepath.c_str(), "r", stdin);
	char buffer[LINEMAX], buffmod[LINEMAX];
	vector<string> word;
	const char * delim = " (,";
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
	c = strtok(buffmod, delim);
	while (c)
	{
		word.push_back(c);
		if (*(word.end() - 1) == "") word.erase(word.end() - 1);
		c = strtok(NULL, delim);
	}
	word.erase(word.end() - 1);//删去最后一个右括号
	//字符串分割,保留"Polygon",x,y坐标以及右括号
	unsigned int w = 1, poly=0,ring = 0;
	polygons.clear();
	polygons.push_back(*new vector<Line>());
	polygons[0].push_back(*new Line);
	while (w < word.size())
	{
		polygons[poly][ring].push_back(make_pair(stod(word[w]), stod(word[w+ 1])));
		w+= 2;
		//根据右括号判断进入新的环或新的多边形
		if (w < word.size() && word[w] == ")")
		{
			polygons[poly].push_back(*new Line());
			ring++; w++;
			if (w < word.size() && word[w] == ")")
			{
				polygons.push_back(*new vector<Line>);
				poly++; ring = 0; w++;
				polygons[poly].push_back(*new Line);
			}
		}
	}
	polygons.erase(polygons.end() - 1);
	for (int p = 0; p < polygons.size(); p++)
	{
		polygons[p].erase(polygons[p].end() - 1);
	}
	fclose(stdin);
}

void OGRMultiPolygon::ExportToWkt(string& filepath)
{
	freopen(filepath.c_str(), "w", stdout);
	cout << "MultiPolygon (";
	for (unsigned int p = 0; p < polygons.size(); p++)
	{
		if (p != 0) cout << ", ";
		cout << "(";
		for (unsigned int r = 0; r < polygons[p].size(); r++)
		{
			if (r != 0) cout << ", ";
			cout << "(";
			for (unsigned int i = 0; i <polygons[p][r].size(); i++)
			{
				if (i != 0) cout << ", ";
				cout << polygons[p][r][i].first << " " << polygons[p][r][i].second;
			}
			cout << ")";
		}
		cout << ")";
	}
	cout << ")" << endl;
	fclose(stdout);
}