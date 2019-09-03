#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"

#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char *argv[]) {
    if(argc<2)
    {
        cout<<"Укажите путь к картинке в качестве параметра";
        exit(0);
    }
  Mat src, src_gray, dst;
  int kernel_size = 3;
  src = imread(argv[1]);
  cvtColor(src, src_gray, cv::COLOR_BGR2GRAY);

  cv::Mat res = src.clone();

  blur(src_gray, src_gray, Size(3, 3));

  //-------Canny----------------------------
  int lowThreshold = 30;
  int ratio = 1;
  Mat abs_dst;

  Canny(src_gray, abs_dst, 15, 25, kernel_size, true);

  //-----Dilate--------------------------------
  int dilation_size = 2;
  Mat element = getStructuringElement(
      MORPH_RECT, Size(2 * dilation_size + 1, 2 * dilation_size + 1),
      Point(dilation_size, dilation_size));
  dilate(abs_dst, abs_dst, element);

  //-------Threshold----------------------------
  double thresh = 90;
  cv::Mat thresholded;
  cv::threshold(abs_dst, thresholded, thresh, 255, THRESH_BINARY);

  //----Inverse------------------------------------
  cv::Mat hough_long_res = thresholded.clone();
  cv::threshold(hough_long_res, hough_long_res, 128, 255,
                cv::THRESH_BINARY_INV);
  int border = 5;
  cv::Mat roi = hough_long_res(
      Rect(border, border, src.cols - 2 * border, src.rows - 2 * border));
  cv::copyMakeBorder(roi, hough_long_res, border, border, border, border,
                     BORDER_ISOLATED, 0);



  //----FindContoursConvexHull-------------------------------
  std::vector<std::vector<cv::Point>> contours_for_approx;
  cv::findContours(hough_long_res, contours_for_approx, cv::RETR_TREE,
                   cv::CHAIN_APPROX_NONE);

  int true_contour_area = 1500;
  for (size_t i = 0; i < contours_for_approx.size(); ++i) {
    if (cv::contourArea(contours_for_approx[i]) > true_contour_area) {

      vector<vector<Point>> hull(1);
      cv::convexHull(contours_for_approx[i], hull[0]);

      drawContours(res, hull, 0, Scalar(255, 0, 0), 2);
    }
  }

  cv::imshow("res.jpg", res);

  waitKey(0);

  return 0;
}
