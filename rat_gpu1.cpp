#include <iostream>
#include <time.h>
#include <fstream>
#include<string>
#include <iomanip>
#include <Windows.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace std;
using namespace cv;
	String extension=".csv";
	int number=1;
	String numb=to_string(number);
	String path="ashish"+numb+extension;
int cl=1;
double total_distance=0.0;
int previous_time=0;
int current_time=0;
UMat imgOriginal;
int entry=0;
Mat mMask,tFront;
Mat imgLines,imgTmp;

int ashish=0;
const int extreme=15;// limts of number to be selected
    int lineType = 8;                        
Point pts[1][extreme];
int cnt=-1;  // this is showing number of points selected
// Flags updated according to left mouse button activity
bool ldown = false, lup = false;
// Original image

// Starting and ending points of the user's selection
Point corner1, corner2;
// ROI
Rect box;
// Callback function for mouse events
static void mouse_callback(int event, int x, int y, int, void *)
{
	if(event == EVENT_RBUTTONDOWN)
	{ cout<< "you have pressed right button to start real thing"<<endl;
		entry=1;
	}

	

	if (cnt<extreme){

	
// When the left mouse button is pressed, record its position and save it in corner1
if((event == EVENT_LBUTTONDOWN ) )
{
	cnt++;
	
corner1.x = x;
corner1.y = y;
pts[0][cnt]=Point(corner1.x,corner1.y);
cout << "Corner "<<cnt <<" recorded at x:" << pts[0][cnt].x<<" recorded at y:" << pts[0][cnt].y <<"count no is :"<<cnt<< endl;


}

}
	else        /// means right button is pressed to end
	{  
		
		mMask.create(imgOriginal.rows,imgOriginal.cols,imgOriginal.type());
		mMask.setTo(0);
		namedWindow("cropping", CV_WINDOW_AUTOSIZE);
		const Point* ppt[1] = { pts[0] };
		int npt[] = { extreme };

		
	
			 fillPoly(mMask,
            ppt,
            npt,
            1,
           Scalar(0xFF,0xFF,0xFF ),
            lineType );
		imshow("cropping",mMask);
	
	
	}



 
	

}


/////////////////////////////////////////////////////////////



int main()
{
  ifstream indata;
	ofstream outdata;

	outdata.open("E:/"+path);	
VideoCapture cap("E:/bt.mp4"); //capture the video from webcam

if ( !cap.isOpened() ) // if not success, exit program
{
cout << "Cannot open the web cam" << endl;
return -1;
}
int iLowH = 0;
int iHighH = 179;
int iLowS = 0;
int iHighS = 255;
int iLowV =69;
int iHighV = 255;//89


namedWindow("thresh", CV_WINDOW_AUTOSIZE);
namedWindow("original", CV_WINDOW_AUTOSIZE);
namedWindow("cropp", CV_WINDOW_AUTOSIZE);
namedWindow("originall", CV_WINDOW_AUTOSIZE);

createTrackbar("LowH", "originall", &iLowH, 179); //Hue (0 - 179)
createTrackbar("HighH", "originall", &iHighH, 179);
createTrackbar("LowS", "originall", &iLowS, 255); //Saturation (0 - 255)
createTrackbar("HighS", "originall", &iHighS, 255);
createTrackbar("LowV", "originall", &iLowV, 255);//Value (0 - 255)
createTrackbar("HighV", "originall", &iHighV, 255);
int iLastX = -1;
int end=0;
int iLastY = -1;
cap.read(imgTmp);
outdata<<"distance"<<","<<"time"<<","<<"velocity"<<endl;
//Create a black image with the size as the camera output
imgLines = Mat::zeros( imgTmp.size(), CV_8UC3 );



clock_t tStart = clock();
double dd=0.0;
while (true )
{
	if(cl==1){
Mat imgOriginall;
UMat tFrontt;

bool bSuccess = cap.read(imgOriginall); // read a new frame from video
//bool bSuccess1 = cap1.read(imgOriginall); // read a new frame from video

if (!bSuccess) //if not success, break loop
{
cout << "Cannot read a frame from video stream" << endl;
break;
}

imgOriginall.copyTo(imgOriginal);
     
	






if(entry==1){
	/*ashish++;
	if(ashish==1){
	 tStart = clock();
	}*/
	bitwise_and(imgOriginal, mMask, tFront);
	
	dd=(double)(clock() - tStart)/CLOCKS_PER_SEC;
	putText(imgOriginal,to_string(dd),Point(10,10),1,1,Scalar(255,255,255));
	putText(imgOriginal,"file open is"+" "+path,Point(200,10),1,1,Scalar(255,255,255));
	
Mat imgHSV;

cvtColor(tFront, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV
//Mat imgThresholded2;
Mat imgThresholded;


inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image
//morphological opening (removes small objects from the foreground)
erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)) );
dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)) );


imshow("thresh",imgThresholded);
Moments oMoments = moments(imgThresholded);
double dM01 = oMoments.m01;
double dM10 = oMoments.m10;

double dArea = oMoments.m00;
  double instant_time=0;
if (dArea > 10000)
{ashish++;
	if(ashish==1){
	 tStart = clock();
	}
//calculate the position of the ball
int posX = dM10 / dArea;
int posY = dM01 / dArea;
current_time = clock();
if (iLastX >= 0 && iLastY >= 0 && posX >= 0 && posY >= 0)
{
//Draw a red line from the previous point to the current point
	//cout<<"posX"<<posX;
	//cout<<"posY"<<posY<<endl;
	line(imgLines, Point(posX, posY), Point(iLastX, iLastY), Scalar(0,0,255), 2);
	int xx=pow((posX-iLastX),2);
	int yy=pow((posY-iLastY),2);
	double dist=(double)sqrt(xx+yy);
	total_distance+=dist;
	if (previous_time>0){
       instant_time=(double)(current_time-previous_time)/CLOCKS_PER_SEC;
	  //cout<<"Veloity is "<<(dist/instant_time)<<" px/second"<<endl;
	}

	putText(imgOriginal,"distance is "+to_string(dist),Point(150,300),1,1,Scalar(255,255,255));
	putText(imgOriginal,"Total distance is "+to_string(total_distance),Point(150,400),1,1,Scalar(255,255,255));
	outdata<<total_distance<<","<<(double)(current_time-tStart)/CLOCKS_PER_SEC<<","<<(dist/instant_time)<<endl;
	cout<<(double)(current_time-tStart)/CLOCKS_PER_SEC<<endl;

}
iLastX = posX;
iLastY = posY;
previous_time=current_time;



}


tFront = tFront + imgLines;
tFront.copyTo(tFrontt);
imshow("cropp",tFrontt);


}}

imshow("original",imgOriginal);  // show original Image and used for cropping interction


setMouseCallback("original", mouse_callback);

if (waitKey(30) == 'x') //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
{cl=0;
imgLines = Mat::zeros( imgTmp.size(), CV_8UC3 );

}
if (waitKey(30) == 's') //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
{cl=1;
}
if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
{
cout << "esc key is pressed by user" << endl;
break;
}


}
return 0;
}