#include "helpers.h"

class DetectTrafficSignal
{
  public:
  int iLowH, iHighH, iLowS, iHighS, iLowV, iHighV;
  DetectTrafficSignal();
  DetectTrafficSignal(int,int,int,int,int,int);
  vector<Moments> findCentres(Mat imgThresholded,vector<Point2f> &points);
  Mat findSignalContours(Mat imgOriginal);
  bool checkForRedSignal(vector<Moments>&,vector<Moments>&);
};
