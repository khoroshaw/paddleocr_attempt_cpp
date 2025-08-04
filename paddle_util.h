#pragma once
using namespace std;

#include <include/paddleocr.h>

class PaddleUtil {
public:
  PaddleUtil();

public:
  static PaddleUtil &get();
  static void init();
  void rec_image(const string &imageFile);

private:
  PaddleOCR::PPOCR ocr;
};
