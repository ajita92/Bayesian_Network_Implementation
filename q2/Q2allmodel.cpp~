
// Part A, B and C combined................
// g++ -std=c++11 Qa.cpp 
// ./a.out  small/large < inputfile

#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>
#include <math.h>
#include <map>
using namespace std;

double likelihood1 = 0, likelihood2 = 0, likelihood3 = 0;
long word1 = 0, word2 = 0, word3 = 0, char1 = 0, char2 = 0, char3 = 0, totalchar = 0, twords = 0, cwords = 0;
char charaters[10] = {'e', 't', 'a', 'o', 'i', 'n', 's', 'h', 'r', 'd'};
double ocr[1000][10], transition[10][10];
map <char, long> char2int;
map <long, char> int2char;
vector <string> correctWords;

void createPD(long idCount, vector <long> &row, vector < pair< vector <long>, vector <double> > >&PD, 
	vector <long> &imageIds, double ocrProb, double transProb, double combProb)
{
	if (idCount != 0)
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
			createPD(idCount - 1, row, PD, imageIds, ocrProb + o, transProb + t, combProb + c);
			row.pop_back();
		}
	}
	else 
	{	vector <double> tmp;
		tmp.push_back(exp(ocrProb));
		tmp.push_back(exp(transProb));
		tmp.push_back(exp(combProb));
		PD.push_back(make_pair(row, tmp));
		return;
	}
}

void printAccuracy()
{
	long total = correctWords.size();
	cout << endl;
	cout << "Word Wise Accuracy" << endl;
	cout << "OCR Model " << (double)word1/total * 100 << endl;
	cout << "Transition Model " << (double)word2/total * 100 << endl;
	cout << "Combination Model " << (double)word3/total * 100 << endl;
	cout << endl;
	cout << "Character wise Accuracy" << endl;
	cout << "OCR Model " << (double)char1/totalchar * 100 << endl;
	cout << "Transition Model " << (double)char2/totalchar * 100 << endl;
	cout << "Combination Model " << (double)char3/totalchar * 100 << endl;
	cout << endl;
	cout << "Likelihood " << endl;
	cout << "OCR Model " << (double)likelihood1/total << endl;
	cout << "Transition Model " << (double)likelihood2/total << endl;
	cout << "Combination Model " << (double)likelihood3/total << endl;
	cout << endl;
}

bool moresamechar(string str2, string str1, string word)
{
	long s1 = 0, s2 = 0;
	for (long w = 0; w < word.size(); w++)
	{
		if (str1[w] == word[w]) s1++;
		if (str2[w] == word[w]) s2++;
	}
	if (s2 > s1) return true;
	return false;
}

void printwords(string word, string str1, string str2, string str3)
{
	if (str1 != word)
	{	
		bool f = (str2 == word)&& twords <= 5;
		bool g = ((str3 == word && moresamechar(str2, str1, word)) && cwords <= 5) && (str3 != str2);
		if ( f || g)
		{
			cout << "correct = " << word << " ";
			if (f)
			{
				cout << "ocr = " << str1 << "  " << "transition = " << str2 << endl; 
				twords++;
			}
			else if (g)
			{
				cout << "ocr = " << str1 << " transition = " << str2 << " combinded = " << str3 << endl;
				cwords++;
			}
		}
	}
}

void Modelling(vector <long> imageIds, string word)
{
	vector <long> row; 
	long len = imageIds.size();
	double Z1 = 0, Z2 = 0, Z3 = 0;
	string str1 = "", str2 = "", str3 = "";
	vector < pair< vector <long>, vector <double> > >PD;
	long maxIndex1 = 0, index = 0, maxIndex2 = 0, maxIndex3 = 0;

	createPD(len, row, PD, imageIds, 0, 0, 0);

	for (long w = 0; w < word.size(); w++)
		index += (char2int[word[w]] * pow(10, word.size() - 1 - w));

	for (long i = 0; i < PD.size(); i++)
	{
		Z1 += PD[i].second[0];
		Z2 += PD[i].second[1];
		Z3 += PD[i].second[2];
	}
	for (long i = 0; i < PD.size(); i++)
	{
		PD[i].second[0] /= Z1;
		PD[i].second[1] /= Z2;
		PD[i].second[2] /= Z3;
		if (PD[maxIndex1].second[0] < PD[i].second[0])
			maxIndex1 = i;
		if (PD[maxIndex2].second[1] < PD[i].second[1])
			maxIndex2 = i;
		if (PD[maxIndex3].second[2] < PD[i].second[2])
			maxIndex3 = i;
	}

	likelihood1 += log(PD[index].second[0]);
	likelihood2 += log(PD[index].second[1]);
	likelihood3 += log(PD[index].second[2]);

	for (auto m : PD[maxIndex1].first)
		str1 += int2char[m];

	for (auto m : PD[maxIndex2].first)
		str2 += int2char[m];

	for (auto m : PD[maxIndex3].first)
		str3 += int2char[m];
 
 	if (str1 == word) word1++;
 	if (str2 == word) word2++;
 	if (str3 == word) word3++;

 	for (long w = 0; w < word.size(); w++)
 	{
 		if (word[w] == str1[w])char1++;
 		if (word[w] == str2[w])char2++;
 		if (word[w] == str3[w])char3++;
 	}
 	totalchar += len;
 	if (twords <= 5 || cwords <= 5)
 		printwords(word, str1, str2, str3);
}

int main(int argc, char const *argv[])
{
	char ch, ch1;
	string str;
	long n, index = 0;
	double prob;
    long input = atoi(argv[1]);

	ifstream ocrDat, transDat, wordsDat;
	ocrDat.open("./OCRdataset/potentials/ocr.dat");
	transDat.open("./OCRdataset/potentials/trans.dat");

	if (input == 1)
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

		Modelling(imageIds, correctWords[index]);
		index += 1;
	}
	printAccuracy();
	return 0;
}
