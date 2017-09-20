#ifndef LSYS_H
#define LSYS_H

#include <linecalc.h>
#include <circlecalc.h>
#include <vector>
#include <string>
#include <stack>
#include <unordered_map>
#include <time.h>

#define PI 3.14159265

using namespace std;

class LSys
{
public:

	// Array of string encodings
	// (each string in the array is for a certain iteration)
	string encoding[6];

	// Position of the drawing turtle
	pair<float, float> pos;

	// Orientation angle of the drawing turtle (w.r.t. +X axis)
	float angle;

	// Angle change that occurs with each + or - sign
	float delta;

	// Distance the turtle covers in one stride
	float dist;

	// Stack to store position and orientation of
	// the turtle when entering a branch
	stack<pair<pair<float, float>, float> > stk;

	// A hash-map that maps characters to their rewrite strings
	unordered_map<char, string> rewriteRule;

	// ****************************************************
	// Array of pairs of float vectors to store vertices
	// at each of the 6 iterations:
	// - The first vector in each pair stores positions
	//   of the green colored points of the tree
	// - The second vector in each pair stores positions
	//   of the points that denote the flowers of the tree
	// ****************************************************
	pair<vector<float>, vector<float> > v[6];

	LSys(unsigned int treeType)
	{
		encoding[0] = "T";
		if (treeType == 1)
		{
			dist = 50;
			delta = 28.0f;
			rewriteRule['T'] = "T[+TR][-TR][TR]";
		}
		if (treeType == 2)
		{
			dist = 10;
			delta = 22.5f;
			rewriteRule['T'] = "TT-[-T+T+T]+[+T-T-T]";
		}
		if (treeType == 3)
		{
			dist = 10;
			delta = 22.5f;
			rewriteRule['T'] = "TT-[-T+T+T]+[+T-T-T]";
		}
		pos = {0.0f, 0.0f};
		angle = 90.0f;
	}

	void expand()
	{
		for (int e = 0; e < 5; e++)
		{
			string rewrite;
			for (size_t i = 0; i < encoding[e].size(); i++)
			{
				char ch = encoding[e][i];
				if (rewriteRule.find(ch) != rewriteRule.end())
				{
					rewrite += rewriteRule[ch];
				}
				else
				{
					rewrite += ch;
				}
			}
			encoding[e + 1] = rewrite;
		}
	}

	pair<float, float> nextPos()
	{
		pair<float, float> npos;
		npos.first = pos.first + dist * (float)cos(angle * PI / 180);
		npos.second = pos.second + dist * (float)sin(angle * PI / 180);
		return npos;
	}

	void setVertices()
	{
		for (int e = 0; e < 6; e++)
		{
			for (size_t i = 0; i < encoding[e].length(); i++)
			{
				if (encoding[e][i] == 'T')
				{
					pair<float, float> npos = nextPos();
					npos = {round(npos.first), round(npos.second)};
					LineCalc twig(pos, npos);
					vector<float> points = twig.getVertexVector();
					for (size_t j = 0; j < points.size(); j++)
					{
						v[e].first.push_back(points[j]);
					}
					pos = npos;
				}
				if (encoding[e][i] == 'R')
				{
					v[e].second.push_back(pos.first);
					v[e].second.push_back(pos.second);
					v[e].second.push_back(0.0f);
				}
				if (encoding[e][i] == '[')
				{
					stk.push({pos, angle});
				}
				if (encoding[e][i] == ']')
				{
					pos = stk.top().first;
					angle = stk.top().second;
					stk.pop();
				}
				if (encoding[e][i] == '-')
				{
					angle -= delta;
				}
				if (encoding[e][i] == '+')
				{
					angle += delta;
				}
			}
			pos = {0.0f, 0.0f};
		}
	}
};
#endif