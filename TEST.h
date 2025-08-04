#pragma once

#include <opencv2/opencv.hpp> 
#include <vector> 

#ifdef TEST_EXPORTS
#define API __declspec(dllexport)
#else
#define API __declspec(dllimport)
#endif

extern "C" API bool RecognizeDominantColorHSV(cv::Mat *image,
                                              double *hsv_color);

extern "C" API bool FindColorRegionHSV(
    cv::Mat *image, const double *target_hsv, const double *tolerance_hsv,
    cv::Mat *mask, std::vector<std::vector<cv::Point>> *contours = nullptr);


extern "C" API bool SimpleOCR(cv::Mat *image, char *out_text, int *max_length);
