#ifndef CARTESIANBUILDER_H
#define CARTESIANBUILDER_H
#include <math.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include<iostream>
#include<string>

using namespace cv;
using namespace std;
enum direction { undef,left,slightLeft,right,slightRight,straight};
class CartesianBuilder
{
    public:
    int x1,y1,x2,y2;
    int breadth;
    int CarRows;
    CartesianBuilder();
    CartesianBuilder(int a,int b,int c,int d,int rows = 0,int brdth = 0);
    CartesianBuilder(int a,int b);
    bool checkYCoordCompatibility(CartesianBuilder &cJ,int distance,bool isXCoord = false);
    float calcDistance();
    void reset();
    bool removeHorizontalLines();
    bool areEqual(CartesianBuilder &cJ);
    void copyExtremes(CartesianBuilder);
    bool checkCompatibility(CartesianBuilder &cI,int distance);
    bool isZero();
    direction checkSlope(/*string &*/);
    virtual ~CartesianBuilder();
    bool checkSlopeCompatibility(CartesianBuilder &cJ);
    bool validateLine(Mat &img,int R,int G,int B,int mode);
    float slope();
    bool checkHorLines();//satya changes to remove hori lines dec 12
};
#endif

