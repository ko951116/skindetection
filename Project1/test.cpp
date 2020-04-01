#include <opencv2\core\core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>

using namespace cv;
using namespace std;

#define DEFAULT_CAM 0

static char* INPUT_WINDOW = "Input Window";
static char* RESULT_WINDOW = "Result Window";

int getMax(int* arr, int size);

int main()
{ //Face Detection
	VideoCapture capture(DEFAULT_CAM);
	Mat frame;
	Mat skinMat;
	Mat result;
	vector<Mat> channels;

	int nr = result.rows;
	int nc = result.cols;

	namedWindow(INPUT_WINDOW, CV_WINDOW_AUTOSIZE);
	namedWindow(RESULT_WINDOW, CV_WINDOW_AUTOSIZE);

	while (1) {
		capture.read(frame);

		cvtColor(frame, skinMat, CV_BGR2YCrCb);

		inRange(skinMat, Scalar(0, 133, 77), Scalar(255, 173, 127), result);

		nr = result.rows;
		nc = result.cols;

		int countX[480] = { 0 };
		int countY[640] = { 0 };

		int maxX, maxY;

		for (int i = 0; i < nr; i++)
		{
			for (int j = 0; j < nc; j++)
			{
				if (result.at<uchar>(i, j)) {
					countX[i]++;
					countY[j]++;
				}
			}
		}

		//printf("%d\n", countX[200]);
		maxX = getMax(countX, 480);
		maxY = getMax(countY, 640);

		int avgX = maxX / 2;
		int avgY = maxY / 2;
		int x1, x2, y1, y2;
		int indexX;
		int indexY;

		for (int i = 0; i < 480; i++)
		{
			if (countX[i] == maxX)
				indexX = i;

			if ((countX[i] == avgX) && (i < indexX))
				x1 = i;
			if ((countX[i] == avgX) && (i > indexX))
				x2 = i;
		}

		//printf("%d, %d, %d\n\n", indexX, x1, x2);
		for (int j = 0; j < 640; j++)
		{
			if (countY[j] == maxY)
				indexY = j;

			if ((countY[j] == avgY) && (j < indexY))
				y1 = j;
			if ((countY[j] == avgY) && (j > indexY))
				y2 = j;
		}
		//printf("%d, %d, %d\n", indexY, y1, y2);
		//printf("%d, %d\n", maxX, x1);

		//printf("%d, %d\n", maxX, maxY);

		Rect rect(Point(y1, x1), Point(y2, x2));
		rectangle(frame, rect, Scalar(0, 255, 0), 1, 8, 0);

		imshow(INPUT_WINDOW, frame);
		imshow(RESULT_WINDOW, result);

		if (waitKey(10) > 0)
			break;
	}

	cvDestroyAllWindows();
	capture.release();
	return 0;
}

int getMax(int* arr, int size) {
	int max = arr[0];

	for (int i = 1; i < size; i++) {
		if (max < arr[i])
			max = arr[i];
	}
	return max;
}