set CC=clang
set CXX=clang++
mkdir build-clang
cd build-clang
cmake .. -G "Unix Makefiles" && cmake --build . && ctest ..
cd ..
