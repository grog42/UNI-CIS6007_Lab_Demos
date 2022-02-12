
#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <opencv2/opencv.hpp>
#include <execution>
#include <future>

using namespace std;
using namespace cv;
namespace fs = filesystem;

double PeakSigToNoiseRatio_P(const Mat& img_a, const Mat& img_b) {

	int nChannel = img_a.channels();
	const uchar* data_a = img_a.data;
	const uchar* data_b = img_b.data;
	const int imageArea = img_a.rows * img_a.cols;

	function<double (int, int, int)> Partition = [&](int start, int end, int depth) {

		if (depth > 9) {

			double sum = 0;

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
			int mid = start + end / 2;

			auto p1 = Partition(start, mid, ++depth);
			auto p2 = Partition(mid, end, ++depth);

			return p1 + p2;
		}
	};
	
	double sum = Partition(0, imageArea, 0);

	return sqrt(sum) / imageArea;
}

int main(int argc, char** argv)
{
	string baseImgPath = argv[1];
	string commImgPath = argv[2];

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

	if (img_a.rows != img_b.rows || img_a.cols != img_b.cols) {
		cout << "Images are not comparable" << endl;
		return 0;
	}

	double ratio = PeakSigToNoiseRatio_P(img_a, img_b);

	cout << "Ratio is: " << ratio << endl;

	return 0;
}
