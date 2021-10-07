#!/bin/bash

gcc main.c \
-lSDL2 \
-lSDL2_ttf \
-o main

sudo ./main
