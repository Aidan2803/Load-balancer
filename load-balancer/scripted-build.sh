#!/bin/bash

while getopts "f" opt; do
  case $opt in
    f)
      echo "Performing clang-format..."
      clang-format -i include/*.hpp
      clang-format -i src/*.cpp
      ;;
    \?)
      echo "Invalid option: -$OPTARG" >&2
      ;;
  esac
done

cd build
make