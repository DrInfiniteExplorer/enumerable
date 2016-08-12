set CC=
set CXX=
mkdir build-vc
cd build-vc
call cmake -G "Visual Studio 14 2015" .. && cmake --build . && ctest ..
cd ..
