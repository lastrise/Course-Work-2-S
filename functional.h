//
// Created by LastRise on 15/05/2020.
//

#ifndef UNTITLED2_FUNCTIONAL_H
#define UNTITLED2_FUNCTIONAL_H

#include "./cmake-build-debug/struct_data.h"

void pngSetPixel(int x, int y, Image* image, Color color);
void setLine(Point point_1, Point point_2, Image* image, int thickness, Color color);
void fillTriangle(Triangle triangle, Image* image);
void setTriangle(Triangle triangle, Image* image);
void setCollage(Image* image, setCollageModeParameters parameters);
Color definePixelColor(Image* image, int x, int y);
bool compareColors(Color color_1, Color color_2);
bool checkRectangle(Image* image, Rectangle rectangle);
Rectangle findRectangles(Image* image, Color color);
void setRectangleColor(Rectangle rectangle, Color color, Image* image);
void read_png_file(char *file_name, struct Image* image);
void write_png_file(char *file_name, struct Image* image);

#endif //UNTITLED2_FUNCTIONAL_H
