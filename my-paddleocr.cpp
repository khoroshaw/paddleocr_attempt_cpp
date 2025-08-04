#include <iostream>
#include <opencv2/opencv.hpp>

#include "paddle_util.h"

bool RecognizeDominantColorHSV(cv::Mat *image, double *hsv_color) {
  /* 以一张照片中的所有像素点为样本，统计最多的Hue作为输出H；S和V则是所有像素点的均值
   */

  // 检查输入图像是否为空或者格式不正确
  if (!image || image->empty() || image->type() != CV_8UC3) {
    std::cerr
        << "Invalid input image. Ensure it is a non-empty BGR color image."
        << std::endl;
    return false;
  }

  // 转换图像到HSV空间，HSV三个维度分别表示色调(Hue)、饱和度(Saturation)和明度(Value)
  cv::Mat hsvImage;
  cv::cvtColor(*image, hsvImage, cv::COLOR_BGR2HSV);

  // 计算直方图以找到主颜色
  int h_bins =
      180; // H通道理论范围为[0,
           // 360)，红绿蓝对应0,120,240度；OpenCV中为适配8bit储存范围，将区间设定为[0,
           // 180)，每刻度代表2度
  int histSize[] = {h_bins};
  float h_ranges[] = {0, 180};
  const float *ranges[] = {
      h_ranges};        // 每个维度的上下界，这里只考虑H通道，故只有一个范围
  int channels[] = {0}; // 只计算H通道，H的索引为0
  cv::Mat hist;

  cv::calcHist(&hsvImage, 1, channels, cv::Mat(), hist, 1, histSize, ranges,
               true, false);
  cv::normalize(hist, hist, 0, 255, cv::NORM_MINMAX);

  // 找到直方图中最大值对应的H通道值
  double maxVal = 0;
  int dominantHue = 0;
  for (int i = 0; i < h_bins; i++) {
    float binVal = hist.at<float>(i);
    if (binVal > maxVal) {
      maxVal = binVal;
      dominantHue = i;
    }
  }

  // 计算 S 和 V 的平均值
  cv::Scalar meanValues = cv::mean(hsvImage);
  double dominantSaturation = meanValues[1];
  double dominantValue = meanValues[2];

  // 输出主颜色的 HSV 值
  hsv_color[0] = dominantHue;        // H
  hsv_color[1] = dominantSaturation; // S
  hsv_color[2] = dominantValue;      // V

  return true;
}

bool FindColorRegionHSV(
    cv::Mat *image, const double *target_hsv, const double *tolerance_hsv,
    cv::Mat *mask, std::vector<std::vector<cv::Point>> *contours = nullptr) {
  // 红色：H: 4, S: 154.402, V: 78.1397

  // 检查输入图像是否为空或者格式不正确
  if (!image || image->empty() || image->type() != CV_8UC3) {
    std::cerr
        << "Invalid input image. Ensure it is a non-empty BGR color image."
        << std::endl;
    return false;
  }

  int rows = image->rows; // y-axis
  int cols = image->cols; // x-axis
  cv::Mat hsvImage;
  cv::cvtColor(*image, hsvImage, cv::COLOR_BGR2HSV);

  // 遍历每个像素点
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      // 获取当前像素点的HSV值
      cv::Vec3b pixel = hsvImage.at<cv::Vec3b>(i, j);
      double h = pixel[0];
      double s = pixel[1];
      double v = pixel[2];

      // 检查当前像素点是否在目标颜色范围内
      if (h >= target_hsv[0] - tolerance_hsv[0] &&
          h <= target_hsv[0] + tolerance_hsv[0] &&
          s >= target_hsv[1] - tolerance_hsv[1] &&
          s <= target_hsv[1] + tolerance_hsv[1] &&
          v >= target_hsv[2] - tolerance_hsv[2] &&
          v <= target_hsv[2] + tolerance_hsv[2]) {
        // 将符合条件的像素点标记为白色
        mask->at<uchar>(i, j) = 255;
      } else {
        // 将不符合条件的像素点标记为黑色
        mask->at<uchar>(i, j) = 0;
      }
    }
  }

  // 如果需要，查找轮廓
  if (contours) {
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(*mask, *contours, hierarchy, cv::RETR_EXTERNAL,
                     cv::CHAIN_APPROX_SIMPLE);
  }

  // 保存结果
  //cv::imwrite("Masks.jpg", *mask);
  //cv::waitKey(0);
  // if (contours)
  //{
  //     cv::imwrite("Contours.jpg", *contours);
  //     cv::waitKey(0);
  // }
  return true;
}

bool SimpleOCR(cv::Mat *image, char *out_text, int *max_length) {
  // 调用PaddleOCR，实现简单的OCR功能
  PaddleUtil::init();
  //std::cout << "PaddleOCR initialized.\n";
  //std::cout << "------------------------------------------------------"
  //          << std::endl;
  PaddleUtil::get().rec_image(image, out_text, max_length);
  //std::cout << "Done!\n";

  return true;
}

int main() {
  // 读取图像
  cv::Mat image = cv::imread("D:/MyFiles/SupconCup/pics/1.jpg");
  if (image.empty()) {
    std::cerr << "Could not open or find the image!" << std::endl;
    return -1;
  }

  // 定义 HSV 颜色数组
  double hsv_color[3];

  // 调用函数识别主颜色
  if (RecognizeDominantColorHSV(&image, hsv_color)) {
    std::cout << "Dominant Color in HSV: "
              << "H: " << hsv_color[0] << ", "
              << "S: " << hsv_color[1] << ", "
              << "V: " << hsv_color[2] << std::endl;
  } else {
    std::cerr << "Failed to recognize dominant color." << std::endl;
  }

  const double target_hsv[3] = {hsv_color[0], hsv_color[1], hsv_color[2]};
  const double tolerance_hsv[3] = {3, 60, 60}; // 可修改，甚至是调参的重要对象
  cv::Mat mask = cv::Mat::zeros(image.size(), CV_8U);
  std::vector<std::vector<cv::Point>> contours;

  FindColorRegionHSV(&image, target_hsv, tolerance_hsv, &mask, &contours);

  cv::Mat word_img = cv::imread("D:/MyFiles/SupconCup/pics/words.jpg");
  if (word_img.empty()) {
    std::cerr << "Could not open or find the word image!" << std::endl;
    return -1;
  }
  char out_text[256] = {0};              // 输出文本缓冲区
  int max_length = sizeof(out_text) - 1; // 最大长度

  SimpleOCR(&word_img, out_text, &max_length);
  std::cout << "Recognized Text: " << out_text << std::endl;

  return 0;
}
