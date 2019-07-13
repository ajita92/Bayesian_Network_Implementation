
// Part A, B and C combined................
// g++ -std=c++11 Qa.cpp 
// ./a.out modelno small/large < inputfile

#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>
#include <math.h>
#include <map>
using namespace std;

char charaters[10] = {'e', 't', 'a', 'o', 'i', 'n', 's', 'h', 'r', 'd'};
double ocr[1000][10], transition[10][10];
map <char, long> char2int;
map <long, char> int2char;
vector <string> correctWords;

void createPD(long idCount, vector <long> &row, vector < pair< vector <long>, double> >&PD, 
	vector <long> &imageIds, long flag, double ocrProb, double transProb, double combProb)
{
	if(idCount != 0)
	{
		for (long i = 0; i < 10; i++)
		{
			double t = 0, o = 0, c = 0;
			o = ocr[imageIds[row.size()]][i];
			t = (row.size() == 0) ? o : transition[row[row.size()-1]][i] + o;
			c = t;

			for (long j = row.size() - 1; j >= 0; j--)
			{
				if (row[j] == i && imageIds[j] == imageIds[row.size()])
					c += log(5.0);
			}
			row.push_back(i);
			createPD(idCount - 1, row, PD, imageIds, flag, ocrProb + o, transProb + t, combProb + c);
			row.pop_back();
		}
	}
	else
	{
		double probability = 0;
		if (flag == 1)
			probability = ocrProb;
		else if (flag == 2)
			probability = transProb;
		else if (flag == 3)
			probability = combProb;

		PD.push_back(make_pair(row, exp(probability)));
		return;
	}
}

void Modelling(vector <long> imageIds, long input, string word)
{
	long maxIndex = 0, index = 0;
	double Z = 0;
	string str = "";
	long len = imageIds.size();
	vector <long> row; 
	vector < pair< vector <long>, double> >PD;

	createPD(len, row, PD, imageIds, input, 0, 0, 0);

	for (long i = 0; i < PD.size(); i++)
		Z += PD[i].second;

	for (long i = 0; i < PD.size(); i++)
	{
		PD[i].second /= Z;
		if (PD[maxIndex].second < PD[i].second)
			maxIndex = i;
	}
	for (long w = 0; w < word.size(); w++)
		index += (char2int[word[w]] * pow(10, word.size() - 1 - w));

	for (auto m : PD[maxIndex].first)
	{
		str += int2char[m];
	}
	cout << str << "," << PD[maxIndex].second << "," << word << "," << PD[index].second << endl; // << PD[maxIndex].second 
	return;
}
int main(int argc, char const *argv[])
{
	char ch, ch1;
	string str;
	long n, input, index = 0, ip;
	double prob;
	input = atoi(argv[1]);  // tells the model number
	ip = atoi(argv[2]);     // tells the type of file to be read small or large

	ifstream ocrDat, transDat, wordsDat;
	ocrDat.open("./OCRdataset/potentials/ocr.dat");
	transDat.open("./OCRdataset/potentials/trans.dat");

	if (ip == 1)
		wordsDat.open("./OCRdataset/data/small/words.dat");
	else
		wordsDat.open("./OCRdataset/data/large/allwords.dat");

	for (int i = 0; i < 10; i++)
	{
		int2char[i] = charaters[i];
		char2int[charaters[i]] = i;
	}
	for (long i = 0; i < 10000; i++)
	{
		ocrDat >> n >> ch >> prob;
		ocr[n][char2int[ch]] = log(prob);
	}
	for (long i = 0; i < 100; i++)
	{
		transDat >> ch >> ch1 >> prob;
		transition[char2int[ch]][char2int[ch1]] = log(prob);
	}
	while(getline(wordsDat, str))
		correctWords.push_back(str);

	while(getline(cin, str))
	{
		vector <long> imageIds;
		stringstream images(str);

		while(images >> n)
			imageIds.push_back(n);

		Modelling(imageIds, input, correctWords[index]);
		index += 1;
	}
	return 0;
}