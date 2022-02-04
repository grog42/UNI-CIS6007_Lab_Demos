#pragma once
#include <iostream>
#include <random>

using namespace std;

/*A template class used to generate random float values*/
class RandomFloatGenerator{
private:
	default_random_engine re;
	uniform_real_distribution<float> dist;

public:

	/*Constructor taking paramaters to set the range in which values will be generated*/
	RandomFloatGenerator(float low, float high) :
		dist{ low, high } {}

	/*This function generates and returns the next random value*/
	float operator()() {
		return (float)dist(re);
	}
};

