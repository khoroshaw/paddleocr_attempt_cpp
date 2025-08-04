#include <iostream>
#include "TEST.h"
#include "paddle_util.h"

bool RecognizeDominantColorHSV(cv::Mat *image, double *hsv_color) {
  /* ��һ����Ƭ�е��������ص�Ϊ������ͳ������Hue��Ϊ���H��S��V�����������ص�ľ�ֵ
   */

  // �������ͼ���Ƿ�Ϊ�ջ��߸�ʽ����ȷ
  if (!image || image->empty() || image->type() != CV_8UC3) {
    std::cerr
        << "Invalid input image. Ensure it is a non-empty BGR color image."
        << std::endl;
    return false;
  }

  // ת��ͼ��HSV�ռ䣬HSV����ά�ȷֱ��ʾɫ��(Hue)�����Ͷ�(Saturation)������(Value)
  cv::Mat hsvImage;
  cv::cvtColor(*image, hsvImage, cv::COLOR_BGR2HSV);

  // ����ֱ��ͼ���ҵ�����ɫ
  int h_bins =
      180; // Hͨ�����۷�ΧΪ[0,
           // 360)����������Ӧ0,120,240�ȣ�OpenCV��Ϊ����8bit���淶Χ���������趨Ϊ[0,
           // 180)��ÿ�̶ȴ���2��
  int histSize[] = {h_bins};
  float h_ranges[] = {0, 180};
  const float *ranges[] = {
      h_ranges};        // ÿ��ά�ȵ����½磬����ֻ����Hͨ������ֻ��һ����Χ
  int channels[] = {0}; // ֻ����Hͨ����H������Ϊ0
  cv::Mat hist;

  cv::calcHist(&hsvImage, 1, channels, cv::Mat(), hist, 1, histSize, ranges,
               true, false);
  cv::normalize(hist, hist, 0, 255, cv::NORM_MINMAX);

  // �ҵ�ֱ��ͼ�����ֵ��Ӧ��Hͨ��ֵ
  double maxVal = 0;
  int dominantHue = 0;
  for (int i = 0; i < h_bins; i++) {
    float binVal = hist.at<float>(i);
    if (binVal > maxVal) {
      maxVal = binVal;
      dominantHue = i;
    }
  }

  // ���� S �� V ��ƽ��ֵ
  cv::Scalar meanValues = cv::mean(hsvImage);
  double dominantSaturation = meanValues[1];
  double dominantValue = meanValues[2];

  // �������ɫ�� HSV ֵ
  hsv_color[0] = dominantHue;        // H
  hsv_color[1] = dominantSaturation; // S
  hsv_color[2] = dominantValue;      // V

  return true;
}

bool FindColorRegionHSV(
    cv::Mat *image, const double *target_hsv, const double *tolerance_hsv,
    cv::Mat *mask, std::vector<std::vector<cv::Point>> *contours = nullptr) {
  // ��ɫ��H: 4, S: 154.402, V: 78.1397

  // �������ͼ���Ƿ�Ϊ�ջ��߸�ʽ����ȷ
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

  // ����ÿ�����ص�
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      // ��ȡ��ǰ���ص��HSVֵ
      cv::Vec3b pixel = hsvImage.at<cv::Vec3b>(i, j);
      double h = pixel[0];
      double s = pixel[1];
      double v = pixel[2];

      // ��鵱ǰ���ص��Ƿ���Ŀ����ɫ��Χ��
      if (h >= target_hsv[0] - tolerance_hsv[0] &&
          h <= target_hsv[0] + tolerance_hsv[0] &&
          s >= target_hsv[1] - tolerance_hsv[1] &&
          s <= target_hsv[1] + tolerance_hsv[1] &&
          v >= target_hsv[2] - tolerance_hsv[2] &&
          v <= target_hsv[2] + tolerance_hsv[2]) {
        // ���������������ص���Ϊ��ɫ
        mask->at<uchar>(i, j) = 255;
      } else {
        // �����������������ص���Ϊ��ɫ
        mask->at<uchar>(i, j) = 0;
      }
    }
  }

  // �����Ҫ����������
  if (contours) {
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(*mask, *contours, hierarchy, cv::RETR_EXTERNAL,
                     cv::CHAIN_APPROX_SIMPLE);
  }

  // ������
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
  // ����PaddleOCR��ʵ�ּ򵥵�OCR����
  PaddleUtil::init();
  PaddleUtil::get().rec_image(image, out_text, max_length);
  return true;
}