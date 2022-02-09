#pragma once
#include <string>
#include <ctime>
#include <iostream>

using namespace std;

/*Structure used to store data related to light or temperature readings*/
struct Reading
{
public:

	//Variables have been set to constant so that they can be accessed public without risk of being changed.
	const string type;
	const double value;
	const time_t timestamp;

	Reading(string _type, double _value, time_t _timestamp) : type(_type), value(_value), timestamp(_timestamp) {}

	/* << operator used for outputing the content of the reading to the console*/
	friend ostream& operator<<(ostream& out, const Reading& r) {
		out << "Reading:{type=" << r.type << ", value=" << r.value << ", timestamp=" << r.timestamp << "}" << endl;
		return out;
	}
};

