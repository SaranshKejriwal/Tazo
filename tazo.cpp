#include <iostream>
#include <vector>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/objdetect/objdetect.hpp>//for Cascade Classifiers
#include<stdio.h>//for sprintf

using namespace cv;
using namespace std;

vector<Point> kalmanv;//to load coordinates of previous and next points
CascadeClassifier facecascade;

int main () 
{
        Mat img(480, 640, CV_8UC3);
	Mat t1=imread("gybolg.jpg");
	Mat t2=imread("tao.jpg");
	
	Mat t_res;
	VideoCapture cap(0);
	facecascade.load("haarcascade_frontalface_alt.xml");
	namedWindow("facepos");
	moveWindow("facepos",1040,630);
        namedWindow("gybolg");
        namedWindow("tao");
	namedWindow("tazo");
	moveWindow("tazo",440,0);
	moveWindow("tao",80,0);
	moveWindow("gybolg",1000,0);
	
	imshow("gybolg",t1);
	imshow("tao",t2);
	//make tazo effect
	Mat taz1(t1.rows,t1.cols,CV_8UC3,Scalar(255,255,255)),taz2(t2.rows,t2.cols,CV_8UC3,Scalar(255,255,255));
	for(int i=0;i<int(t1.cols);i++)
	{
		if(i%2==0)
		{
			t1.col(i).copyTo(taz1.col(i));//even cols on taz1
		}
		else //if(i%2==1)
		{
			t2.col(i).copyTo(taz2.col(i));//odd cols on taz2
		}
		
		
	}
	//imshow("taz1",taz1);
	//imshow("taz2",taz2);

	resize(t1,t1,Size(2*t1.cols,2*t1.rows));//tazo looks bigger
	resize(t2,t2,Size(2*t2.cols,2*t2.rows));
	resize(taz1,taz1,Size(2*taz1.cols,2*taz1.rows));//tazo looks bigger
	resize(taz2,taz2,Size(2*taz2.cols,2*taz2.rows));
	int x_pos=320;//to decide the ratio for addweighted
	float add_rat=(float)x_pos/420;
        for(;;)
        {
			cap>>img;
			flip(img,img,1);
			cvtColor(img,img,CV_BGR2GRAY);
			vector<Rect>faces;
			facecascade.detectMultiScale(img,faces,1.1,2,0|CV_HAAR_SCALE_IMAGE,Size(70,70));//30,30 is max object size
			Point facecenter;
				if(faces.size()>0)
				 {
				 cout<<"\a";
				// cout<<"sound\n";
				 for(int i=0;i<faces.size();i++)
					{
						facecenter=Point((faces[i].x+faces[i].width/2),(faces[i].y+faces[i].height/2));
						//putText(img,"Face",Point(40,10),FONT_HERSHEY_TRIPLEX,1,Scalar(250,250,250),2);
						rectangle(img,faces[i],Scalar(200,200,250),30,8,0);
						circle(img,facecenter,5,Scalar(255,255,255),30);
						if(faces[i].x>30)
						{
							x_pos=faces[i].x;//make sure that tracker doesn't go to origin
							add_rat=(float)x_pos/420;//addition ratio for add weighted
							//addWeighted(t1,add_rat,t2,1-add_rat,0,t_res);
							addWeighted(taz1,add_rat,taz2,1-add_rat,0,t_res);
						}
					 }
				}
			char text[10];
			sprintf(text, "x_pos: %f ", add_rat);//to print a variable text on screen.
			putText(img,text,Point(40,40),FONT_HERSHEY_TRIPLEX,2,Scalar(250,250,250),2);
			
			/*x_max=426 and x_min=43*/	

            resize(img,img,Size(0.1*img.cols,0.1*img.rows));
	    imshow("tazo",t_res);
            imshow( "facepos", img );
            char code = waitKey(10);
	 if(code == 'b' || code == 'B' )//breaks when Q is pressed
            break;
			
         
        }
       
  
	
    return 0;
}
