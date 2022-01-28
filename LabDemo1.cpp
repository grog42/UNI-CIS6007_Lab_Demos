// LabDemo1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

#include "Iris.h"

using namespace std;

/*
* Uses template to take a range of inputs to print to console.
* The object must have a defintion for "ostream& operator<<" to work
*/
template<class T>
void PrintToConsole(T t) {
    std::cout << t;
}

/*
*Reads data about Iris flowers from csv file and creates Iris objects to store values.
* The Iris instances which express the min and max values for each iris property are located and printed
*/
int main()
{
    ifstream stream("./IRIS.csv");
	string line;
	vector<Iris> flowers;

	cout << stream.is_open();

	//Pop column names from results
	getline(stream, line);

	while (getline(stream, line))
	{
		istringstream s(line);
		string field;

		int fieldCount = 0;

		vector<float> values(4);

		while (getline(s, field, ',')) {

			if (fieldCount < 4) {
				values[fieldCount] = stof(field);
				fieldCount++;
			}
			else
			{
				flowers.push_back(Iris(values[0], values[1], values[2], values[3], field));
				values.clear();
				fieldCount = 0;
				break;
			}
		}
	}

	auto maxSepalLength = max_element(flowers.begin(), flowers.end(), &Iris::compSepalLength);
	auto maxSepalWidth = max_element(flowers.begin(), flowers.end(), &Iris::compSepalWidth);
	auto maxPetalLength = max_element(flowers.begin(), flowers.end(), &Iris::compPetalLength);
	auto maxPetalWidth = max_element(flowers.begin(), flowers.end(), &Iris::compPetalWidth);

	auto minSepalLength = min_element(flowers.begin(), flowers.end(), &Iris::compSepalLength);
	auto minSepalWidth = min_element(flowers.begin(), flowers.end(), &Iris::compSepalWidth);
	auto minPetalLength = min_element(flowers.begin(), flowers.end(), &Iris::compPetalLength);
	auto minPetalWidth = min_element(flowers.begin(), flowers.end(), &Iris::compPetalWidth);

	PrintToConsole(*maxSepalLength);
	PrintToConsole(*maxSepalWidth);
	PrintToConsole(*maxPetalLength);
	PrintToConsole(*maxPetalWidth);

	PrintToConsole(*minSepalLength);
	PrintToConsole(*minSepalWidth);
	PrintToConsole(*minPetalLength);
	PrintToConsole(*minPetalWidth);
}