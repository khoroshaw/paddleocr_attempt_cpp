# My-PaddleOCR

## 介绍

如何在 C++ 项目中，通过源码使用 PaddlePaddle 实现 OCR 功能。  
本项目的所有源码：[gitee: paddleocr](https://gitee.com/sn-yang/my-paddleocr)

目前，官方提供使用 PaddleOcr 的方案有：

- 在 Python 项目中，调用 `paddlepaddle` + `paddleocr` 包。
- 在 C++项目中，调用一个可执行文件。（由编译 [PaddleOCR](https://gitee.com/paddlepaddle/PaddleOCR) 中的 `deploy/cpp_infer` 下的代码形成）

  Paddle OCR 提供了一个通过编译 `deploy/cpp_infer` 下的代码为 `ppocr.exe`，然后通过命令行调用获取 OCR 的结果。
  具体过程见： [服务器端 C++预测](https://gitee.com/paddlepaddle/PaddleOCR/tree/release/2.6/deploy/cpp_infer)

其它方法：

- 使用 Python 写一个 RESTful 服务，然后让 C++项目调用这个服务功能。

这里主要介绍一个更加直接的方法：

- 将 `deploy/cpp_infer` 的源码引入到我们的 C++项目。
- 做适当修改后，我们就可以直接使用这些 API。
- 当然，如果你愿意的话，也可以将这些源码形成一个新的项目，编译成一个 dll。（这里并不介绍）
  说明：
- 下面的方法只在 release 版中有效。可能是因为`paddle_inference.dll` 只支持 release 版。
  如果要支持 debug 版，可能需要重新编译 PaddlePaddle。

## Paddle OCR C++ 源码

Paddle OCR 的仓库，在[github: PaddleOCR](https://github.com/PaddlePaddle/PaddleOCR) 或者 [gitee: PaddleOCR](https://gitee.com/paddlepaddle/PaddleOCR)
C++ 相关的代码在目录 `deploy/cpp_infer` 里。

## 如何引入 Paddle OCR C++ 源码

### 需要安装的组件

- opencv
  我在 opencv 4.6 版本上测试通过。
  注意：opencv 4.5 版本存在一些问题，会导致功能异常。
  设置环境变量：

```bat
@REM 设置opencv目录环境变量
setx OPENCV_ROOT "C:\3rd\opencv4.6\build"
```

- paddle_inference
  我用的是 2.6 版。
  设置环境变量：

```bat
@REM 设置paddle_inference目录环境变量
setx PADDLE_ROOT "C:\3rd\paddle_inference"
```

- cuda (optional)
  你应该懂得。
- cudnn (optional)
  你应该懂得。

### 创建 C++项目

- 创建一个空的 C++ 项目。

- 在项目中创建文件：`OpenCVDebug.props`

```xml
<?xml version="1.0" encoding="utf-8"?>
<Project ToolsVersion="4.0" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">
  <ImportGroup Label="PropertySheets" />
  <PropertyGroup Label="UserMacros" />
  <PropertyGroup />
  <ItemDefinitionGroup>
    <ClCompile>
      <AdditionalIncludeDirectories>$(OPENCV_ROOT)\include;%(AdditionalIncludeDirectories)</AdditionalIncludeDirectories>
    </ClCompile>
    <Link>
      <AdditionalLibraryDirectories>$(OPENCV_ROOT)\x64\vc15\lib;%(AdditionalLibraryDirectories)</AdditionalLibraryDirectories>
      <AdditionalDependencies>opencv_world460d.lib;%(AdditionalDependencies)</AdditionalDependencies>
    </Link>
  </ItemDefinitionGroup>
  <ItemGroup />
</Project>
```

- 在项目中创建文件：`OpenCVRelease.props`

```xml
<?xml version="1.0" encoding="utf-8"?>
<Project ToolsVersion="4.0" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">
  <ImportGroup Label="PropertySheets" />
  <PropertyGroup Label="UserMacros" />
  <PropertyGroup />
  <ItemDefinitionGroup>
    <ClCompile>
      <AdditionalIncludeDirectories>$(OPENCV_ROOT)\include;%(AdditionalIncludeDirectories)</AdditionalIncludeDirectories>
    </ClCompile>
    <Link>
      <AdditionalLibraryDirectories>$(OPENCV_ROOT)\x64\vc15\lib;%(AdditionalLibraryDirectories)</AdditionalLibraryDirectories>
      <AdditionalDependencies>opencv_world460.lib;%(AdditionalDependencies)</AdditionalDependencies>
    </Link>
  </ItemDefinitionGroup>
  <ItemGroup />
</Project>
```

- 在项目中创建文件：`PaddleDebug.props`

```xml
<?xml version="1.0" encoding="utf-8"?>
<Project ToolsVersion="4.0" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">
  <ImportGroup Label="PropertySheets" />
  <PropertyGroup Label="UserMacros" />
  <PropertyGroup />
  <ItemDefinitionGroup>
    <ClCompile>
      <AdditionalIncludeDirectories>./;$(PADDLE_ROOT)/paddle/include;$(PADDLE_ROOT)/third_party/install/zlib/include;$(PADDLE_ROOT)/third_party/boost;$(PADDLE_ROOT)/third_party/eigen3;$(PADDLE_ROOT)/third_party/install/mklml/include;%(AdditionalIncludeDirectories)</AdditionalIncludeDirectories>
    </ClCompile>
    <Link>
      <AdditionalLibraryDirectories>$(PADDLE_ROOT)/third_party/install/zlib/lib;$(PADDLE_ROOT)/third_party/install/mklml/lib;$(PADDLE_ROOT)/paddle/lib;%(AdditionalLibraryDirectories)</AdditionalLibraryDirectories>
      <AdditionalDependencies>paddle_inference.lib;mklml.lib;libiomp5md.lib;%(AdditionalDependencies)</AdditionalDependencies>
    </Link>
  </ItemDefinitionGroup>
  <ItemGroup />
</Project>
```

- 在项目中创建文件：`PaddleRelease.props`

```xml
<?xml version="1.0" encoding="utf-8"?>
<Project ToolsVersion="4.0" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">
  <ImportGroup Label="PropertySheets" />
  <PropertyGroup Label="UserMacros" />
  <PropertyGroup />
  <ItemDefinitionGroup>
    <ClCompile>
      <AdditionalIncludeDirectories>./;$(PADDLE_ROOT)/paddle/include;$(PADDLE_ROOT)/third_party/install/zlib/include;$(PADDLE_ROOT)/third_party/boost;$(PADDLE_ROOT)/third_party/eigen3;$(PADDLE_ROOT)/third_party/install/mklml/include;%(AdditionalIncludeDirectories)</AdditionalIncludeDirectories>
    </ClCompile>
    <Link>
      <AdditionalLibraryDirectories>$(PADDLE_ROOT)/third_party/install/zlib/lib;$(PADDLE_ROOT)/third_party/install/mklml/lib;$(PADDLE_ROOT)/paddle/lib;%(AdditionalLibraryDirectories)</AdditionalLibraryDirectories>
      <AdditionalDependencies>paddle_inference.lib;mklml.lib;libiomp5md.lib;%(AdditionalDependencies)</AdditionalDependencies>
    </Link>
  </ItemDefinitionGroup>
  <ItemGroup />
</Project>
```

- 修改 C++项目文件：`my-paddleocr.vcxproj`

```xml
  <ImportGroup Label="PropertySheets" Condition="'$(Configuration)|$(Platform)'=='Debug|Win32'">
    <Import Project="$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props" Condition="exists('$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props')" Label="LocalAppDataPlatform" />
    <Import Project=".\OpenCVDebug.props" />
    <Import Project=".\PaddleDebug.props" />
  </ImportGroup>
  <ImportGroup Label="PropertySheets" Condition="'$(Configuration)|$(Platform)'=='Release|Win32'">
    <Import Project="$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props" Condition="exists('$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props')" Label="LocalAppDataPlatform" />
    <Import Project=".\OpenCVRelease.props" />
    <Import Project=".\PaddleRelease.props" />
  </ImportGroup>
  <ImportGroup Label="PropertySheets" Condition="'$(Configuration)|$(Platform)'=='Debug|x64'">
    <Import Project="$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props" Condition="exists('$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props')" Label="LocalAppDataPlatform" />
    <Import Project=".\OpenCVDebug.props" />
    <Import Project=".\PaddleDebug.props" />
  </ImportGroup>
  <ImportGroup Label="PropertySheets" Condition="'$(Configuration)|$(Platform)'=='Release|x64'">
    <Import Project="$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props" Condition="exists('$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props')" Label="LocalAppDataPlatform" />
    <Import Project=".\OpenCVRelease.props" />
    <Import Project=".\PaddleRelease.props" />
  </ImportGroup>
```

### 引入 Paddle OCR C++ 源码

- 克隆 Paddle OCR 仓库到本地。
- 切换到分支 `release/2.6`。

```sh
git clone https://gitee.com/paddlepaddle/PaddleOCR.git
cd PaddleOCR
git checkout release/2.6
```

- 将 `deploy/cpp_infer` 目录下的 `include`、`src`两个目录的内容复制到我们的 C++ 项目中。
- 修改新的 `src` 目录名称为 `ocr`。
- 删除`ocr/main.cpp`
- 在`include`目录下创建`ocr_flags.h`文件，内容如下：

这个文件是为了替换 google 的 gflags 库的使用。

```c++
#pragma once

// common args
#include <string>
using std::string;

#define DECLARE_bool(name) extern bool FLAGS_##name;
#define DECLARE_int32(name) extern int FLAGS_##name;
#define DECLARE_string(name) extern string FLAGS_##name;
#define DECLARE_double(name) extern double FLAGS_##name;

#define DEFINE_VARIABLE(type, name, value, help)                               \
    static const type FLAGS_nono##name = value;                                \
    type FLAGS_##name = FLAGS_nono##name;                                      \
    static type FLAGS_no##name = FLAGS_nono##name;

#define DEFINE_bool(name, val, txt) DEFINE_VARIABLE(bool, name, val, txt)
#define DEFINE_int32(name, val, txt) DEFINE_VARIABLE(int, name, val, txt)
#define DEFINE_string(name, val, txt) DEFINE_VARIABLE(string, name, val, txt)
#define DEFINE_double(name, val, txt) DEFINE_VARIABLE(double, name, val, txt)
```

- 修改`include/args.h`文件，内容如下：

不使用 `gflags/gflags.h`  
注释行 `// #include <gflags/gflags.h>`  
增加行 `#include "ocr_flags.h"`

```c++
...
// #include <gflags/gflags.h>
#include "ocr_flags.h"
...
```

- 修改`ocr/args.cpp`文件，内容如下：

不使用 `gflags/gflags.h`  
注释行 `// #include <gflags/gflags.h>`  
增加行 `#include "include/ocr_flags.h"`

```c++
...
// #include <gflags/gflags.h>
#include "include/ocr_flags.h"
...
```

- 修改`include/paddleocr.cpp`文件，内容如下：

不使用 `auto_log/autolog.h`  
注释行 `// #include "auto_log/autolog.h"`  
注释方法 `PPOCR::benchmark_log` 的内容。

```c++
// #include "auto_log/autolog.h"
...
void PPOCR::benchmark_log(int img_num) {
  // if (this->time_info_det[0] + this->time_info_det[1] +
  // this->time_info_det[2] >
  //     0) {
  //   AutoLogger autolog_det("ocr_det", FLAGS_use_gpu, FLAGS_use_tensorrt,
  //                          FLAGS_enable_mkldnn, FLAGS_cpu_threads, 1,
  //                          "dynamic", FLAGS_precision, this->time_info_det,
  //                          img_num);
  //   autolog_det.report();
  // }
  // if (this->time_info_rec[0] + this->time_info_rec[1] +
  // this->time_info_rec[2] >
  //     0) {
  //   AutoLogger autolog_rec("ocr_rec", FLAGS_use_gpu, FLAGS_use_tensorrt,
  //                          FLAGS_enable_mkldnn, FLAGS_cpu_threads,
  //                          FLAGS_rec_batch_num, "dynamic", FLAGS_precision,
  //                          this->time_info_rec, img_num);
  //   autolog_rec.report();
  // }
  // if (this->time_info_cls[0] + this->time_info_cls[1] +
  // this->time_info_cls[2] >
  //     0) {
  //   AutoLogger autolog_cls("ocr_cls", FLAGS_use_gpu, FLAGS_use_tensorrt,
  //                          FLAGS_enable_mkldnn, FLAGS_cpu_threads,
  //                          FLAGS_cls_batch_num, "dynamic", FLAGS_precision,
  //                          this->time_info_cls, img_num);
  //   autolog_cls.report();
  // }
}
...
```

- 修改`include/paddlestructure.cpp`文件，内容如下：

不使用 `auto_log/autolog.h`  
注释行 `// #include "auto_log/autolog.h"`  
注释方法 `PPOCR::benchmark_log` 的内容。

```c++
// #include "auto_log/autolog.h"
...
void PaddleStructure::benchmark_log(int img_num) {
  // if (this->time_info_det[0] + this->time_info_det[1] +
  // this->time_info_det[2] >
  //     0) {
  //   AutoLogger autolog_det("ocr_det", FLAGS_use_gpu, FLAGS_use_tensorrt,
  //                          FLAGS_enable_mkldnn, FLAGS_cpu_threads, 1,
  //                          "dynamic", FLAGS_precision, this->time_info_det,
  //                          img_num);
  //   autolog_det.report();
  // }
  // if (this->time_info_rec[0] + this->time_info_rec[1] +
  // this->time_info_rec[2] >
  //     0) {
  //   AutoLogger autolog_rec("ocr_rec", FLAGS_use_gpu, FLAGS_use_tensorrt,
  //                          FLAGS_enable_mkldnn, FLAGS_cpu_threads,
  //                          FLAGS_rec_batch_num, "dynamic", FLAGS_precision,
  //                          this->time_info_rec, img_num);
  //   autolog_rec.report();
  // }
  // if (this->time_info_cls[0] + this->time_info_cls[1] +
  // this->time_info_cls[2] >
  //     0) {
  //   AutoLogger autolog_cls("ocr_cls", FLAGS_use_gpu, FLAGS_use_tensorrt,
  //                          FLAGS_enable_mkldnn, FLAGS_cpu_threads,
  //                          FLAGS_cls_batch_num, "dynamic", FLAGS_precision,
  //                          this->time_info_cls, img_num);
  //   autolog_cls.report();
  // }
  // if (this->time_info_table[0] + this->time_info_table[1] +
  //         this->time_info_table[2] >
  //     0) {
  //   AutoLogger autolog_table("table", FLAGS_use_gpu, FLAGS_use_tensorrt,
  //                            FLAGS_enable_mkldnn, FLAGS_cpu_threads,
  //                            FLAGS_cls_batch_num, "dynamic", FLAGS_precision,
  //                            this->time_info_table, img_num);
  //   autolog_table.report();
  // }
  // if (this->time_info_layout[0] + this->time_info_layout[1] +
  //         this->time_info_layout[2] >
  //     0) {
  //   AutoLogger autolog_layout("layout", FLAGS_use_gpu, FLAGS_use_tensorrt,
  //                             FLAGS_enable_mkldnn, FLAGS_cpu_threads,
  //                             FLAGS_cls_batch_num, "dynamic",
  //                             FLAGS_precision, this->time_info_layout,
  //                             img_num);
  //   autolog_layout.report();
  // }
}
...
```

- 修改`include/utility.cpp`文件，内容如下：

不使用 `dirent.h`  
注释行 `// #include <dirent.h>`  
注释方法 `Utility::GetAllFiles` 的内容。

```c++
// #include <dirent.h>
...
// list all files under a directory
void Utility::GetAllFiles(const char *dir_name,
                          std::vector<std::string> &all_inputs) {
  // if (NULL == dir_name) {
  //   std::cout << " dir_name is null ! " << std::endl;
  //   return;
  // }
  // struct stat s;
  // stat(dir_name, &s);
  // if (!S_ISDIR(s.st_mode)) {
  //   std::cout << "dir_name is not a valid directory !" << std::endl;
  //   all_inputs.push_back(dir_name);
  //   return;
  // } else {
  //   struct dirent *filename; // return value for readdir()
  //   DIR *dir;                // return value for opendir()
  //   dir = opendir(dir_name);
  //   if (NULL == dir) {
  //     std::cout << "Can not open dir " << dir_name << std::endl;
  //     return;
  //   }
  //   std::cout << "Successfully opened the dir !" << std::endl;
  //   while ((filename = readdir(dir)) != NULL) {
  //     if (strcmp(filename->d_name, ".") == 0 ||
  //         strcmp(filename->d_name, "..") == 0)
  //       continue;
  //     // img_dir + std::string("/") + all_inputs[0];
  //     all_inputs.push_back(dir_name + std::string("/") +
  //                          std::string(filename->d_name));
  //   }
  // }
}
...
```

## 编写调用程序

- 在项目中创建文件：`paddle_util.h`
  内容如下：

```c++
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
```

- 在项目中创建文件：`paddle_util.cpp`
  内容如下：

```c++
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

  std::vector<OCRPredictResult> ocr_result =
      ocr.ocr(img, FLAGS_det, FLAGS_rec, FLAGS_cls);

  Utility::print_result(ocr_result);
  if (FLAGS_visualize && FLAGS_det) {
    std::string file_name = Utility::basename(imageFile);
    Utility::VisualizeBboxes(img, ocr_result, FLAGS_output + "/" + file_name);
  }
}
```

- 在项目中创建主程序文件：`my-paddleocr.cpp`
  内容如下：

```c++
#include <iostream>

#include "paddle_util.h"

int main() {

  PaddleUtil::init();
  PaddleUtil::get().rec_image("model/254.jpg");
  std::cout << "Done!\n";
}
```

## 编译运行

- 在 Visual Studio 中使用 Release 版本编译。
- 运行 `copy_paddle_dll.bat`，复制 `paddle_inference.dll` 到输出目录。

```bat
@REM copy dll to Release
copy /Y %PADDLE_ROOT%\paddle\lib\paddle_inference.dll .\x64\Release\
copy /Y %PADDLE_ROOT%\third_party\install\mkldnn\lib\mkldnn.dll .\x64\Release\
copy /Y %PADDLE_ROOT%\third_party\install\mklml\lib\mklml.dll .\x64\Release\
copy /Y %PADDLE_ROOT%\third_party\install\mklml\lib\libiomp5md.dll .\x64\Release\
copy /Y %PADDLE_ROOT%\third_party\install\paddle2onnx\lib\paddle2onnx.dll .\x64\Release\
copy /Y %PADDLE_ROOT%\third_party\install\onnxruntime\lib\onnxruntime.dll .\x64\Release\
```
