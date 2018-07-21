#include "OGRMultiPoint.h"

char* OGRMultiPoint::GetGeometryType()
{
	return "OGRMultiPoint";
}

MBR OGRMultiPoint::GetMBR()
{
	double xmax = points[0].first, xmin = points[0].first;
	double ymax = points[0].second, ymin = points[0].second;
	for (unsigned int i = 1; i < points.size(); i++)
	{
		xmax = max(xmax, points[i].first);
		xmin = min(xmin, points[i].first);
		ymax = max(ymax, points[i].second);
		ymin = min(ymin, points[i].second);
	}
	return make_pair(make_pair(xmin, ymin), make_pair(xmax, ymax));
}

void OGRMultiPoint::Copy(OGRMultiPoint& obj)
{
	points = obj.points;
}

bool OGRMultiPoint::Equals(OGRMultiPoint& obj)
{
	//多点几何相等，需考虑点的索引次序不同情形，这里用标记法记录匹配情况
	unsigned int t = points.size();
	if (t != obj.points.size()) return false;
	short * marked = new short[t];
	memset(marked, 0, t * sizeof(short));
	bool equal = true;
	for (int i = 0; i < t; i++)
	{
		int j;
		for (j = 0; j < t; j++)
		{
			if (marked[j]) continue;
			if (Point_equal(points[i], obj.points[j]))
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

void OGRMultiPoint::AddGeometry(Point& obj)
{
	points.push_back(obj);
}

void OGRMultiPoint::UpdateGeometry(int pos, Point& obj)
{
	if (pos < points.size())
	{
		points[pos] = obj;
	}
}

void OGRMultiPoint::RemoveGeometry(int pos)
{
	if (pos < points.size())
	{
		points.erase(points.begin() + pos);
	}
}

Point& OGRMultiPoint::GetGeometry(int pos)
{
	if (pos < points.size())
	{
		return points[pos];
	}
	else return *(points.end() - 1);
}

int OGRMultiPoint::GetNumGeometries()
{
	return points.size();
}

void OGRMultiPoint::ImportFromWkt(string& filepath)
{
	freopen(filepath.c_str(), "r", stdin);
	char buffer[LINEMAX];
	vector<string> word;
	const char * delim = " ,()";
	char * c;
	cin.clear();
	cin.getline(buffer, LINEMAX);
	//字符串分割，得到"Point"及一系列坐标对
	c = strtok(buffer, delim);
	while (c)
	{
		word.push_back(c);
		if (*(word.end() - 1) == "") word.erase(word.end() - 1);
		c = strtok(NULL, delim);
	}
	for (unsigned int i = 1; i < word.size(); i+=2)
	{
		points.push_back(make_pair(stod(word[i]), stod(word[i + 1])));
	}
	fclose(stdin);
}

void OGRMultiPoint::ExportToWkt(string& filepath)
{
	freopen(filepath.c_str(), "w", stdout);
	cout << "MultiPoint (";
	for (unsigned int i = 0; i < points.size(); i++)
	{
		if (i != 0) cout << ", ";
		cout << "(" << points[i].first << " " << points[i].second << ")";
	}
	cout << ")" << endl;
	fclose(stdout);
}