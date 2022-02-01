#pragma once
#include <iostream>
#include <random>

using namespace std;

template<typename T>
class RandomValGenerator{
private:
	default_random_engine re;
	uniform_real_distribution<double> dist;

public:

	RandomValGenerator(T low, T high) :
		dist{ low, high } {}

	T operator()() {

		return (T)dist(re);
	}
};

