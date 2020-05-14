//
// Created by LastRise on 14/05/2020.
//

#ifndef UNTITLED2_STRUCT_DATA_H
#define UNTITLED2_STRUCT_DATA_H

#include <png.h>
#include <iostream>
#include <algorithm>

#define COUNT_COLORS 8


struct Image{
    int width, height;
    png_byte color_type;
    png_byte bit_depth;

    png_structp png_ptr;
    png_infop info_ptr;
    int number_of_passes;
    png_bytep *row_pointers;
};

struct Point{
    int x;
    int y;
};

struct Color{
    int red;
    int green;
    int blue;
    int alpha;
};

struct Rectangle{
    Point leftBottom;
    Point rightTop;
    Color color;
    int square = 0;
};

struct Triangle{
    Point point_1;
    Point point_2;
    Point point_3;
    int is_filled;
    int thickness;
    Color backgroundColor;
    Color lineColor;
};

struct setCollageModeParameters{
    int N;
    int M;
};

struct findRectanglesModeParameters{
    Color fColor;
    Color rColor;
};



#endif //UNTITLED2_STRUCT_DATA_H
