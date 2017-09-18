#ifndef LSYS_H
#define LSYS_H

#include <string>

using namespace std;

class LSys
{
public:
	unsigned int treeType;

	LSys(unsigned int treeType)
	{
		this -> treeType = treeType;
	}

	string expand1(string exp)
	{
		return exp + "F";
	}

	string expand2(string exp)
	{
		return exp + "F";
	}

	string expand3(string exp)
	{
		return exp + "F";
	}

	string expand(string exp)
	{
		if (treeType == 1)
		{
			return expand1(exp);
		}
		if (treeType == 2)
		{
			return expand2(exp);
		}
		if (treeType == 3)
		{
			return expand3(exp);
		}
	}
};
#endif