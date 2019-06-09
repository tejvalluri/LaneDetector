#include "DetectTrafficSignal.h"
void printTime()
{
  time_t rawtime;
  struct tm * timeinfo;
  time( &rawtime );
  timeinfo = localtime( &rawtime );
  cout<< "Current local time and date: "<<asctime (timeinfo)<< endl;
}
bool pointCompareSort(cv::Point2f  a,cv::Point2f  b)
{
   if(a.y > b.y)
     return true;
   else
     return false;
}
bool momentsCompareSort(cv::Moments  mu_1,cv::Moments  mu_2)
{
   if((mu_1.m01/mu_1.m00) > (mu_2.m01/mu_2.m00))
     return true;
   else
     return false;
}
 DetectTrafficSignal::DetectTrafficSignal()
  {
    iLowH = 0;
    iHighH = 10;

    iLowS = 100;
    iHighS = 255;

    iLowV = 100;
    iHighV = 255;
  }
  DetectTrafficSignal::DetectTrafficSignal(int lh,int ls,int lv,int hh,int hs,int hv)
  {
      iLowH = lh;
      iLowS = ls;
      iLowV = lv;

      iHighH = hh;
      iHighS = hs;
      iHighV = hv;
  }
Mat DetectTrafficSignal::findSignalContours(Mat imgOriginal)
   {
     //cout<<"\n begin";
     printTime();
     Mat imgHSV, imgThresholded,imgThresholdBlack;

     cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV);
     //cout<<endl<<"after cvt";
     printTime();
     inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded);//for red
     inRange(imgHSV, cv::Scalar(0, 0, 0, 0), cv::Scalar(180, 255, 30, 0), imgThresholdBlack);//for black

     erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
     dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

     dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
     erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );


     //erode(imgThresholdBlack, imgThresholdBlack, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
     //dilate( imgThresholdBlack, imgThresholdBlack, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

     //cout<<endl<<"after erode";
     printTime();
     Mat imgThresholdGreen;
     inRange(imgHSV, cv::Scalar(0, 0, 0, 0), cv::Scalar(180, 255, 30, 0), imgThresholdGreen);//for black

     erode(imgThresholdGreen, imgThresholdGreen, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
     dilate( imgThresholdGreen, imgThresholdGreen, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

     dilate( imgThresholdGreen, imgThresholdGreen, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
     erode(imgThresholdGreen, imgThresholdGreen, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
     //vector<Moments> mu_3 = findCentres(imgThresholdGreen,points2);
     //cout<<endl<<"after erode green";
     printTime();




     //imshow("Thresholded Black Image", imgThresholdBlack);
     //Mat contureImage(imgThresholdBlack);
    //imshow("Thresholded Image", imgThresholded); //show the thresholded image
     //cout<<endl<<"finding";
     vector<Point2f> points1,points2,points3;
     vector<Moments> mu_1 = findCentres(imgThresholded,points1);
     vector<Moments> mu_2 = findCentres(imgThresholdBlack,points2);
     vector<Moments> mu_3 = findCentres(imgThresholdGreen,points3);
    //cout<<endl<<"after centers";
     printTime();
     /*for(unsigned int i = 0;i< points1.size();i++)
     {
       //cout<<endl<<points1[i].x<<" "<<points1[i].y;
     }
     for(unsigned int i = 0;i< points2.size();i++)
     {
       //cout<<endl<<points2[i].x<<" "<<points2[i].y;
     }
     std::sort (points1.begin(), points1.end(), pointCompareSort);
     std::sort (points2.begin(), points2.end(), pointCompareSort);
     if(checkForRedSignal(points1,points2))*/
     std::sort (mu_1.begin(), mu_1.end(), momentsCompareSort);
     std::sort (mu_2.begin(), mu_2.end(), momentsCompareSort);
     std::sort (mu_3.begin(), mu_3.end(), momentsCompareSort);
     //cout<<endl<<"before red";
     printTime();
     if(checkForRedSignal(mu_1,mu_2))
     {
         //cout<<"\nHi Mate Red it is,stop!!";
     }
     else if(checkForRedSignal(mu_3,mu_2))
     {
         //cout<<"\nHi Mate Green it is,good to go";
     }
     else
     {
         //cout<<"\nGreen or nothing";
     }
     //cout<<endl<<"after red";
     printTime();
     return imgThresholded;
   }
bool DetectTrafficSignal::checkForRedSignal(vector<Moments>&p1,vector<Moments>&p2)
{
  for(unsigned int i = 0;i<p1.size();i++)
  {
      for(unsigned int j = 0;j<p2.size();j++)
      {
           float x1 = p1[i].m10/p1[i].m00;
           float y1 = p1[i].m01/p1[i].m00;

           float x2 = p2[j].m10/p2[j].m00;
           float y2 = p2[j].m01/p2[j].m00;

           float dist= sqrt(pow((x2-x1),2) + pow((y2-y1),2));
           if(dist <= p1[i].m00 && p1[i].m00 <= 45)
           {
             //cout<<endl<<"distance "<<p1[i].m00;
             return true;
           }
      }
  }
  return false;
}
vector<Moments> DetectTrafficSignal::findCentres(Mat imgThresholded,vector<Point2f> &points)
{
  RNG rng(12345);
  Mat canny_output;
  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;
  findContours( imgThresholded, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

  vector<Moments> mu(contours.size() );
  for( unsigned int i = 0; i < contours.size(); i++ )
  {
    mu[i] = moments( contours[i], false );
  }

  vector<Point2f> mc( contours.size() );
  for( unsigned int i = 0; i < contours.size(); i++ )
  {
    mc[i] = Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 );
  }
  points = mc;
  Mat drawing = Mat::zeros( /*canny_output*/imgThresholded.size(), CV_8UC3 );
  for( unsigned int i = 0; i< contours.size(); i++ )
  {

    //cout<<endl<<mu[i].m10/mu[i].m00 <<"  "<< mu[i].m01/mu[i].m00;
    Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
    drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );
    circle( drawing, mc[i], 4, color, -1, 8, 0 );
  }
  //imshow( "Contours Circles", drawing );
  return mu;
}
