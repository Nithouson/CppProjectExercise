#include "OGRMultiLineString.h"

char* OGRMultiLineString::GetGeometryType()
{
	return "OGRMultiLineString";
}

MBR OGRMultiLineString::GetMBR()
{
	double xmax = lines[0][0].first, xmin = lines[0][0].first;
	double ymax = lines[0][0].second, ymin = lines[0][0].second;
	for (unsigned int j = 0; j < lines.size(); j++)
	{
		for (unsigned int i = 0; i < lines[j].size(); i++)
		{
			xmax = max(xmax, lines[j][i].first);
			xmin = min(xmin, lines[j][i].first);
			ymax = max(ymax, lines[j][i].second);
			ymin = min(ymin, lines[j][i].second);
		}
	}
	return make_pair(make_pair(xmin, ymin), make_pair(xmax, ymax));
}

double OGRMultiLineString::GetLength()
{
	double l = 0;
	for (unsigned int j = 0; j < lines.size(); j++)
	{
		for (unsigned int i = 0; i < lines[j].size() - 1; i++)
		{
			l += Point_dist(lines[j][i], lines[j][i + 1]);
		}
	}
	return l;
}


void OGRMultiLineString::Copy(OGRMultiLineString& obj)
{
	lines = obj.lines;
}

bool OGRMultiLineString::Equals(OGRMultiLineString& obj)
{
	//多折线几何相等，需考虑线的索引次序不同情形，这里用标记法记录匹配情况
	unsigned int t = lines.size();
	if (t != obj.lines.size()) return false;
	short * marked = new short[t];
	memset(marked, 0, t * sizeof(short));
	bool equal = true;
	for (int i = 0; i < t; i++)
	{
		int j;
		for (j = 0; j < t; j++)
		{
			if (marked[j]) continue;
			if (Line_equal(lines[i], obj.lines[j]))
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

void OGRMultiLineString::AddGeometry(Line& obj)
{
	lines.push_back(obj);
}

void OGRMultiLineString::UpdateGeometry(int pos, Line& obj)
{
	if (pos < lines.size())
	{
		lines[pos] = obj;
	}
}

void OGRMultiLineString::RemoveGeometry(int pos)
{
	if (pos < lines.size())
	{
		lines.erase(lines.begin() + pos);
	}
}

Line& OGRMultiLineString::GetGeometry(int pos)
{
	if (pos < lines.size())
	{
		return lines[pos];
	}
	else return *(lines.end() - 1);
}

int OGRMultiLineString::GetNumGeometries()
{
	return lines.size();
}