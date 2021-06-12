#include "./randomAdress.h"
#include <bits/stdc++.h>
#include <cstdlib>
#include <ctime>
#include <string>
using namespace std;

std::string randomAdress()
{
	std::vector <int> values = { 1,2,3,4,5,6,7,8,9 };
	std::vector <string> alp = { "a","b","c","d","e","f" };
	std::vector <int> result;
	std::vector <string> results;
	std::string res;
	srand(time(NULL));
	for (int i = 0; i < 6; i++)
	{
		int random = rand() % (values.size() - 1);
		result.push_back(values[random]);
	}
	for (int l = 0; l < 6; l++)
	{
		int randoms = rand() % (alp.size() - 1);
		results.push_back(alp[randoms]);
	}
	for (int i = 0; i < 1; i++)
	{
		for (int k = 0; k <= 3; k++)
		{
			res += std::to_string(result[k]);
		}
		for (int j = 0; j <= 3; j++)
		{
			res += results[j];
		}
		for (int k = 3; k < 6; k++)
		{
			res += std::to_string(result[k]);
		}
		for (int j = 3; j < 6; j++)
		{
			res += results[j];
		}
	}
	return res;
}
