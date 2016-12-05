call "%VS140COMNTOOLS%..\..\VC\vcvarsall.bat" x86
SET qmake=C:\\Qt\\5.6\\msvc2013\\bin\\qmake.exe

mkdir tmp
cd tmp

%qmake% ..\\qmake\\librazer.pro
nmake

cd ..
