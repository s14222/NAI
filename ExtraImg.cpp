#include "ExtraImg.h"
ExtraImg::ExtraImg() {};
ExtraImg::ExtraImg(std::string filePath, typeDetect type, int x, int y) {
	this->img = cv::imread(filePath, CV_LOAD_IMAGE_UNCHANGED);
	this->x=x;
	this->y=y;
	this->type = type;
}