#include "ViewProcess.h"

void ViewProcess::process(cv::Mat &img)
{
	cv::imshow("img", img);
	cv::waitKey(1);
}
