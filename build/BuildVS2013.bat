call "%VS140COMNTOOLS%..\..\VC\vcvarsall.bat" x86

mkdir tmp
cd tmp

msbuild ..\vs2013\librazer.vcxproj

cd ..
