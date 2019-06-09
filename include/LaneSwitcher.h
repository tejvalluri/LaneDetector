#include "helpers.h"
#include "CartesianBuilder.h"
#include "ObjectDetection.h"
class LaneSwitcher
{
    public:
    int R,G,B;
    int length;
    int lMode;
    int distance;
    std::vector<CartesianBuilder> identifyVehicles(std::vector<CartesianBuilder> &vehVector,int distance);//,Mat &img,std::vector<CartesianBuilder> &outputVector);
    //bool lookUp[864][482];
    ObjectDetection checkForObject(Mat &,cv::Vec3b* prevPixel,int i,int j);
    LaneSwitcher(int mode,int len,int distance);
    //bool checkSlope(CartesianBuilder temp,CartesianBuilder temp1,CartesianBuilder temp2);
    bool initiateLaneDetection(Mat &img);
    Mat performCannyEdgeDetetion(Mat &img);
    Mat performGrayScaleConversion(Mat &img);
    vector<Vec4i> performHoughLineTransformation(Mat &img);
    int laneCount(std::vector<CartesianBuilder> &laneVector,bool &isConfidence);
    string convertEnum(int right,int slightRight,int left,int slightLeft,int straight);
    bool customLaneDetect(Mat &img);
    bool detectLanes(vector<Vec4i> &,Mat &img);
    void applyMagic(std::vector<CartesianBuilder> &coordVector,Mat img);
    bool removeIntersections(std::vector<CartesianBuilder> &coordVector,Mat &img,int distance);
};
