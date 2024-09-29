mkdir -p build
g++ -o build/client http-client.cpp -I../SocketWrapperLib/include -L../SocketWrapperLib/build -lSocketWrapper
