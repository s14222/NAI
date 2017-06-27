#include "ExtraImg.h"

using namespace cv;
using namespace std;

CascadeClassifier face_cascade;
CascadeClassifier eye_cascade;


ExtraImg * bonus[10];
int nrExtra = 0;
string faceCascadeName = "C:\\haarcascade_frontalface_alt.xml";
string eyeCascadeName = "C:\\haarcascade_eye.xml";



void overlayImage(const cv::Mat &background, const cv::Mat &foreground, cv::Point2i location);
void drawHelpEyelines(vector<Rect> eye, Mat img)
{
	for (unsigned i = 0; i + 1 < eye.size(); i+=2)
	{
		Rect rect_face;    //Kwadrat okreslaj¹cy twarz 
						   //ellipse( img, center, Size( faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, Scalar( 255, 120, 12 ), 2, 2, 0 ); 
		rect_face.width = eye[i].width;
		rect_face.height = 2;
		rect_face.x = eye[i].x;
		rect_face.y = eye[i].y + eye[i].height / 2;
		rectangle(img, rect_face, Scalar(1, 255, 255), 1, 1, 0);

		Mat tmp;
		

		/*WYLICZENIE SRODKA MIEDZY OCZAMI*/

		int x1 = eye[i].x + eye[i].width / 2;
		int y1 = eye[i].y + eye[i].height / 2;

		int x2 = eye[i+1].x + eye[i+1].width / 2;
		int y2 = eye[i+1].y + eye[i+1].height / 2;
		int x = (x1 + x2) / 2;
		int y = (y1 + y2) / 2;


		// /10 i mnozenie przez 10 aby obrazek nie drga³ 
		int width = (x2 - x1) / 10.0;
		
		if (x1 > x2)
		{
			width = (x1 - x2) / 10.0+4;
		}
		width *= 20;

		x -= width / 2;

		if (bonus[nrExtra]->type == EYE)
		{
			resize(bonus[nrExtra]->img, tmp, Size(width, width / 2));

			// Dodanie obrazka do obrazu 
			overlayImage(img, tmp, Point2i(x+ bonus[nrExtra]->x, y + bonus[nrExtra]->y));
		}
	}
}
void drawHelpHeadLine(vector<Rect> faces, Mat img)
{
	Mat x;
	//cvtColor(img, x, CV_BGR2RGBA,4);

	for (unsigned i = 0; i < faces.size(); i++)
	{
		


		Mat tmp;
		// /10 i mnozenie przez 10 aby obrazek nie drga³ 
		int width = faces[i].width / 10.0;
		width *= 10;
		// Skalowanie obrazka
		if (bonus[nrExtra] -> type == HEAD)
		{
			resize(bonus[nrExtra]->img, tmp, Size(width, width / 2));

			// Dodanie obraza do obrazu 
			overlayImage(img, tmp, Point2i(faces[i].x + bonus[nrExtra]->x, faces[i].y + bonus[nrExtra]->y));
		}
		

		/*Dodanie lini do obrazu*/
		/*
		Rect rect_face;
		rect_face.width = faces[i].width;
		rect_face.height = 2;
		rect_face.x = faces[i].x;
		rect_face.y = faces[i].y;
		*/
		// POmocniece rysowanie lini 
		//rectangle(x, rect_face, Scalar(255, 255, 255), 1, 1, 0);
	}

}
void overlayImage(const Mat &background, const Mat &small, Point2i location)
{
	/*Kod tej funkcji wziêty ze strony:
	https://stackoverflow.com/questions/35463797/opencv-stitching-how-to-get-better-performance-using-opencv-stitcher-class
	*/
	for (int y = std::max(location.y, 0); y < background.rows; ++y) // Przejscie po wierszach 
	{
		int fY = y - location.y; 
		if (fY >= small.rows)
			break;
		for (int x = std::max(location.x, 0); x < background.cols; ++x) //przejscie po kolumnach 
		{
			int fX = x - location.x; 
			if (fX >= small.cols)
				break;
			double opacity =((double)small.data[fY * small.step + fX * small.channels() + 3])/ 255.0;
			for (int c = 0; opacity > 0 && c < background.channels(); ++c) // poloczenie obrazkow i dla kazdego kana³u 3->4->3 
			{
				unsigned char foregroundPx = small.data[fY * small.step + fX * small.channels() + c];
				unsigned char backgroundPx = background.data[y * background.step + x * background.channels() + c];
				background.data[y*background.step + background.channels()*x + c] = backgroundPx * (1. - opacity) + foregroundPx * opacity;
			}
		}
	}
}
void detectFace(Mat img)
{
	//vektory do przechowywania info o twarzy
	vector<Rect> faces;
	vector<Rect> eye;


	Mat img_gray;                                
	cvtColor(img, img_gray, CV_BGR2GRAY);                // img tylko odcienie szarosci bez kolorow i Czarnobiale (wykrywaie twarzy)
	
	/*Wykrycie twarzy i oczu*/
	//https://4programmers.net/Forum/C_i_C++/273919-opencv_?p=1269036
	face_cascade.detectMultiScale(img_gray, faces, 1.1, 3, 0 | CV_HAAR_SCALE_IMAGE, Size(50, 50));
	eye_cascade.detectMultiScale(img_gray, eye, 1.1, 3, 0 | CV_HAAR_SCALE_IMAGE, Size(50, 50));

	/*Dodanie obrazka do wykrytych twarzy / oczu */
	drawHelpHeadLine(faces, img);
	drawHelpEyelines(eye, img);


}

int main(int argc, char** argv)
{
	
	/*Zaladowanie obrazku */
	bonus[0] = new ExtraImg("C:\\1.png", HEAD, 0, 130);
	bonus[1] = new ExtraImg("C:\\2.png", HEAD, 40, -60);
	bonus[2] = new ExtraImg("C:\\3.png", HEAD, -30, -60);
	bonus[3] = new ExtraImg("C:\\4.png", HEAD, 0, -70);
	bonus[4] = new ExtraImg("C:\\5.png", EYE, 0, -40);
//	bonus[5] = new ExtraImg("C:\\6.png", EYE, 0, -40);
	bonus[5] = new ExtraImg("C:\\6.png", HEAD, 0, -75);
	//bonus[0] = new ExtraImg("C:\\1.png", HEAD, 0, -75);
	string window_name = "Hello Face !";
	VideoCapture cap;
	
	// open the default camera, use something different from 0 otherwise;
	// Check VideoCapture documentation.

	if (!cap.open(0))
		return 0;
	if (!face_cascade.load(faceCascadeName))    // £adowanie xml aby wykrywal twatz
	{
		cout << "Nie znaleziono pliku " << faceCascadeName << ".";
		return -2;
	}

	if(!eye_cascade.load(eyeCascadeName))  // Ladowanie xml do wykrywania oczu 
	{
		cout << "Nie znaleziono pliku " << faceCascadeName << ".";
		return -2;
	}
	for (;;)
	{
		//wzorowane siê na: http://answers.opencv.org/question/1/how-can-i-get-frames-from-my-webcam/
		Mat frame;
		/*Czytanie z kamerki*/
		cap >> frame;
		/*Wykryj i dodaj obrazek*/
		detectFace(frame);

		if (frame.empty()) break; //sprawdzenie czy popbrano klatke z kamerki
			imshow("s14222", frame);
		
	
		if ((waitKey(10) % 256) == 27) break;
		if (waitKey('A') % 256 == 65)
		{
			cout << "[Info] Zmieniono obrazek"<< endl;

			nrExtra++;
			nrExtra %= 6;
		}
		
	}

	// cap.close();
	return 0;
}

