#include "opencv2/core.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <vector>

#include "paddle_util.h"
#include <include/args.h>

using namespace PaddleOCR;

PaddleUtil::PaddleUtil() {}

PaddleUtil &PaddleUtil::get() {
  static PaddleUtil self;
  return self;
}

void PaddleUtil::init() {
  FLAGS_det = true;
  FLAGS_rec = true;
  FLAGS_cls = false;
  FLAGS_use_angle_cls = false;
  FLAGS_det_model_dir = "model/whl/det/en/en_PP-OCRv3_det_infer";
  FLAGS_rec_model_dir = "model/whl/rec/en/en_PP-OCRv4_rec_infer";
  FLAGS_rec_char_dict_path = "model/en_dict.txt";
}

void PaddleUtil::rec_image(const string &imageFile) {
  if (FLAGS_benchmark) {
    ocr.reset_timer();
  }

  cv::Mat img = cv::imread(imageFile, cv::IMREAD_COLOR);
  if (!img.data) {
    std::cerr << "[ERROR] image read failed! image path: " << imageFile
              << std::endl;
    return;
  }
  //cv::imwrite("test.jpg", img);

  std::vector<OCRPredictResult> ocr_result =
      ocr.ocr(img, FLAGS_det, FLAGS_rec, FLAGS_cls);

  // Print the OCR results
  for (int i = 0; i < ocr_result.size(); i++) {
    if (ocr_result[i].score != -1.0) {
      std::cout << ocr_result[i].text << std::endl;
    } 
  } 

  //Utility::print_result(ocr_result);
  //if (FLAGS_visualize && FLAGS_det) {
  //  std::string file_name = Utility::basename(imageFile);
  //  Utility::VisualizeBboxes(img, ocr_result, FLAGS_output + "/" + file_name);
  //}
}

//
// void PaddleUtil::rec_image(const string& imageFile)
//{
//
//    std::vector<cv::String> cv_all_img_names;
//    cv_all_img_names.push_back(imageFile);
//
//    if (FLAGS_benchmark) {
//        ocr.reset_timer();
//    }
//
//    std::vector<cv::Mat> img_list;
//    std::vector<cv::String> img_names;
//    for (int i = 0; i < cv_all_img_names.size(); ++i) {
//        cv::Mat img = cv::imread(cv_all_img_names[i], cv::IMREAD_COLOR);
//        if (!img.data) {
//            std::cerr << "[ERROR] image read failed! image path: "
//                      << cv_all_img_names[i] << std::endl;
//            continue;
//        }
//        img_list.push_back(img);
//        img_names.push_back(cv_all_img_names[i]);
//    }
//
//    std::vector<std::vector<OCRPredictResult>> ocr_results =
//        ocr.ocr(img_list, FLAGS_det, FLAGS_rec, FLAGS_cls);
//
//    for (int i = 0; i < img_names.size(); ++i) {
//        std::cout << "predict img: " << cv_all_img_names[i] << std::endl;
//        Utility::print_result(ocr_results[i]);
//        if (FLAGS_visualize && FLAGS_det) {
//            std::string file_name = Utility::basename(img_names[i]);
//            cv::Mat srcimg = img_list[i];
//            Utility::VisualizeBboxes(
//                srcimg, ocr_results[i], FLAGS_output + "/" + file_name
//            );
//        }
//    }
//    if (FLAGS_benchmark) {
//        ocr.benchmark_log(cv_all_img_names.size());
//    }
//}
