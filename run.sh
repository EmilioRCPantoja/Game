#!/bin/bash

g++ -std=c++17 \
    src/main.cpp src/Game.cpp src/Entity/AimEntity.cpp src/Entity/SliceEntity.cpp \
    -Iinclude \
    -lsfml-graphics -lsfml-window -lsfml-system \
    -o game

./game