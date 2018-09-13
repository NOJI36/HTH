#include<vector>
#include <iostream>
#include <memory>

// USER Defined
#include "connector.h"
#include "VideoDecoder.h"
#include "ViewProcess.h"
#include "Yolo_origin.h"


// OPENCV
#include"opencv2/opencv.hpp"


//#define CONNECT_TEST
#define H264_TEST
//#define YOLO_TEST

void drawPred(int classId, float conf, int left, int top, int right, int bottom, cv::Mat& frame)
{
	std::vector<std::string> classes = Yolo_origin::GetClasses();
	cv::rectangle(frame, cv::Point(left, top), cv::Point(right, bottom), cv::Scalar(0, 255, 0));

	std::string label = cv::format("%.2f", conf);
	if (!classes.empty())
	{
		CV_Assert(classId < (int)classes.size());
		label = classes[classId] + ": " + label;
	}

	int baseLine;
	cv::Size labelSize = cv::getTextSize(label, cv::FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);

	top = std::max(top, labelSize.height);
	rectangle(frame, cv::Point(left, top - labelSize.height),
		cv::Point(left + labelSize.width, top + baseLine), cv::Scalar::all(255), cv::FILLED);
	putText(frame, label, cv::Point(left, top), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar());
}



int main(int argc, char* arg[])
{

#ifdef CONNECT_TEST
	Connector* connect = Connector::GetInstance();
	if (!connect->readyToConnect("192.168.0.6",9050)) {
		std::cout << "ready failed" << std::endl;
	}
	SOCKET client = connect->acceptClient();
	std::cout << "client 연결됨" << std::endl;
	char buffer[255];
	while (true) {
		int sendLen = 0;
		std::cin >> buffer;
		if (strcmp(buffer,"end")==0)
			break;
		else {
			char lenBuffer[5];
			int stringLen = strlen(buffer);
			memcpy(lenBuffer, (void*)&stringLen, sizeof(int));
			send(client, lenBuffer, 4, 0);
			sendLen = send(client, buffer, strlen(buffer), 0);
		}
		std::cout << sendLen << std::endl;
	}	
#endif
#ifdef H264_TEST
	cv::Mat img;
	const char *szFilePath = "C:\\Users\\rnwhd\\Downloads\\serenity_hd_dvd-trailer\\test.mp4";
	if(VideoDecoder::ready(szFilePath)){		
		ImageProcess* processor = new ViewProcess();
		VideoDecoder::Decode(processor);
		delete processor;
	}
#endif
#ifdef YOLO_TEST
	
	const char* cmd[] = {" ","--config=C:/Users/rnwhd/source/repos/opencv/x64/Debug/yolov3.cfg",
		"--model=C:/Users/rnwhd/source/repos/opencv/x64/Debug/yolov3.weights",
		"--classes=C:/Users/rnwhd/source/repos/opencv/x64/Debug/coco.names",
		"--scale=0.4","--width=416","--height=416","--rgb" };
	if (Yolo_origin::ready(8, cmd)) {

		cv::VideoCapture vc("C:\\Users\\rnwhd\\Downloads\\serenity_hd_dvd-trailer\\test.mp4");
		int key=10;
		while ((key=cv::waitKey(10))!=27) {
			cv::Mat img;
			vc >> img;
			cv::imshow("img", img);
			if (key == 'a'){
				Yolo_Result result = Yolo_origin::YOLO(img);
				for (size_t i = 0; i < result.indices.size(); ++i)
				{
					int idx = result.indices[i];
					cv::Rect box = result.boxes[idx];
					drawPred(result.classIds[idx], result.confidences[idx], box.x, box.y,
						box.x + box.width, box.y + box.height, img);
				}
			}
			cv::imshow("img", img);
			cv::waitKey(1);
		}
	}
#endif
}