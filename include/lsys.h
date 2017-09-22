#ifndef LSYS_H
#define LSYS_H

#include <vector>
#include <string>
#include <stack>
#include <unordered_map>
#include <time.h>
#include <linecalc.h>
#include <circlecalc.h>

#define PI 3.14159265

using namespace std;

class LSys
{
public:

	// Array of string encodings
	// (each string in the array is for a certain iteration)
	string encoding[5];

	// Position of the drawing turtle
	pair<float, float> pos;

	// Orientation angle of the drawing turtle (w.r.t. +X axis)
	float angle;

	// Angle change that occurs with each + or - sign
	float delta;

	// Distance the turtle covers in one stride
	float dist;

	// Stack to store position and orientation of
	// the turtle when entering a branch segment
	stack<pair<pair<float, float>, float> > stk;

	// Hash-map that maps characters to their rewrite strings
	unordered_map<char, string> rewriteRule;

	// ***********************************************************
	// Array of vectors of float vectors to store vertices
	// at each of the 5 iterations [0 - 5]:
	// - The first vector in each element of the array stores
	//   position vertices of the green colored points of the tree
	// - The second vector in each element of the array stores
	//   position vertices of the trunk of the tree
	// - The third vector in each element of the array stores
	//   position vertices of the branches of the tree
	// - The fourth vector in each element of the array stores
	//   positions of the orange colored points of the tree
	// - The fifth vector in each element of the array stores
	//   positions of the red colored points of the tree
	// ***********************************************************
	vector<vector<float> > v[5];

	// Branch depth during vertex calculations
	int depth = 0;

	// Constructor definition
	LSys(unsigned int treeType)
	{
		if (treeType == 1)
		{
			encoding[0] = "KT";
			dist = 25.0f;
			delta = 25.0f;
			rewriteRule['T'] = "KT[++TTR][+T][-TT][-T++TR][--TTR]R";
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
			// String to store expanded form of current iteration
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
			// Assign expansion to next iteration
			encoding[e + 1] = rewrite;
		}
	}

	// Utility function to calculate next position according
	// to global position and angle variables
	pair<float, float> nextPos()
	{
		pair<float, float> npos;
		npos.first = pos.first + dist * (float)cos(angle * PI / 180);
		npos.second = pos.second + dist * (float)sin(angle * PI / 180);
		return npos;
	}

	// Calculate and assign vertices of all tree parts
	// to the vector array in accordance with the expanded
	// strings for all 5 iterations of the L-system string
	void setVertices()
	{
		for (int e = 0; e < 5; e++)
		{
			v[e].resize(5);
			for (size_t i = 0; i < encoding[e].length(); i++)
			{
				if (encoding[e][i] == 'T' || encoding[e][i] == 'K')
				{
					// Calculate next position
					pair<float, float> npos = nextPos();
					npos = {round(npos.first), round(npos.second)};

					int y = max(3 - depth, 0);
					for (int x = -y; x <= y; x++)
					{
						// Calculate and assign points of a line from pos to npos
						LineCalc Twig({pos.first + round(x * sin(angle)), pos.second + round(x * cos(angle))}, {npos.first + round(x * sin(angle)), npos.second + round(x * cos(angle))});
						vector<float> points = Twig.getVertexVector();
						for (size_t j = 0; j < points.size(); j += 3)
						{
							if (depth == 0)
							{
								// We're at the trunk
								v[e][1].push_back(points[j]);
								v[e][1].push_back(points[j + 1]);
								v[e][1].push_back(points[j + 2]);
							}
							else if (depth == 1)
							{
								// We're at a branch
								v[e][2].push_back(points[j]);
								v[e][2].push_back(points[j + 1]);
								v[e][2].push_back(points[j + 2]);
							}
							else
							{
								// We're at a stalk or leaf
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
					// If we're at maximum depth, there is a 70% chance of a flower growing
					// If we're at (max depth - 1), there is a 20% chance of a flower growing
					int luck = rand() % 100;
					if ((luck > 30 && depth == e) || (luck > 80 && depth == e - 1))
					{
						// x determines whether the flower will be red or orange
						int x = 3 + rand() % 2;
						// Assign a random radius between 2 and 5 to the flower
						float flowerRadius = (float)(rand() % 4) + 2;
						// Draw circles with radius 1 to flowerRadius
						// to make an approximately filled circle
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
			// Reset position to origin for new iteration
			pos = {0.0f, 0.0f};
		}
	}
};
#endif