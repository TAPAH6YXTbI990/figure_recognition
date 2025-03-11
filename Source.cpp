#include <opencv2/opencv.hpp>
#include <iostream>
#include <locale>
#include <codecvt>

using namespace cv;
using namespace std;

string shapeFinder(const vector<Point>& contour)
{
    vector<Point> approx;
    double epsilon = 0.01 * arcLength(contour, true);
    approxPolyDP(contour, approx, epsilon, true);

    if (approx.size() <= 3)
    {
        return "triangle";
    }
    else if (approx.size() == 4)
    {
        Rect rect = boundingRect(approx);
        return "rectangle";
    }
    else if (approx.size() > 4)
    {
        double area = contourArea(contour);
        double perimeter = arcLength(contour, true);
        double circularity = 4 * CV_PI * area / (perimeter * perimeter);
        if (circularity > 0.5)
        {
            return "circle";
        }
    }

    return "???";
}

int main()
{
    system("chcp 65001 > nul");

    string image_path = "C:/Users/User/.vscode/projects/figures.png"; 
   
    Mat img = imread(image_path);

    if (img.empty())
    {
        cerr << "Error when uploading the image!" << endl;
        return -1; 
    }
    
    Mat gray;
    cvtColor(img, gray, COLOR_BGR2GRAY); 

    Mat binary;
    threshold(gray, binary, 10, 50, THRESH_BINARY);

    vector<vector<Point>> contours;
    findContours(binary, contours, RETR_TREE, CHAIN_APPROX_SIMPLE);
    
    Mat contourOutput = gray.clone();
    cvtColor(contourOutput, contourOutput, COLOR_GRAY2BGR);
    for (size_t i = 0; i < contours.size(); i++)
    {
        double area = contourArea(contours[i]);
        if (area < 100)
            continue;

        string shape = shapeFinder(contours[i]);

        drawContours(contourOutput, contours, i, Scalar(255, 0, 0), 2);

        Moments M = moments(contours[i]);
        Point center(M.m10 / M.m00, M.m01 / M.m00);
        putText(contourOutput, shape, center, FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 0, 0), 2);
    }
    
    // imshow("Original", img);
    // imshow("Edges (threshold)", binary);
    imshow("Contours", contourOutput);
    waitKey(0);
    return 0;
}
