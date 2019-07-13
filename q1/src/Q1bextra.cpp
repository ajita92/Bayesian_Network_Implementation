
// Question 1: a part) 
// Input File : network.txt and number of queries as command line argument
// run : g++ -std=c++11 Q1bextra.cpp
// ./a.out querynum < network.cpp
// output file : o.txt

#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <stdio.h> 
#include <algorithm>
#include <set>

using namespace std;

class Node 
{

	vector <long> parents;   // id's corresponding to the parents are stored
	vector <long> children; //  id's corresponding to the children are stored
	vector <long> trail;
	vector <bool> parflag;
	vector <bool> childflag;

	bool givenNode;
	bool reachable;

public:

	void updateChildren(vector <long> children)
	{
		this->children = children;
		this->childflag.clear();

		for (long i = 0; i < children.size(); i++)
		{
			this->childflag.push_back(false);
		}
	}

	void updateParents(long n)
	{
		this->parents.push_back(n);
		this->parflag.push_back(false);
	}

	void updateTrail(vector <long> trail)
	{
		this->trail.clear();
		this->trail = trail;
	}

	void insertTrail(long n)
	{
		long remove = 0;
		for (long t = 0; t < trail.size(); t++)
		{
			if (trail[t] == n)
				remove = trail.size() - t;
		}
		for (long i = 0; i < remove; i++)
		{
			trail.pop_back();
		}
		this->trail.push_back(n);
	}

	vector <long> getParent()
	{
		return this->parents;
	}

	vector <long> getChildren()
	{
		return this->children;
	}

	vector <long> getTrail()
	{
		return this->trail;
	}

	void printdetails()
	{
		cout << "Parents are: \n";
		for (long i = 0; i < parents.size(); i++)
		{
			cout << parents[i] << " ";
		}

		cout << "\nChildren are: \n";
		for (long i = 0; i < children.size(); i++)
		{
			cout << children[i] << " ";
		}

	}

	void setGivennode(bool givenNode)
	{
		this->givenNode = givenNode;
	}

	bool getGivennode()
	{
		return this->givenNode;
	}

	void setReached(bool reachable)
	{
		this->reachable = reachable;
	}

	bool getReached()
	{
		return this->reachable;
	}

	void setparFlag(bool reachable, long pos)
	{
		this->parflag[pos] = reachable;
	}

	bool getparFlag(long pos)
	{
		return this->parflag[pos];
	}

	void setchildFlag(bool reachable, long pos)
	{
		this->childflag[pos] = reachable;
	}

	bool getchildFlag(long pos)
	{
		return this->childflag[pos];
	}

	void clearTrail()
	{
		this->trail.clear();
		for (auto&& f : parflag)
		{
			f = false;
		}
		for (auto&& f : childflag)
		{
			f = false;
		}
	}

};

void splitString(string str1, char separator, vector <long> &children)
{
	string temp = "";
	string str = str1.substr(2, str1.size() - 3);

	for (long s = 0; s < str.size(); s++)
	{
		if (str[s] == ',')
		{
			children.push_back(atoi(temp.c_str()));
			temp = "";
		}
		else
		{
			temp += str[s];
		}
	}
	if (!temp.empty())
		children.push_back(atoi(temp.c_str()));

}

set <long> getObservedNodes(long count, vector <long> samples) 
{
	long k;
	set <long> s;

	while(s.size() != count)
	{
		k = rand() % samples.size();
		s.insert(samples[k]);
	}
	return s;
}

void printVector(vector <long> Znodes)
{
	cout << "[";
	for (long c = 0; c < Znodes.size(); c++) 
	{
		cout << Znodes[c];
		if (c != (Znodes.size() - 1))
			cout << ",";
	}
	cout << "]" << endl;
}

void checkActiveTrail(long Xi, Node nodes[], bool goUp, bool goDown, vector <long> trailVal)
{
	bool b1, b2;
	nodes[Xi].setReached(true);

	if (!trailVal.empty())
		nodes[Xi].updateTrail(trailVal);

	nodes[Xi].insertTrail(Xi);

	vector <long> child = nodes[Xi].getChildren();
	vector <long> pars = nodes[Xi].getParent();

	if (goUp)
	{
		for (long l = 0; l < pars.size(); l++)
		{

			long p = pars[l];
			b1 = nodes[Xi].getGivennode();
			b2 = nodes[p].getGivennode();

			if (nodes[Xi].getparFlag(l))
				continue;
			nodes[Xi].setparFlag(true, l);
			

			if ((b1 && !b2) || (!b1 && !b2))
				checkActiveTrail(p, nodes, true, true, nodes[Xi].getTrail());

			else if ((b1 && b2) || (!b1 && b2))
				checkActiveTrail(p, nodes, false, false, nodes[Xi].getTrail());
		}
	}

	if (goDown)
	{
		for (long l = 0; l < child.size(); l++)
		{
			if (nodes[Xi].getchildFlag(l))
				continue;
			nodes[Xi].setchildFlag(true, l);
			long c = child[l];

			if (nodes[c].getGivennode())
				checkActiveTrail(c, nodes, true, false, nodes[Xi].getTrail());
			else
				checkActiveTrail(c, nodes, false, true, nodes[Xi].getTrail());
		}
	}
}

void ballReached(Node nodes[], long N)
{
	for (long i = 1; i <= N; i++)
	{
		cout << i <<"th"<<nodes[i].getReached() <<"  ";
	}
}

void processQuery(long Q, long N, Node nodes[], ifstream& file2)
{
	long Xi, Xj;
	string str;
	vector <long> samples, trail, Znodes;

	for (long q = 1; q <= Q; q++)
	{
		file2 >> Xi >> Xj;
		for (long n = 1; n <= N; n++)
		{
			samples.push_back(n);
			nodes[n].setGivennode(false);
			nodes[n].setReached(false);
			nodes[n].clearTrail();
		}
		getline(file2, str);
		splitString(str, ',', Znodes);
		for (auto z : Znodes)
		{
			nodes[z].setGivennode(true);
		}
		cout << Xi << " " << Xj << " " << str << endl;
		checkActiveTrail(Xi, nodes, true, true, trail);

		if (nodes[Xj].getTrail().empty())
		{
			cout << "yes" << endl;
		}
		else
		{	
			cout << "no ";
			printVector(nodes[Xj].getTrail());
		}
		samples.clear();
		Znodes.clear();
	}
}

int main(int argc, char* argv[])
{
	long N, parentNode, Q;
	string str;

	ifstream file1, file2;
	file1.open(argv[1]);

	file1 >> N;

	Node nodes[N + 1];

	
	for (long n = 1; n <= N; n++)
	{
		file1 >> parentNode;
		getline(file1, str);
		vector <long> children;
		
		splitString(str, ',', children);
		nodes[n].updateChildren(children);

		for (auto c : nodes[n].getChildren())
		{
			nodes[c].updateParents(n);
		}
	}

	file2.open(argv[2]);
	file2 >> Q;
	processQuery(Q, N, nodes, file2);

	file1.close();
	file2.close();

	return 0;
}
