#!/bin/bash

g++ -std=c++17 \
    src/main.cpp \
    src/Game.cpp \
    src/GameState.cpp \
    src/Entity/AimEntity.cpp \
    src/Entity/FruitEntity.cpp \
    src/Entity/BombEntity.cpp \
    src/Entity/SliceEntity.cpp \
    -Iinclude \
    -Iinclude/Entity \
    -lsfml-graphics -lsfml-window -lsfml-system \
    -O2 \
    -o game

./game
