#include "CartesianBuilder.h"
#include "helpers.h"
CartesianBuilder::CartesianBuilder(int a,int b,int c,int d,int rows,int brdth)
{
        x1 = a;
        y1 = b;
        x2 = c;
        y2 = d;
        CarRows = rows;
        breadth = brdth;
}
CartesianBuilder::CartesianBuilder(int a,int b)
{
        x1 = a;
        y1 = b;
}
CartesianBuilder::CartesianBuilder()
{

}
CartesianBuilder::~CartesianBuilder()
{
    //dtor
}
void CartesianBuilder::reset()
{
  this->x1 = this->y1 = this->x2 = this->y2 = 0;
}
float CartesianBuilder::calcDistance()
{
    return sqrt(pow((this->x2 - this->x1),2) + pow((this->y2 - this->y1),2));
}
bool CartesianBuilder::isZero()
{
  if(x1 == 0 && y1 == 0 && x2 == 0 && y2 == 0)
    return true;
  return false;
}

direction CartesianBuilder::checkSlope(/*string &dir*/)
{
  direction d;
  float x1,y1,x2,y2;
  //dir = "Straight";
  //cout<<endl<<"slope value "<<yCord/xCord<<" x1 "<<this->x1<<" y1 "<<this->y1<<" x2 "<<this->x2<<" y2 "<<this->y2;
  if(this->y1 < this->y2)
  {
     x1 = this->x1;
     y1 = this->y1;
     x2 = this->x2;
     y2 = this->y2;
  }
  else
  {
     x2 = this->x1;
     y2 = this->y1;
     x1 = this->x2;
     y1 = this->y2;
  }
  float xCord = x2 - x1;
  float yCord = y2 - y1;
  //cout<<endl<<"slope value "<<yCord/xCord<<" x1 "<<x1<<" y1 "<<y1<<" x2 "<<x2<<" y2 "<<y2;
  if(xCord * yCord == 0)
  {
    //cout<<"\nstraight";
    return direction::straight;
  }
  else if((xCord /** yCord*/) < 0)
  {
    //cout<<"\nright";
    if(x1 > 1.1*x2 /*&& y1 > 1.05*y2*/)
    {
      //dir = "Right";
      return direction::right;
    }
    else
    {
       //dir = "Slight Right";
       return direction::straight;
    }
  }
  else
  {
    //cout<<"\nleft";
    if(x1 < 1.1*x2 /*&& y1 < 1.05*y2*/)
    {
      //dir = "Left";
      return direction::left;
    }
    else
    {
       //dir = "Slight Left";
       return direction::straight;
    }
  }
}
bool CartesianBuilder::removeHorizontalLines()
{
  //cout<<endl<<"removeHorizontalLines";
  int x1,y1,x2,y2;
  if(this->y2 < this->y1)
  {
    x1 = this->x2;
    y1 = this->y2;
    x2 = this->x1;
    y2 = this->y1;
  }
  else
  {
    x1 = this->x1;
    y1 = this->y1;
    x2 = this->x2;
    y2 = this->y2;
  }

  float yCoord = abs(y2-y1);
  float xCoord = abs(x2-x1);
  //cout<<endl<<"hor x1 "<<x1<<" y1 "<<y1<<" x2 "<<x2<<" y2 "<<y2<<" xcoord "<<xCoord<<" yCoord "<<yCoord<<" y/x "<<yCoord/xCoord;
  if(yCoord/xCoord < 0.3 && xCoord/4 > yCoord && yCoord > 50)
  {
      cout<<endl<<"\nReturning false"<<endl;
      return true;
  }
  return false;
}
bool CartesianBuilder::validateLine(Mat &img,int R,int G,int B,int mode)
{
  //cout<<endl;
  int x1,y1,x2,y2;
  if(this->y2 < this->y1)
  {
    x1 = this->x2;
    y1 = this->y2;
    x2 = this->x1;
    y2 = this->y1;
  }
  else
  {
    x1 = this->x1;
    y1 = this->y1;
    x2 = this->x2;
    y2 = this->y2;
  }

  float yCoord = abs(y2-y1);
  float xCoord = abs(x2-x1);
  //cout<<endl<<"x1 "<<x1<<" y1 "<<y1<<" x2 "<<x2<<" y2 "<<y2<<" xcoord "<<xCoord<<" yCoord "<<yCoord<<" y/x "<<yCoord/xCoord;
  //bool a = yCoord/xCoord < 0.3;
  //bool b = xCoord/4 > yCoord;
  //cout<<endl<<"bool "<<a<<" "<<b;
  if(yCoord/xCoord < 0.3 && xCoord/4 > yCoord)
  {
      //cout<<endl<<"\nReturning false"<<endl;
      return false;
  }
  Vec3b colorAbove,colorBelow,colorMidPnt;
  int isBlack = 0;
  int tempX1 = (x1+x2)/2;
  int tempY1 = (y1+y2)/2;
  colorMidPnt = img.at<Vec3b>(Point(x1,y1));
  //cout<<endl<<"mid "<<(int)colorMidPnt[0]<<" "<<(int)colorMidPnt[1]<<" "<<(int)colorMidPnt[2];

  if(tempX1 - 10 > 0)

  {
    colorAbove = img.at<Vec3b>(Point(tempX1-10,tempY1));
    //cout<<endl<<"rows "<<img.rows<<"cols "<<img.cols;
    //cout<<endl<<"exp "<<(int)colorAbove[0]<<" "<<(int)colorAbove[1]<<" "<<(int)colorAbove[2];

    if((int)colorAbove[0] <= 170 && (int)colorAbove[1] <= 170 && (int)colorAbove[2] <= 170/* &&
        ((int)colorMidPnt[0] >= R && (int)colorMidPnt[1] >= G && (int)colorMidPnt[2] >= B)*/)
    {
      //cout<<"'13'";
      isBlack++;
    }
  }
  else
  {
    //cout<<"'14'";
    //isBlack++;
  }
  if(tempX1 + 10 < img.cols)
  {
    colorAbove = img.at<Vec3b>(Point(tempX1+10,y2));
    //cout<<"\nexp2 "<<(int)colorAbove[0]<<" "<<(int)colorAbove[1]<<" "<<(int)colorAbove[2];

    if((int)colorAbove[0] <= 170 && (int)colorAbove[1] <= 170 && (int)colorAbove[2] <= 170/* &&
        ((int)colorMidPnt[0] >= R && (int)colorMidPnt[1] >= G && (int)colorMidPnt[2] >= B)*/)
    {
      //cout<<"'15'";
      isBlack++;
    }
  }
  else
  {
    //cout<<"'16'";
    //isBlack++;
  }
  //cout<<endl<<"black"<<isBlack<<" abs x "<<abs(x1-x2);
  if(isBlack >= 1 && abs(x1-x2) <= img.cols/2)
  {
    //cout<<endl;
    return true;
  }
  //cout<<endl<<"removed"<<endl;
  return false;
}
bool CartesianBuilder::checkYCoordCompatibility(CartesianBuilder &cJ,int distance, bool isXCoord)
{
  //cout<<endl<<"compa x1 "<<this->x1 <<" y1 "<< this->y1 <<" x2 "<< this->x2 <<" y2 "<< this->y2;
  //cout<<endl<<"other end x1 "<<cJ.x1 <<" y1 "<< cJ.y1 <<" x2 "<< cJ.x2 <<" y2 "<< cJ.y2;
  CartesianBuilder *c1,*c2;
  if((this->y1 < cJ.y1 && this->y2 < cJ.y1) && (this->y1 < cJ.y2 && this->y2 < cJ.y2))
  {
    //cout<<"\n1";
    c1 = this;
    c2 = &cJ;
  }
  else if((this->y1 > cJ.y1 && this->y2 > cJ.y1) && (this->y1 > cJ.y2 && this->y2 > cJ.y2))
  {
    c2 = this;
    c1 = &cJ;
    //cout<<"\n2";
  }
  else
  {
    //cout<<endl<<"done";
    return false;
  }


  int x1 = -1,y1 = -1,x2 = -1,y2 = -1;
  if(c1->y1 < c1->y2)
  {
    x1 = c1->x2;
    y1 = c1->y2;
  }
  else
  {
    x1 = c1->x1;
    y1 = c1->y1;
  }
  if(c2->y1 > c2->y2)
  {
    x2 = c2->x2;
    y2 = c2->y2;
  }
  else
  {
    x2 = c2->x1;
    y2 = c2->y1;
  }

  //cout<<endl;
  bool xCoord = abs(x1 - x2) < 2*distance;
  bool yCoord = abs(y1 - y2) < 3*distance && abs(y1 - y2) > 10;
  //cout<<endl<<"after "<<"x1 "<<x1<<"y1 "<<y1<<"x2 "<<x2<<"y2 "<<y2<<" abs-x "<<abs(x1 - x2)<<" abs-y "<<abs(y1 - y2)<<" dist "<<distance;
  //cout<<endl<<"bool x "<<xCoord<<" bool y "<<yCoord<<" done"<<endl;
  if(!isXCoord)
  {
    return xCoord;
  }
  else
  {
    return xCoord || (xCoord && yCoord);
  }
  return false;
}
bool CartesianBuilder::areEqual(CartesianBuilder &cJ)
{
  if(this->x1 == cJ.x1 &&
     this->x2 == cJ.x2 &&
     this->y1 == cJ.y1 &&
     this->y2 == cJ.y2)
       return true;
  return false;
}
float CartesianBuilder::slope()
{
  //return /*sqrt*/(pow(this->y2-this->y1,2)+pow(this->x1-this->x1,2));
    return abs(this->x2-this->x1);
}
//satya changes to remove hori lines dec 12
bool CartesianBuilder::checkHorLines()
{
  if(abs(this->y2-this->y1) < 3)
  {
     if(abs(this->x2-this->x1) < 5)
       return false;
     return true;
  }
  return false;
}
bool CartesianBuilder::checkCompatibility(CartesianBuilder &cJ,int distance)
{
  //cout<<endl<<"checkCompatibility";
  CartesianBuilder *cI = this;
  float distX1 = abs(cI->x1 - cJ.x1);
  float distX2 = abs(cI->x2 - cJ.x2);
  float distX3 = abs(cI->x2 - cJ.x1);

  bool slopeCheckP1 = false;
  bool slopeCheckP2 = false;

  if((cI->x2 - cI->x1 != 0) && ((cI->y2 - cI->y1)/(cI->x2 - cI->x1)) > 0)
    slopeCheckP1 = true;

  if((cJ.x2 - cJ.x1 != 0) && ((cJ.y2 - cJ.y1)/(cJ.x2 - cJ.x1)) > 0)
    slopeCheckP2 = true;

  /*bool slopeCheck = false;
  if(distX1 < CarRows/2.5 && distX2 < CarRows/2.5 && distX3 < CarRows/2.5)
    slopeCheck = true;*/

  if((distX1 <= distance && distX2 <= distance) || distX3 <= distance)
    return true;
  //cout<<endl<<"false for cI "<<cI->x1<<":"<<cI->y1<<":"<<cI->x2<<":"<<cI->y2;
  //cout<<endl<<"false for cJ "<<cJ.x1<<":"<<cJ.y1<<":"<<cJ.x2<<":"<<cJ.y2<<endl;
  return false;
}
