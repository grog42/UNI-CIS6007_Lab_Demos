
#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <opencv2/opencv.hpp>
#include <execution>
#include <future>
#include <chrono>

using namespace std;
using namespace cv;
using namespace std::chrono;
namespace fs = filesystem;

//A serial implmentation of the Peak Signal to noise ratio function
double PeakSigToNoiseRatio_Ser(const Mat& img_a, const Mat& img_b) {

	//Preload shared resources to minimise execution time
	const int nChannel = img_a.channels();
	const uchar* data_a = img_a.data;
	const uchar* data_b = img_b.data;
	const int imageArea = img_a.rows * img_a.cols;

	double sum = 0;

	for (int i = 0; i < imageArea; i++) {

		//i maps to pixles of size nChannel
		int iPtr = i * nChannel;
		uchar b = data_a[iPtr] - data_b[iPtr];
		uchar g = data_a[iPtr + 1] - data_b[iPtr + 1];
		uchar r = data_a[iPtr + 2] - data_b[iPtr + 2];

		sum += pow(r + g + b / 3, 2);
	}

	return sqrt(sum) / imageArea;
}

//A parallel implmentation of the Peak Signal to noise ratio function
double PeakSigToNoiseRatio_Par(const Mat& img_a, const Mat& img_b) {

	//Preload shared resources to minimise execution time
	int nChannel = img_a.channels();
	const uchar* data_a = img_a.data;
	const uchar* data_b = img_b.data;
	const int imageArea = img_a.rows * img_a.cols;

	function<double (int, int, int)> Partition = [&](int start, int end, int depth) {

		if (depth > 8) {

			double sum = 0;

			//i maps to pixles of size nChannel
			for (int i = start; i < end; i++) {
				int iPtr = i * nChannel;
				uchar b = data_a[iPtr] - data_b[iPtr];
				uchar g = data_a[iPtr + 1] - data_b[iPtr + 1];
				uchar r = data_a[iPtr + 2] - data_b[iPtr + 2];

				sum += pow(r + g + b / 3, 2);
			}

			return sum;
		}
		else
		{
			int mid = (start + end) / 2;

			auto p1 = async(Partition, start, mid, ++depth);
			auto p2 = async(Partition, mid, end, ++depth);

			return p1.get() + p2.get();
		}
	};
	
	double sum = Partition(0, imageArea, 0);

	return sqrt(sum) / imageArea;
}

int main(int argc, char** argv)
{
	string baseImgPath = argv[1];
	string commImgPath = argv[2];

	//Checks performed to ensure both images exist 
	if (!fs::exists(baseImgPath)) {
		cout << "Base image not found" << endl;
		return 0;
	}

	if (!fs::exists(commImgPath)) {
		cout << "Compressed image not found" << endl;
		return 0;
	}

	Mat img_a(imread(baseImgPath));
	Mat img_b(imread(commImgPath));

	//Images must have the same dimentions to be compared
	if (img_a.rows != img_b.rows || img_a.cols != img_b.cols) {
		cout << "Images are not comparable" << endl;
		return 0;
	}

	//Calculate PSNR ratio serialy and log duration
	auto start_ser = high_resolution_clock::now();
	double ratio_ser = PeakSigToNoiseRatio_Ser(img_a, img_b);
	int duration_ser = duration_cast<microseconds>(high_resolution_clock::now() - start_ser).count();

	//Calculate PSNR ratio in parallel and log duration
	auto start_par = high_resolution_clock::now();
	double ratio_par = PeakSigToNoiseRatio_Par(img_a, img_b);
	int duration_par = duration_cast<microseconds>(high_resolution_clock::now() - start_par).count();

	cout << "Ratio is: " << ratio_ser << endl;
	cout << "Serial execution time: " << duration_ser << endl;
	cout << "Parallel execution time: " << duration_par << endl;

	return 0;
}
