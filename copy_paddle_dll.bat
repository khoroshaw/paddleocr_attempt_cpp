copy /Y %PADDLE_ROOT%\paddle\lib\paddle_inference.dll .\lib\
copy /Y %PADDLE_ROOT%\third_party\install\mkldnn\lib\mkldnn.dll .\lib\
copy /Y %PADDLE_ROOT%\third_party\install\mklml\lib\mklml.dll .\lib\
copy /Y %PADDLE_ROOT%\third_party\install\mklml\lib\libiomp5md.dll .\lib\
copy /Y %PADDLE_ROOT%\third_party\install\paddle2onnx\lib\paddle2onnx.dll .\lib\
copy /Y %PADDLE_ROOT%\third_party\install\onnxruntime\lib\onnxruntime.dll .\lib\

@REM copy dll to Debug
copy /Y %PADDLE_ROOT%\paddle\lib\paddle_inference.dll .\x64\Debug\
copy /Y %PADDLE_ROOT%\third_party\install\mkldnn\lib\mkldnn.dll .\x64\Debug\
copy /Y %PADDLE_ROOT%\third_party\install\mklml\lib\mklml.dll .\x64\Debug\
copy /Y %PADDLE_ROOT%\third_party\install\mklml\lib\libiomp5md.dll .\x64\Debug\
copy /Y %PADDLE_ROOT%\third_party\install\paddle2onnx\lib\paddle2onnx.dll .\x64\Debug\
copy /Y %PADDLE_ROOT%\third_party\install\onnxruntime\lib\onnxruntime.dll .\x64\Debug\

@REM copy dll to Release
copy /Y %PADDLE_ROOT%\paddle\lib\paddle_inference.dll .\x64\Release\
copy /Y %PADDLE_ROOT%\third_party\install\mkldnn\lib\mkldnn.dll .\x64\Release\
copy /Y %PADDLE_ROOT%\third_party\install\mklml\lib\mklml.dll .\x64\Release\
copy /Y %PADDLE_ROOT%\third_party\install\mklml\lib\libiomp5md.dll .\x64\Release\
copy /Y %PADDLE_ROOT%\third_party\install\paddle2onnx\lib\paddle2onnx.dll .\x64\Release\
copy /Y %PADDLE_ROOT%\third_party\install\onnxruntime\lib\onnxruntime.dll .\x64\Release\

