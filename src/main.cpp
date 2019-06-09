#include "DetectTrafficSignal.h"
#include "LaneSwitcher.h"
#include <fstream>
using namespace std;

Mat regionOfInterest(Mat &imgOriginal)
{
  cv::Rect roi((int)imgOriginal.cols/2,0,(int)imgOriginal.cols/3,(int)imgOriginal.rows);
  Mat half(imgOriginal,roi);
  imshow("half", half);
  return half;
}
void printTime2(string s)
{
  ofstream myfile;
  myfile.open ("C:/Users/tej/Desktop/Test_19Sep/log/timing.log",std::ofstream::app);

  time_t rawtime;
  struct tm * timeinfo;
  time( &rawtime );
  timeinfo = localtime( &rawtime );
  /*cout*/myfile<<endl<<s<< "Current local time and date: "<<asctime (timeinfo)<< endl;
  myfile.close();
}

int main( int argc, char* argv[] )
{
  Mat imgOriginal = imread("C:/Users/tej/Desktop/Test_19Sep/res/NF3.PNG");
  if( !imgOriginal.data )
  {
    cout<<"failed";
    return -1;
  }



  int choice = -1;
  cout<<"\n1.Traffic Signal detection\n2.Lane detection\n";
  cin>>choice;
  if(choice  == 1)
  {
    DetectTrafficSignal *dts = new DetectTrafficSignal();
    Mat imgThresholded = dts->findSignalContours(regionOfInterest(imgOriginal));//left most part only
  }
  else if(choice == 2)
  {
    int mode = 0,len;
    if(argc > 2)
    {
      mode = atoi(argv[1]);
      len = atoi(argv[2]);
    }
    else
      exit(0);
    //cout<<"\nmode "<<mode;

    LaneSwitcher *ls = new LaneSwitcher(mode,len,150);
    LaneSwitcher *ls1 = new LaneSwitcher(2,len,150);
	LaneSwitcher *ls2 = new LaneSwitcher(3,len,150);

    /*if(!ls->initiateLaneDetection(imgOriginal))
    {
        cout<<endl<<"Entering fail safe mode";
        LaneSwitcher *ls1 = new LaneSwitcher(2,len,150);
        if(!ls1->initiateLaneDetection(imgOriginal))
        {
            cout<<endl<<"Entering fail safe mode 2";
            LaneSwitcher *ls2 = new LaneSwitcher(3,len,150);
            if(!ls2->initiateLaneDetection(imgOriginal))
            {
              cout<<endl<<"all failed";

            }
        }
    }*/

    VideoCapture cap("C:/Users/tej/Desktop/Test_19Sep/res/Highway.MP4");
    if(!cap.isOpened())
      return -1;

    Mat sizeFrame;
    cap >> sizeFrame;
    VideoWriter writer;
    int codec = CV_FOURCC('M', 'J', 'P', 'G');
    double fps = 25.0;
    bool isColor = true;
    string filename = "C:/Users/tej/Desktop/Test_19Sep/res/Jun4.avi";


    writer.open(filename, codec, fps, sizeFrame.size(), isColor);
    writer.set(VIDEOWRITER_PROP_QUALITY, 0.1);
	if (!writer.isOpened())
	{
        cout << "Could not open the output video file for write\n";
        return -1;
    }



    Mat frame;
    int count = 0;
    printTime2("before write");
    while(1)
    {
       cout<<endl<<"counter "<<count;
       cap >> frame;
       //ls->initiateLaneDetection(frame);
       if(!ls->initiateLaneDetection(frame))
       {
         cout<<endl<<"Entering fail safe mode";
         if(!ls1->initiateLaneDetection(frame))
         {
           cout<<endl<<"Entering fail safe mode 2";
           if(!ls2->initiateLaneDetection(frame))
           {
              cout<<endl<<"all failed";
           }
         }
       }
       count++;
       if(count > 6000)
         break;
       writer.write(frame);
    }
    printTime2("after write");

    cap.release();
    writer.release();
  }
  else
  {
      cout<<endl<<"exiting";
      exit(0);
  }

  //namedWindow("Lane_detector", WINDOW_OPENGL);
  //imshow("Lane_detector", frame);

  //imshow("Thresholded Image", imgThresholded); //show the thresholded image
  //imshow("Original", imgOriginal);
  //cout<<"rows "<<(int)imgOriginal.rows<<"cols "<<(int)imgOriginal.cols;

  waitKey(0);
  return 1;
}
