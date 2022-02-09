
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

//Global queue used by the diffrent threads when pushing and handling readings
queue<Reading> readings;

mutex mu;
condition_variable cond;

/*Function used by both producer threads to add readings to the queue*/
void PushReading(Reading r) {
	unique_lock<mutex> lock(mu);
	readings.push(r);
	cond.notify_one();
}

/*Thread used for "reading" (generating) temerature values to be pushed to the queue*/
void ReadTempValuses(int delay_ms) {

	RandomFloatGenerator randVal(32, 45);

	while (true)
	{
		this_thread::sleep_for(milliseconds(delay_ms));

		float val = randVal();
		auto t_now = system_clock::now();
		PushReading(Reading("Temp", val, system_clock::to_time_t(t_now)));
	}
}

/*Thread used for "reading" (generating) light values to be pushed to the queue*/
void ReadLightValues(int delay_ms) {

	RandomFloatGenerator randVal(50, 150);

	while (true)
	{
		this_thread::sleep_for(milliseconds(delay_ms));

		float val = randVal();
		auto t_now = system_clock::now();
		PushReading(Reading("Light", val, system_clock::to_time_t(t_now)));
	}
}

/*Thread used to process readings added to the queue*/
void HandleReadings() {

	//Varables used to simulate devices reacting to sensor readings
	int tempOutput = 0;
	int lightOutput = 0;

	//Stores reading which have already been processed
	vector<Reading> handledReadings{};

	while (true)
	{
		unique_lock<mutex> lock(mu);

		cond.wait(lock, []() {return !readings.empty();});

		const Reading r = readings.front();
		readings.pop();

		if (r.type == "Temp") {

			//Temp values are used in a feedback loop to keep the temperature around 37 degrees
			if (r.value > 37) {
				tempOutput--;
			}
			else
			{
				tempOutput++;
			}
		}
		else if (r.type == "Light")
		{
			//Light values are used in a feedback loop to keep the level around 100 lux
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

int main()
{
	int readingDelay_ms;

	//User uses the console to input the time delay between readings (milliseconds)
	cout << "Input reading delay in ms:";
	cin >> readingDelay_ms;

	thread tempReader(ReadTempValuses, readingDelay_ms);
	thread lightReader(ReadLightValues, readingDelay_ms);
	thread readingProcesser(HandleReadings);

	tempReader.join();
	lightReader.join();
	readingProcesser.join();
}
