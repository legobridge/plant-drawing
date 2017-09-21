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

	// ******************************************************
	// Array of vectors of float vectors to store vertices
	// at each of the 6 iterations:
	// - The first vector in each element of the array stores
	//   positions of the green colored points of the tree
	// - The second vector in each element of the array stores
	//   positions of the brown colored points of the tree
	// - The third vector in each element of the array stores
	//   positions of the light brown colored points of the tree
	// - The fourth vector in each element of the array stores
	//   positions of the orange colored points of the tree
	// - The fifth vector in each element of the array stores
	//   positions of the red colored points of the tree
	// ******************************************************
	vector<vector<float> > v[6];

	// Branch depth during vertex calculations
	int depth = 0;

	LSys(unsigned int treeType)
	{
		if (treeType == 1)
		{
			encoding[0] = "KT";
			dist = 25.0f;
			delta = 25.0f;
			rewriteRule['T'] = "KT[++TTR][+T][-TT][-T++TR][--TTR]R";
			// rewriteRule['T'] = "KT[++TTR][+TR][-TTR][-T++TR][--TTR]R";
		}
		if (treeType == 2)
		{
			encoding[0] = "T";
			dist = 14.0f;
			delta = 24.0f;
			rewriteRule['T'] = "TT-[-T+T+T]+[+T-T-T]R";
		}
		if (treeType == 3)
		{
			encoding[0] = "T";
			dist = 14.0f;
			delta = 21.0f;
			rewriteRule['T'] = "T[++T[-TR]]T[-TT[TR]]R";
		}
		pos = {0.0f, 0.0f};
		angle = 90.0f;
		depth = 0;
		srand((int)time(NULL));
	}

	void expand()
	{
		for (int e = 0; e < 4; e++)
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
		for (int e = 0; e < 5; e++)
		{
			v[e].resize(5);
			for (size_t i = 0; i < encoding[e].length(); i++)
			{
				if (encoding[e][i] == 'T' || encoding[e][i] == 'K')
				{
					pair<float, float> npos = nextPos();
					npos = {round(npos.first), round(npos.second)};

					int y = max(3 - depth, 0);
					for (int x = -y; x <= y; x++)
					{
						LineCalc Twig({pos.first + round(x * sin(angle)), pos.second + round(x * cos(angle))}, {npos.first + round(x * sin(angle)), npos.second + round(x * cos(angle))});
						vector<float> points = Twig.getVertexVector();
						for (size_t j = 0; j < points.size(); j += 3)
						{
							if (depth == 0)
							{
								v[e][1].push_back(points[j]);
								v[e][1].push_back(points[j + 1]);
								v[e][1].push_back(points[j + 2]);
							}
							else if (depth == 1)
							{
								v[e][2].push_back(points[j]);
								v[e][2].push_back(points[j + 1]);
								v[e][2].push_back(points[j + 2]);
							}
							else
							{
								v[e][0].push_back(points[j]);
								v[e][0].push_back(points[j + 1]);
								v[e][0].push_back(points[j + 2]);
							}
						}
					}
					pos = npos;
				}
				if (encoding[e][i] == 'R')
				{
					int chance = rand() % 100;
					if ((chance > 80 && depth == e - 1) || (chance > 30 && depth == e))
					{
						int x = 3 + rand() % 2;
						float flowerRadius = (float)(rand() % 5) + 1;
						while (flowerRadius--)
						{
							CircleCalc flower(pos, flowerRadius);
							flower.computeVertexVector();
							for (size_t j = 0; j < flower.v.size(); j += 3)
							{
								v[e][x].push_back(flower.v[j]);
								v[e][x].push_back(flower.v[j + 1]);
								v[e][x].push_back(flower.v[j + 2]);
							}
						}
					}
				}
				if (encoding[e][i] == '[')
				{
					stk.push({pos, angle});
					depth++;
				}
				if (encoding[e][i] == ']')
				{
					pos = stk.top().first;
					angle = stk.top().second;
					stk.pop();
					depth--;
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