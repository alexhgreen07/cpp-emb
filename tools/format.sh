#!/bin/bash

clang-format-12 -i -style=llvm emb/*.cpp emb/*.hpp test/*.cpp test/*.hpp
