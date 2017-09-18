#ifndef CIRCLECALC_H
#define CIRCLECALC_H

#include <algorithm>
#include <cmath>
#include <vector>
#include <utility>

using namespace std;

class CircleCalc
{
public:

	pair<float, float> c;
	float r;
	vector<float> v;
	size_t n;

	CircleCalc(pair<float, float> c, float r)
	{
		this -> c = c;
		this -> r = r;
	}

	// Normalize coordinate magnitudes to values between -0.8 to 0.8
	void normalizeCoordinates()
	{
		float mx = max(abs(c.first) + r, abs(c.second) + r);
		for (size_t i = 0; i < n; i++)
		{
			v[i] = (0.8f * v[i]) / mx;
		}
	}

	// Translate the calculated coordinates to the original centre
	void translateCircle()
	{
		for (size_t i = 0; i < n; i += 3)
		{
			v[i] += c.first;
			v[i + 1] += c.second;
		}
	}

	// Add all symmetric points to the vertex vector
	void addPoint(float x, float y)
	{
		v.push_back(x);
		v.push_back(y);
		v.push_back(0.0f);

		v.push_back(-x);
		v.push_back(y);
		v.push_back(0.0f);

		v.push_back(x);
		v.push_back(-y);
		v.push_back(0.0f);

		v.push_back(-x);
		v.push_back(-y);
		v.push_back(0.0f);

		v.push_back(y);
		v.push_back(x);
		v.push_back(0.0f);

		v.push_back(-y);
		v.push_back(x);
		v.push_back(0.0f);

		v.push_back(y);
		v.push_back(-x);
		v.push_back(0.0f);

		v.push_back(-y);
		v.push_back(-x);
		v.push_back(0.0f);
	}

	// Compute a vector of coordinates the Midpoint Algorithm,
	// assuming the origin to be the centre
	void computeVertexVector()
	{
		float x = 0;
		float y = r;
		float d = 1 - r;
		float deltaE = 3;
		float deltaSE = -2 * r + 5;
		addPoint(x, y);

		while (y > x)
		{
			if (d < 0)
			{
				// Select E
				d += deltaE;
				deltaE += 2;
				deltaSE += 2;
			}
			else
			{
				// Select SE
				d += deltaSE;
				deltaE += 2;
				deltaSE += 4;
				y--;
			}
			x++;
			addPoint(x, y);
		}
	}

	// Driver function to call other functions of the class
	// and return an array of coordinates to be drawn
	float* getVertexArray()
	{
		computeVertexVector();
		n = v.size();
		translateCircle();
		normalizeCoordinates();
		float *a = new float[n];
		for (size_t i = 0; i < n; i++)
		{
			a[i] = v[i];
		}
		return a;
	}
};

#endif