#pragma once
#include <string>
#include <ctime>
#include <iostream>

using namespace std;

struct Reading
{
	string type;
	double value;
	time_t timestamp;
public:

	Reading(string _type, double _value, time_t _timestamp) {
		type = _type;
		value = _value;
		timestamp = _timestamp;
	}

	friend ostream& operator<<(ostream& out, const Reading& r) {
		out << "Reading:{type=" << r.type << ", value=" << r.value << ", timestamp=" << r.timestamp << "}" << endl;
		return out;
	}
};

