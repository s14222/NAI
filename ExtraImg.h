#pragma once
#include "typeDetect.h"
/*Klasa przechowuje info o sciezce pliku, przechowuje obraz w atrybucie typu mat oraz x i y odpowieadajace za przesunniecie obrazka */
class ExtraImg {

public:
	typeDetect type;
	cv::Mat img;
	int x;
	int y;
public:
	ExtraImg();
	ExtraImg(std::string filePath, typeDetect type, int x, int y);

};