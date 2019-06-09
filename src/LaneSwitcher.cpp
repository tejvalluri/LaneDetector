#include "LaneSwitcher.h"
#include <fstream>
void printTime1(string s)
{
  time_t rawtime;
  struct tm * timeinfo;
  time( &rawtime );
  timeinfo = localtime( &rawtime );
  cout<<endl<<s<< "Current local time and date: "<<asctime (timeinfo)<< endl;
}
bool distanceFromX(CartesianBuilder a,CartesianBuilder b)
{
  if(a.x1 < b.x1)
    return true;
  else if(a.x1 > b.x1)
    return false;
  else
  {
    if(a.y1 < b.y1)
      return true;
    else
      return false;
  }
  return false;
}

bool distanceFromOrigin(CartesianBuilder a,CartesianBuilder b)
{
    //(0,a.CarRows)(a.x1,a.y1)
    /*float dist1 = sqrt((a.x1*a.x1) +pow((a.y1-a.CarRows),2));
    float dist2 = sqrt((b.x1*b.x1) +pow((b.y1-b.CarRows),2));*/
    if(a.y1 > b.y1)
      return true;
    else if(a.y1 < b.y1)
      return false;
    else
    {
      if(a.x1 > b.x1)
        return true;
      else
        return false;
    }
    return false;
}
int max(int a,int b,int c)
{
    if(a > b && a > c)
      return a;
    else if(b > a && b > c)
      return b;
    else
      return c;
}
LaneSwitcher::LaneSwitcher(int mode,int len,int dist)
{
  lMode = mode;
  distance = dist;
  if(mode == 1)
  {
      R = 200;
      G = 170;
      B = 170;
  }
  else if(mode == 2)
  {
      R = 140;
      G = 150;
      B = 140;
  }
  else if(mode == 3)
  {
      R = G = B = 140;
  }
  length = len;
}
bool LaneSwitcher::customLaneDetect(Mat &img)
{
    //cout<<"\nCustomLaneDetector";
    Mat lineImg = img;//.clone();//video
    printTime1("Starting");
    uchar r, g, b;
    int len = 0;
    int prevX = 0,prevY = 0;
    bool isWhite = false,isBlack = false;
    //ofstream myfile;
    std::vector<CartesianBuilder> coordVector;
    //myfile.open ("C:\Users\tej\Desktop\Test_19Sep\log\example.txt");

    //cout<<"\n i: "<<img.cols<<" j: "<<img.rows;

    int pR = -1,pG = -1,pB = -1,lengthObj = -1,pX = -1,pY = -1,lineCount = 0;
    bool resetRGB = false;
    /*changes to store potential obj start*/
    std::vector<CartesianBuilder> vehVector;
    int prevObjX = -1,prevObjY = -1;
    /*changes to store potential obj end*/

    /*prev points*/
    CartesianBuilder *currObj = new CartesianBuilder(0,0,0,0),*prevObj = new CartesianBuilder(0,0,0,0);
    /*prev points*/


    for (int i = 0; i < img.cols; ++i)
    {
        isBlack = isWhite = false;
        prevX = prevY = len = 0;

        resetRGB = true;
        lengthObj = 0;
        pR = pG = pB = -1;
        bool isFirst = false,lineStarted = false;
        for (int j = img.rows/3; j < img.rows-20; ++j)
        {
            cv::Vec3b* pixel = img.ptr<cv::Vec3b>(j);
            //object identification begins

            {
                if(pR < 0 || resetRGB)
                {
                    prevObjX = i;
                    prevObjY = j;
                    resetRGB = false;
                    pX = i;
                    pY = j;
                    if(pR < 0 || abs(pR - pixel[i][2]) < 50 && abs(pG - pixel[i][1]) < 50 && abs(pB - pixel[i][0]) < 50)
                    {
                        pR = (int)pixel[i][2];
                        pG = (int)pixel[i][1];
                        pB = (int)pixel[i][0];
                    }
                    if(lineStarted && lengthObj > 10 && lengthObj < 150 && pixel[i][2] < 155 && pixel[i][1] <155 && pixel[i][0] < 155)
                    {
                        line( img,Point(pX,pY), Point(i,j-1), Scalar(255,0,250), 1, CV_AA);

					}
					lineStarted = false;
					lengthObj = 0;
                }
                else
                {
                    if(abs(pR - pixel[i][2]) > 50 && abs(pG - pixel[i][1]) > 50 && abs(pB - pixel[i][0]) > 50)
                    {
                        if(lineStarted)
                        {

                            //cout<<endl<<"length "<<lengthObj<<" x1 "<<pX<<" y1 "<<pY<<" x2 "<<i<<" j "<<j;
                            //cout<<endl<<"pR "<<pR<<" pG "<<pG<<" rB "<<pB;
                            //cout<<endl<<"cR "<<(int)pixel[i][2]<<" cG "<<(int)pixel[i][1]<<" cB "<<(int)pixel[i][0]<<endl;

                            if((i - 1) > 0 && pixel[i][2] < 155 && pixel[i][1] < 155 && pixel[i][0] < 155)
                              //line( img,Point(pX,pY), Point(i,j-1), Scalar(255,250,0), 0.5, CV_AA);//-->tej uncomment for all lines to be drawn
                              vehVector.push_back(CartesianBuilder(pX,pY,i,j-1));

                            lineStarted = false;
                            lengthObj = 0;
                            lineCount++;
                            resetRGB = true;
                        }
                        else
                        {

                            //cout<<"\nsetting "<<(int)pixel[i][2]<<" "<<(int)pixel[i][1]<<" "<<(int)pixel[i][0]<<"pR "<<pR<<" pG "<<pG<<" rB "<<pB;;
                            lineStarted = true;
                            pX = i;
                            pY = j;
                            pR = (int)pixel[i][2];
                            pG = (int)pixel[i][1];
                            pB = (int)pixel[i][0];
                            lengthObj = 1;

                        }
                    }
                    else
                    {
                        if(lineStarted)
                        {
                            lengthObj++;
                        }
                        else
                        {
                            resetRGB = true;
                            lengthObj = 0;
                        }

                        //line( img,Point(pX,pY), Point(i,j), Scalar(255,250,0), 1, CV_AA);
                    }
                }
            }
            //object identification ends
            //if(j >= img.rows/5)
            {

                r = pixel[i][2];
                g = pixel[i][1];
                b = pixel[i][0];
                if((int)r > R && (int)g > G && (int)b > B && j != img.rows - 2)
                {
                    if(len == 0)
                    {
                        prevX = i;
                        prevY = j;
                    }
                    isWhite = true;
                    len++;
                }
                else
                {
                  if(isWhite)
                  {
                      if((len > length))
                      {
                        //cout<<endl<<"x "<<i<<" y "<<j<<" PrevX "<<prevX<<" PrevY"<<prevY<<" len "<<len;
                        //myfile<<endl<<"x "<<i<<" y "<<j<<" PrevX "<<prevX<<" PrevY"<<prevY<<" len "<<len;
                        CartesianBuilder *c = new CartesianBuilder(prevX,prevY,i,j,img.rows);
                        coordVector.push_back(*c);
                        //line( lineImg, Point(i,j), Point(prevX,prevY), Scalar(100,10,0), 3, CV_AA);//inter lines
                        prevX = prevY = 0;
                      }
                   }
                   prevX = prevY = 0;
                   isWhite = false;
                   len = 0;
                }
            }
        }
    }

    if(coordVector.size() == 0)
      return false;
    //printTime1();
    sort(coordVector.begin(),coordVector.end(),distanceFromOrigin);
    /*for (std::vector<CartesianBuilder>::iterator it = coordVector.begin() ; it != coordVector.end(); ++it)
    {
      CartesianBuilder c = *it;
      myfile<<endl<<"x "<<c.x1<<" y "<<c.y1<<" x2 "<<c.x2<<" y2 "<<c.y2;
    }*/
    //applyMagic(coordVector,img.clone());
    Mat temp = img;//.clone();//satya
    //printTime1("\nBefore magic ");

	for(int dist = 1; dist <= distance; dist = dist + dist % 5)
    {

      //if(coordVector.size())
      if(removeIntersections(coordVector,temp,dist))
      {
          break;
      }

    }
    //cout<<"\nbefore for "<<coordVector.size();
    printTime1("After Remove Intersections ");
    //myfile<<endl<<endl;

    int leftCount = 0,rightCount = 0,straightCount = 0,slightLeft = 0,slightRight = 0;
    string dir = "";//image text
    std::vector<CartesianBuilder> laneVector;
    for(int i = 0; i < coordVector.size(); i++)
    {
      CartesianBuilder c = coordVector[i];
      if(!c.isZero())
      {
        //myfile<<endl<<"x "<<c.x1<<" y "<<c.y1<<" x2 "<<c.x2<<" y2 "<<c.y2;
        if(c.validateLine(temp,R,G,B,lMode))//modify
        {
          laneVector.push_back(c);
          line( temp, Point(c.x1,c.y1), Point(c.x2,c.y2), Scalar(0,255,255), 5, CV_AA);
          direction eDir = c.checkSlope(/*dir*/);
          if(eDir == direction::left)
            leftCount++;
          else if(eDir == direction::right)
            rightCount++;
          else if(eDir == direction::slightRight)
            slightRight++;
          else if(eDir == direction::slightLeft)
            slightLeft++;
          else
            straightCount++;
        }
        else
        {
          //continue;
          line( temp, Point(c.x1,c.y1), Point(c.x2,c.y2), Scalar(0,100,255), 5, CV_AA);
        }
      }
    }

    /*direction d;//image text
    d = txtBuilder.checkSlope(dir);//image text
    //cout<<"slope "<<dir;*/
    //cout<<endl<<" right count "<<rightCount<<" slight right "<<slightRight<<" left count "<<leftCount<<" slight left "<<slightLeft<<" straight count "<<straightCount;


    dir = convertEnum(rightCount,slightRight,leftCount,slightLeft,straightCount);


    /*--------------------image text,rectangle-----------------------------------------*/
    putText(temp, "C-Eye v1.0", Point(10, 10), FONT_HERSHEY_PLAIN, 0.7/*font size*/, CV_RGB(153,0,153), 1.0);//image text
    rectangle(temp,cv::Point(5, 10),cv::Point(350, 100),cv::Scalar(230, 230, 230));//image rect
    putText(temp, "Direction " + dir, Point(10, 30), FONT_HERSHEY_PLAIN, 1.1/*font size*/, CV_RGB(10,255,0), 1.0);//image text

    char *strLaneCount = new char[5];
    bool confidenceLevel = false;
    int lnCount = laneCount(laneVector,confidenceLevel);
    /*if(lnCount > 1)
      lnCount--;*/
    sprintf(strLaneCount,"%d",lnCount);
    //string laneWarning = "";

    putText(temp, "Num of Lanes " + string(strLaneCount), Point(10, 50), FONT_HERSHEY_PLAIN, 1.1/*font size*/, CV_RGB(0,0,255), 1.0);//image text
    if(!confidenceLevel)
    {
      //laneWarning = "Due to low light conditions Lanes detected may not be accurate";

      putText(temp, "'Due to low light conditions", Point(10, 70), FONT_HERSHEY_PLAIN, 1.1/*font size*/, CV_RGB(255,0,0), 1.0);//lane warning
      putText(temp, "Lanes detected may not be accurate'", Point(10, 90), FONT_HERSHEY_PLAIN, 1.0/*font size*/, CV_RGB(255,0,0), 1.0);//lane warning
    }
    /*--------------------image text,rectangle-----------------------------------------*/


    //printTime1("Everything done,Before drawing lines ");
    //imshow("Init lines drawn Image", lineImg);//satya
    //namedWindow("Intersections applied",1);//satya
    //std::vector<CartesianBuilder> outputVector;
    vehVector = identifyVehicles(vehVector,15);
    vehVector = identifyVehicles(vehVector,30);
    //outputVector = identifyVehicles(outputVector,80);
    for (std::vector<CartesianBuilder>::iterator it = vehVector.begin() ; it != vehVector.end(); ++it)
    {
      CartesianBuilder log = *it;
      if(log.slope() > 5)
      line( temp,Point(log.x1,log.y1), Point(log.x2,log.y2), Scalar(255,0,255), 0.5, CV_AA);

    }
    //imshow("Intersections applied", temp);//uncomment to view image
    //printTime1("After drawing lines ");
    //waitKey(45);
    //printTime1();
    //myfile.close();
    return true;

}
std::vector<CartesianBuilder> LaneSwitcher::identifyVehicles(std::vector<CartesianBuilder> &vehVector,int distance)
{
  std::vector<CartesianBuilder> outputVector;
  ofstream myfile;
  myfile.open ("C:/Users/tej/Desktop/Test_19Sep/log/vehicle.log");


  //clubbing of lines
  //cout<<endl<<"identifyVehicles "<<vehVector.size();
  std::sort(vehVector.begin(),vehVector.end(),distanceFromX);
  CartesianBuilder first(0,0,0,0),next(0,0,0,0),temp(0,0,0,0);
  int index = 0;
  for (std::vector<CartesianBuilder>::iterator it = vehVector.begin() ; it != vehVector.end(); ++it)
  {

    CartesianBuilder log = *it;
    /*if(log.x1 > 250 && log.x1 < 350 )
      continue;
    line( img,Point(log.x1,log.y1), Point(log.x2,log.y2), Scalar(255,index+10,255), 0.5, CV_AA);
    continue;*/
    myfile<<endl<<"coord "<<log.x1<<" "<<log.y1<<":"<<log.x2<<" "<<log.y2;

    if(!first.x1)
    {
      temp = first = *it;
    }
    else if(!next.x1)
    {
      next = *it;
    }
    //else
    if(first.x1 && next.x1)
    {
      if(abs(first.x1-next.x1) < distance && abs(first.y1-next.y1) < distance)
      {
        //myfile<<endl<<"appending";
        temp.x2 = next.x2;
        temp.y2 = next.y2;
        myfile<<endl<<"appending coord:"<<temp.x1<<" "<<temp.y1<<":"<<temp.x2<<" "<<temp.y2;
        //vehVector.erase(vehVector.begin()+index);
        vehVector[index] = temp;
        next.reset();

        //temp.reset();
      }
      else
      {
        myfile<<endl<<"reseting";
        //myfile<<endl<<"first:"<<first.x1<<" "<<first.y1<<":"<<first.x2<<" "<<first.y2;
        //myfile<<endl<<"next :"<<next.x1<<" "<<next.y1<<":"<<next.x2<<" "<<next.y2;
        if(temp.x1)
        {
          //myfile<<endl<<"drawing";
          myfile<<endl<<"drawing coord:"<<temp.x1<<" "<<temp.y1<<":"<<temp.x2<<" "<<temp.y2;
          //line( img,Point(temp.x1,temp.y1), Point(temp.x2,temp.y2), Scalar(255,index+10,255), 0.5, CV_AA);
          outputVector.push_back(temp);
        }
        first.reset();
        //first = next;
        next.reset();
        temp.reset();
        //myfile<<endl<<"first post:"<<first.x1<<" "<<first.y1<<":"<<first.x2<<" "<<first.y2;
      }
    }
    index++;
  }
  myfile.close();
  return outputVector;
}
int LaneSwitcher::laneCount(std::vector<CartesianBuilder> &laneVector,bool &isConfidence)
{
    //cout<<endl<<"entering with size "<<laneVector.size();
    for(int i = 0 ; i < laneVector.size(); i++)
    {
      CartesianBuilder temp1 = laneVector[i];
      for(int j = 0 ; j < laneVector.size(); j++)
      {
        CartesianBuilder temp2 = laneVector[j];

        if(temp2.isZero() || temp1.areEqual(temp2))
          continue;

        if(temp1.checkYCoordCompatibility(temp2,50,true))
        {
          //temp1.x1 = temp1.y1 = temp1.x2 = temp1.y2 = 0;
          temp1.reset();
          //cout<<endl<<"lane count removal";
          if(i > 0)
            laneVector.erase (laneVector.begin()+i-1);
          else
            laneVector.erase (laneVector.begin());
        }
      }
    }
    //cout<<endl<<"end size "<<laneVector.size();
    int laneSize = laneVector.size();
    if(laneSize > 2)
    {
      isConfidence = true;
      laneSize--;
    }
    return laneSize;
    //return laneVector.size();
}
string LaneSwitcher::convertEnum(int right,int slightRight,int left,int slightLeft,int straight)
{
  if(right > left && right > straight && right > slightRight && right > slightLeft)
    return "Right";
  else if(left > right && left > straight && left > slightRight && left > slightLeft)
    return "Left";
  else if(slightRight > left && slightRight > right && slightRight > straight && slightRight > slightLeft)
    return "Slight Right";
  else if(slightLeft > left && slightLeft > right && slightLeft > straight && slightLeft > slightRight)
    return "Slight Left";
  else
    return "straight";

}
bool LaneSwitcher::removeIntersections(std::vector<CartesianBuilder> &final,Mat &img,int distance)
{
  //ofstream myfile;
  //myfile.open ("C:/Users/tej/Desktop/Test_19Sep/log/removal.log");
  //myfile<<endl<<"size before "<<final.size();
  std::sort(final.begin(),final.end(),distanceFromX);
  bool isFirst = true;
  CartesianBuilder tempC;
  /*for(int i = 0; i < final.size(); i++)
  {
    CartesianBuilder c = final[i];
    myfile<<endl<<"x1 "<<c.x1<<" y1 "<<c.y1<<" x2 "<<c.x2<<" y2 "<<c.y2;
  }*/
  CartesianBuilder distCheck;//distance check
  for(int i = 0; i < final.size(); i++)
  {
    CartesianBuilder cI = final[i];
    if(cI.isZero())
    {
      final.erase (final.begin()+i);
      continue;
    }
    //else if(cI.checkHorLines() ||
    else if(cI.removeHorizontalLines())/*satya dec 15 dec 17 */
    {
      //cout<<endl<<"remove lines";
      //myfile<<endl<<"x1 "<<cI.x1<<" y1 "<<cI.y1<<" x2 "<<cI.x2<<" y2 "<<cI.y2;
      line( img, Point(cI.x1,cI.y1), Point(cI.x2,cI.y2), Scalar(10,10,0), 5, CV_AA);
      cout<<endl<<"x1 "<<cI.x1<<" y1 "<<cI.y1<<" x2 "<<cI.x2<<" y2 "<<cI.y2;
      final.erase (final.begin()+i);
      continue;
    }
    for(int j = i; j < final.size(); j++)
    {
        CartesianBuilder cJ = final[j];
        if(i == j || cJ.isZero())
          continue;
        //myfile<<endl<<"x1 "<<cI.x1<<" y1 "<<cI.y1<<" x2 "<<cI.x2<<" y2 "<<cI.y2;
        //myfile<<endl<<"x1 "<<cJ.x1<<" y1 "<<cJ.y1<<" x2 "<<cJ.x2<<" y2 "<<cJ.y2;
        float dist = sqrt(pow((cI.x2 - cJ.x1),2) + pow((cI.y2 - cJ.y1),2));

        float dist1 = sqrt(pow((cI.x1 - cJ.x2),2) + pow((cI.y1 - cJ.y2),2));

        float dist2 = sqrt(pow((cI.x2 - cJ.x2),2) + pow((cI.y2 - cJ.y2),2));

        //myfile<<endl<<"dist "<<dist<<"dist1 "<<dist1<<"dist2 "<<dist2<<" distance "<<distance<<" check comp "<<cI.checkCompatibility(cJ,distance);

        if((dist < distance || dist1 < distance || dist2 < distance) && cI.checkCompatibility(cJ,distance) && cI.checkCompatibility(cJ,30))
        {
            //dec 15 start
            CartesianBuilder temp = final[i];
            CartesianBuilder temp1 = final[j];
            CartesianBuilder *temp2 = new CartesianBuilder(final[i].x1,final[i].y1,cJ.x2,cJ.y2);
            float p2pDist = temp.calcDistance() + temp1.calcDistance();
            //if(1.2*p2pDist <= temp2->calcDistance())
              //continue;

            //checkSlope(temp,temp1,*temp2);
            //dec 15 end
            final[i].x2 = cJ.x2;
            final[i].y2 = cJ.y2;

            //myfile<<endl<<"reset "<<final[j].x1<<" "<<final[j].y1<<" "<<final[j].x2<<" "<<final[j].y2;
            final[j].reset();
            //myfile<<endl<<"merging "<<final[i].x1<<" "<<final[i].y1<<" "<<final[i].x2<<" "<<final[i].y2;
        }
    }
  }
  //myfile<<endl<<"final size "<<final.size();
  //myfile<<endl<<"final coord";
  /*for(int i = 0; i < final.size(); i++)
  {
    CartesianBuilder c = final[i];
    myfile<<endl<<"x1 "<<c.x1<<" y1 "<<c.y1<<" x2 "<<c.x2<<" y2 "<<c.y2;
  }*/
  if(final.size() < 10)
  {
     for(int i = 0; i < final.size(); i++)
     {
       CartesianBuilder c = final[i];
       if(c.calcDistance() < 10)
         final.erase (final.begin()+i);
     }
  }

  if(final.size() < 8)
    return true;
  return false;
  /*for(int i = 0; i < final.size(); i++)
  {
    CartesianBuilder c = final[i];
    //if(c.checkSlope())
      line( img, Point(c.x1,c.y1), Point(c.x2,c.y2), Scalar(100,10,0), 5, CV_AA);
  }*/
  //myfile.close();
  //imshow("Magic Applied", img);

}
/*bool LaneSwitcher::checkSlope(CartesianBuilder temp,CartesianBuilder temp1,CartesianBuilder temp2)
{

}*/
bool LaneSwitcher::initiateLaneDetection(Mat &src)
{
  //cout<<"\ninitiateLaneDetection";
  //Mat src1 = src.clone();
  bool retVal = customLaneDetect(src);
  return retVal;

  //imshow("Original",src);//satya
}
