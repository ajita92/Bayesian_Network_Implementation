
// Question 1: a part)    
// Input : reading value of N and K from command line argument
// Output : Bayesian network file
// run : g++ -std=c++11 Q1a.cpp
//      ./a.out Nval Kval > network.txt
 
#include <iostream>
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
	vector <long> parflag;
	vector <long> childflag;

	bool givenNode;
	bool reachable;

public:

	void updateChildren(vector <long> children)
	{
		this->children = children;
	}

	void updateParents(long n)
	{
		this->parents.push_back(n);
	}

	void updateTrail(vector <long> trail)
	{
		this->trail.clear();
		this->trail = trail;
	}

	void insertTrail(long n)
	{
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
	void clearTrail()
	{
		this->trail.clear();
	}

};

set <long> getChildren(long childCount, long begin, long end) 
{
	long k;
	set <long> s;
	vector <long> sample;

	for (long b = begin; b <= end; b++) 
	{
		sample.push_back(b);
	}

	while(s.size() != childCount)
	{
		k = rand() % sample.size();
		s.insert(sample[k]);
	}
	return s;
}

int main(int argc, char* argv[]) {

	long N = atoi(argv[1]);
	long K = atoi(argv[2]);

	Node nodes[N + 1];
	cout << N << endl;

	set <long> connectedId;

	for (long n = 1; n <= N; n++)
	{
		cout << n << " [";
		vector<long> children;
		connectedId.insert(n);

		long childCount = min(rand() % (K + 1), N - n);

		set <long> child = getChildren(childCount, n + 1, N);

		for (auto c : child)
			children.push_back(c);

		
		nodes[n].updateChildren(children);
		for (long c = 0; c < children.size(); c++) 
		{
			cout << children[c];
			nodes[c].updateParents(n);
			connectedId.insert(children[c]);
			if (c != (children.size() - 1))
				cout << ",";
		}
		cout << "]" << endl;
	}
	return 0;
}
