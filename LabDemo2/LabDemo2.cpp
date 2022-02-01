
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <queue>
#include <mutex>

#include "RandomValGenerator.h"
#include "Reading.h"

using namespace std;
using namespace std::chrono;

Reading PopReading(mutex& mu, queue<Reading>& readings) {
	unique_lock<mutex> lock(mu);
	Reading r = readings.front();
	readings.pop();

	return r;
}

void PushReading(Reading r, mutex& mu, queue<Reading>& readings) {
	unique_lock<mutex> lock(mu);
	readings.push(r);
}

void ReadTempValuses(int delay_ms, mutex& mu, queue<Reading>& readings) {

	RandomValGenerator<float> randVal(0, 100);

	while (true)
	{
		this_thread::sleep_for(milliseconds(delay_ms));

		float val = randVal();
		auto t_now = system_clock::now();
		PushReading(Reading("Temp", val, system_clock::to_time_t(t_now)), mu, readings);
	}
}

void ReadLightValues(int delay_ms, mutex& mu, queue<Reading>& readings) {

	RandomValGenerator<float> randVal(0, 100);

	while (true)
	{
		this_thread::sleep_for(milliseconds(delay_ms));

		float val = randVal();
		auto t_now = system_clock::now();
		PushReading(Reading("Light", val, system_clock::to_time_t(t_now)), mu, readings);
	}
}

int main()
{
	queue<Reading> readings{};
	vector<Reading> handledReadings{};

	int readingDelay_ms;
	int tempOutput = 0;
	int lightOutput = 0;

	cout << "Input reading delay in ms:";
	cin >> readingDelay_ms;

	mutex mu;

	thread tempReader(ReadTempValuses, readingDelay_ms, ref(mu), ref(readings));
	thread lightReader(ReadLightValues, readingDelay_ms, ref(mu), ref(readings));

	while (true)
	{
		while (readings.size() > 0) {
			const Reading r = PopReading(mu, readings);

			if (r.type == "Temp") {
				if (r.value > 37) {
					tempOutput--;
				}
				else
				{
					tempOutput++;
				}
			}
			else if(r.type == "Light")
			{
				if (r.value > 100) {
					tempOutput--;
				}
				else
				{
					tempOutput++;
				}
			}
			else
			{
				cout << "Reading type un-recognised" << endl;
			}

			cout << r;
			handledReadings.push_back(r);
		}
	}

	tempReader.join();
	lightReader.join();
}
