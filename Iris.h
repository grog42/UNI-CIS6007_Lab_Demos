#pragma once

#include <iostream>
#include <string.h>

using namespace std;

/*
* A Class for storing information about Iris flowers
*/
class Iris {

private:

	float sepalLength;
	float sepalWidth;
	float petalLength;
	float petalWidth;
	string species;

public:

	//Constructs a iris oject from a set of inputs
	Iris(float _sepalLength, float _sepalWidth, float _petalLength, float _petalWidth, string _species) : sepalLength(_sepalLength), 
		sepalWidth(_sepalWidth), petalLength(_petalLength), petalWidth(_petalWidth), species(_species) {}

	//Constructs a Iris object which is a copy of anthoer
	Iris(const Iris& i) {

		sepalLength = i.sepalLength;
		sepalWidth = i.sepalWidth;
		petalLength = i.petalLength;
		petalWidth = i.petalWidth;
		species = i.species;
	}

	//Constructs a Iris object which is a copy of anthoer
	Iris& operator=(const Iris& i) {
		sepalLength = i.sepalLength;
		sepalWidth = i.sepalWidth;
		petalLength = i.petalLength;
		petalWidth = i.petalWidth;
		species = i.species;
	}

	/*
	* Comapres the values of antoher iris object
	* Returns true if all values are the same
	*/
	bool operator==(const Iris& i) {
		if (sepalLength == i.sepalLength && sepalWidth == i.sepalWidth &&
			petalLength == i.petalLength && petalWidth == i.petalWidth &&
			species == species) return true;

		return false;
	}

	/*
	* Compares Sepal length of inputs
	* return true if iris "b" is larger than "a"
	*/
	static bool compSepalLength(const Iris& a, const Iris& b) {
		return a.sepalLength < b.sepalLength;
	}

	/*
	* Compares Sepal Width of inputs
	* return true if iris "b" is larger than "a"
	*/
	static bool compSepalWidth(const Iris& a, const Iris& b) {
		return a.sepalWidth < b.sepalWidth;
	}

	/*
	* Compares Petal length of inputs
	* return true if iris "b" is larger than "a"
	*/
	static bool compPetalLength(const Iris& a, const Iris& b) {
		return a.petalLength < b.petalLength;
	}

	/*
	* Compares Petal Width of inputs
	* return true if iris "b" is larger than "a"
	*/
	static bool compPetalWidth(const Iris& a, const Iris& b) {
		return a.petalWidth < b.petalWidth;
	}
	
	/*
	* Writes the values of the object to a stream to be outputted by the console
	*/
	friend ostream& operator<<(ostream& out, const Iris& i) {

		out << "Iris:{sepalLength: " << i.sepalLength << ", sepalWidth: " << i.sepalWidth << ", petalLength: " << i.petalLength << ", petalWidth: " << i.petalWidth << ", species: " << i.species << "}" << endl;
		return out;
	}
};