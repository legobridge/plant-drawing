#ifndef LINECALC_H
#define LINECALC_H

#include <algorithm>
#include <cmath>
#include <vector>
#include <utility>

using namespace std;

class LineCalc
{
public:

	pair<float, float> a, b;
	vector<float> v;
    size_t n;
    int transformCase;

	LineCalc(pair<float, float> a, pair<float, float> b)
	{
		this -> a = a;
		this -> b = b;
	}

	// Normalize coordinate magnitudes to values between -0.8 to 0.8
	void normalizeCoordinates()
	{
		float mx = max(max(abs(a.first), abs(a.second)), max(abs(b.first), abs(b.second)));
		for (size_t i = 0; i < n; i++)
		{
			v[i] = (0.8f * v[i]) / mx;
		}
	}

	// Find which octant the slope vector belongs to
    void findCase()
    {
        float dx = b.first - a.first;
        float dy = b.second - a.second;
        
        if (dy < 0)
        {
            dx = -dx;
            dy = -dy;
        }

        if (dx > 0)
        {
            if (dx > dy)
            {
				// Angle with positive x-axis belongs to [0, 45)°
                transformCase = 1;
            }
            else 
            {
				// Angle with positive x-axis belongs to [45, 90)°
                transformCase = 2;
            }
        }
        else
        {
            if (-dx < dy)
            {
				// Angle with positive x-axis belongs to [90, 135)°
                transformCase = 3;
            }
            else 
            {
				// Angle with positive x-axis belongs to [135, 180)°
                transformCase = 4;
            }
        }
    }

	// Transform the Endpoints of the line segment
	// in accordance with the standard algorithm
    void transformEndpoints()
    {
        pair<float, float> p1 = a;
        pair<float, float> p2 = b;

        switch (transformCase)
        {
            case 2:
                swap (a,b);
                break;
            case 3:
                a.first = p1.second;
                a.second = -p1.first;
                b.first = p2.second;
                b.second = -p2.first;                
                break;
            case 4:
                a.first = -a.first;
                a.second = a.second;
                b.first = -b.first;
                b.second = b.second;                
                break;
        }
		if (a.first > b.first)
		{
			swap(a, b);
		}
    }

	// Invert the transformation for all points on the segment
    pair<float, float> inverseTransformPoint(pair<float, float> p)
    {
        pair<float, float> p1 = p;

        switch (transformCase)
        {
            case 2:
                swap(p.first,p.second);
                break;
            case 3:
                p.first = -p1.second;
                p.second = p1.first;            
                break;
            case 4:
                p.first = -p.first;
                p.second = p.second;             
                break;
        }
        return p;
    }
    
	// Compute a vector of coordinates using Bresenham's Midpoint Algorithm
	void computeVertexVector()
	{
		v.push_back(a.first);
		v.push_back(a.second);
		v.push_back(0.0f);

		float x = a.first;
		float y = a.second;
	
		float dx = b.first - a.first;
		float dy = b.second - a.second;
	
		float d = 2 * dy - dx;
		float dE = 2 * dy;
		float dNE = 2 * (dy - dx);
	
        while (x < b.first) 
        {
            if (d <= 0)
            {
                d += dE;
            }
            else 
            {
				d += dNE;
				y++;
			}
			x++;
			v.push_back(x);
			v.push_back(y);
			v.push_back(0);
		}
		v.push_back(b.first);
		v.push_back(b.second);
		v.push_back(0.0f);
	}

	// Driver function to call other functions of the class
	// and return an array of coordinates to be drawn
	float* getVertexArray()
	{
        findCase();

        transformEndpoints();

        computeVertexVector();
        
        n = v.size();
        for (size_t i = 0; i < n; i += 3)
        {
            pair<float, float> t = inverseTransformPoint({v[i], v[i + 1]});
            v[i] = t.first;
            v[i + 1] = t.second;
        }
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