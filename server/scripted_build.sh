mkdir -p build
g++ -o build/server http-server.cpp -I../SocketWrapperLib/include -L../SocketWrapperLib/build -lSocketWrapper
