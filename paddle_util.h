#pragma once
using namespace std;

#include <include/paddleocr.h>

class PaddleUtil {
public:
  PaddleUtil();

public:
  static PaddleUtil &get();
  static void init();
  void rec_image(cv::Mat *img_with_words, char *out_text, int *max_length);

private:
  PaddleOCR::PPOCR ocr;
};
