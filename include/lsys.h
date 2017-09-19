#ifndef LSYS_H
#define LSYS_H

#include <linecalc.h>
#include <circlecalc.h>
#include <vector>
#include <string>
#include <stack>
#include <time.h>

#define PI 3.14159265

using namespace std;

class LSys
{
public:
	unsigned int treeType;
	string encoding;
	pair<float, float> pos;
	float angle;
	float dist;
	stack<pair<pair<float, float>, float> > stk;
	vector<float> delta;
	int depth;

	LSys(unsigned int treeType, string axiom)
	{
		this -> treeType = treeType;
		encoding = axiom;
		pos = {0.0f, 0.0f};
		angle = 90.0f;
		dist = 50;
		delta = {28.0f, 27.0f, 26.0f, 25.0f};
		depth = 0;
	}

	void expand1()
	{
		string rewrite;
		for (size_t i = 0; i < encoding.size(); i++)
		{
			if (encoding[i] == 'T')
			{
				rewrite += "T[+TR][-TR][TR]";
			}
			else
			{
				rewrite += encoding[i];
			}
		}
		encoding = rewrite;
	}

	void expand2()
	{
		encoding += "F";
	}

	void expand3()
	{
		encoding += "F";
	}

	void expand()
	{
		if (treeType == 1)
		{
			expand1();
		}
		if (treeType == 2)
		{
			expand2();
		}
		if (treeType == 3)
		{
			expand3();
		}
	}

	pair<float, float> nextPos()
	{
		pair<float, float> npos;
		npos.first = pos.first + dist * (float)cos(angle * PI / 180);
		npos.second = pos.second + dist * (float)sin(angle * PI / 180);
		return npos;
	}

	pair<vector<float>, vector<float> > getVertices()
	{
		pair<vector<float>, vector<float> > p;
		for (size_t i = 0; i < encoding.length(); i++)
		{
			if (encoding[i] == 'T' || encoding[i] == 'K')
			{
				pair<float, float> npos = nextPos();
				npos = {round(npos.first), round(npos.second)};
				LineCalc twig(pos, npos);
				vector<float> points = twig.getVertexVector();
				for (size_t j = 0; j < points.size(); j++)
				{
					p.first.push_back(points[j]);
				}
				pos = npos;
			}
			if (encoding[i] == '[')
			{
				stk.push({pos, angle});
				depth++;
			}
			if (encoding[i] == ']')
			{
				pos = stk.top().first;
				angle = stk.top().second;
				stk.pop();
				depth--;
			}
			if (encoding[i] == '-')
			{
				angle -= delta[depth / 2];
			}
			if (encoding[i] == '+')
			{
				angle += delta[depth / 2];
			}
		}
		/*for (size_t j = 0; j < p.first.size(); j += 3)
		{
			cout << p.first[j] << " " << p.first[j + 1] << " " << p.first[j + 2] << endl;
		}
		cout << endl;*/
		return p;
	}
};
#endif