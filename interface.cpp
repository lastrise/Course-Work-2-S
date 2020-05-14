#include "./functional.h"
#include <getopt.h>

char* colors[] = {"red", "yellow", "green", "blue",
                  "purple", "lblue", "white", "black"};

const Color RED = {255, 0, 0, 255};
const Color YELLOW = {255, 255, 0, 255};
const Color GREEN = {0, 255, 0, 255};
const Color BLUE = {0, 0, 255, 255};
const Color PURPLE = {255, 0, 255, 255};
const Color LBLUE = {0, 255, 255, 255};
const Color WHITE = {255, 255, 255, 255};
const Color BLACK = {0, 0, 0, 255};

bool checkCorrectColor(char* checkColor){
    bool isCorrect = false;
    for (int i = 0; i < COUNT_COLORS; ++i){
        char* color = colors[i];
        if (!strcmp(color, checkColor)){
            isCorrect = true;
            break;
        }
    }
    return isCorrect;
}

Color setColorByString(char* color){
    if (!strcmp("red", color)) return RED;
    if (!strcmp("yellow", color)) return YELLOW;
    if (!strcmp("green", color)) return GREEN;
    if (!strcmp("blue", color)) return BLUE;
    if (!strcmp("purple", color)) return PURPLE;
    if (!strcmp("lblue", color)) return LBLUE;
    if (!strcmp("white", color)) return WHITE;
    if (!strcmp("black", color)) return BLACK;
}

bool setTriangleMode(int argc, char *argv[], Image* image){
    int argument;
    int option_index;

    const char* short_options_parameters = "p:fB:T:C:";
    const struct option long_options_parameters[] = {
            {"point", required_argument,NULL,'p'},
            {"fill", no_argument,NULL,'f'},
            {"backgroundColor", required_argument,NULL,'B'},
            {"thickness", required_argument,NULL,'T'},
            {"lineColor", required_argument,NULL,'C'},
            {NULL,0,NULL,0}
    };

    char* point_1 = "-1 -1";
    char* point_2 = "-1 -1";
    char* point_3 = "-1 -1";
    int fill = 0;
    int thickness = 1;
    char* backgroundColor = "black";
    char* lineColor = "black";

    int count_points = 0;
    char* thicknessTemp = nullptr;

    argument = getopt_long(argc, argv, short_options_parameters, long_options_parameters, &option_index);
    while (argument != -1){
        switch (argument){
            case 'p': {
                if (optarg == NULL){
                    std::cout << "[ERROR] One of the point without argument!" << std::endl;
                    break;
                }
                count_points++;
                if (count_points == 1) point_1 = optarg;
                if (count_points == 2) point_2 = optarg;
                if (count_points == 3) point_3 = optarg;
                if (count_points > 3) std::cout << "[ERROR] Max of points is 3. Just your first values will be used!" << std::endl;
                break;
            }

            case 'f': {
                fill = 1;
                break;
            }

            case 'B': {
                if (optarg != NULL) {
                    backgroundColor = optarg;
                } else {
                    std::cout << "[ERROR] You send key 'B' without argument! It will be set by default." << std::endl;
                }
                break;
            }

            case 'T': {
                if (optarg != NULL){
                    thicknessTemp = optarg;
                } else {
                    std::cout << "[ERROR] You send key 'T' without argument! It will be set by default." << std::endl;
                }
                break;
            }

            case 'C': {
                if (optarg != NULL){
                    lineColor = optarg;
                } else {
                    std::cout << "[ERROR] You send key 'C' without argument! It will be set by default." << std::endl;
                }
                break;
            }
        }
        argument = getopt_long(argc, argv, short_options_parameters, long_options_parameters, &option_index);
    }


    //Начало: Проверка количества данных точек
    if (count_points < 3) {
        std::cout << "[ERROR] Count of points < 3!" << std::endl;
        return 0;
    }
    //Конец: Проверка количества данных точек


    //Начало: Считывание толщины линий
    if (thicknessTemp != nullptr){
        int intTempThickness;
        int result = sscanf(thicknessTemp, "%d", &intTempThickness);
        result == 1 ? thickness = intTempThickness : thickness = 1;
        if (thickness <= 0){
            thickness = 1;
            std::cout << "[ERROR] Thickness is not correct! It will be set by default." << std::endl;
        }
    }
    //Конец: Считывание толщины линий


    //Начало: Считывание точек
    int width = 500;
    int height = 500;
    int tempX, tempY;

    Point sPoint_1;
    Point sPoint_2;
    Point sPoint_3;

    for (int i = 1; i < 4; ++i){
        int result = sscanf(i == 1 ? point_1 : i == 2 ? point_2 : point_3, "%d %d", &tempX, &tempY);
        if (result == 2){


            if (tempX < 0 || tempX > width || tempY < 0 || tempY > height) {
                std::cout << "[ERROR] Point is not correct!" << std::endl;
                return false;
            } else {
                std::cout << "[LOG] Parse of point was success ";
                std::cout << "X: " << tempX << "; Y: " << tempY << std::endl;

                Point point = {tempX, tempY};

                i == 1 ? sPoint_1 = point : i == 2 ? sPoint_2 = point : sPoint_3 = point;

            }

        } else {
            std::cout << "[ERROR] Point is not correct!" << std::endl;
            return false;
        }
    }
    //Конец: Считывание точек


    //Начало: Проверка цветов линий и заливки
    bool result = checkCorrectColor(backgroundColor);
    if (!result) {backgroundColor = "black"; std::cout << "[ERROR] Background color is not correct! It will be set by default." << std::endl;}
    result = checkCorrectColor(lineColor);
    if (!result) {lineColor = "black"; std::cout << "[ERROR] Color of lines is not correct! It will be set by default." << std::endl;}
    //Конец: Проверка цветов линий и заливки



    Triangle triangleInformation = {sPoint_1,
                                    sPoint_2,
                                    sPoint_3,
                                    fill,
                                    thickness,
                                    setColorByString(backgroundColor),
                                    setColorByString(lineColor)};

    std::cout << std::endl;
    std::cout << "[LOG] Information about triangle:" << std::endl;
    std::cout << "[LOG] Point 1: " << point_1 << std::endl;
    std::cout << "[LOG] Point 2: " << point_2 << std::endl;
    std::cout << "[LOG] Point 3: " << point_3 << std::endl;
    std::cout << "[LOG] Thickness of lines: " << thickness << std::endl;
    std::cout << "[LOG] Color of lines: " << lineColor << std::endl;
    std::cout << "[LOG] Color of background: " << backgroundColor << std::endl;
    std::cout << "[LOG] Is filled: " << (fill ? "True" : "False") << std::endl;

    setTriangle(triangleInformation, image);

    return true;

}


bool setFindRectanglesMode(int argc, char* argv[], Image* image){
    char* fColor = "black";
    char* rColor = "white";

    const char* short_options_parameters = "f:r:";
    const struct option long_options_parameters[] = {
            {"findColor", required_argument,NULL,'f'},
            {"replaceColor", required_argument,NULL,'r'},
            {NULL,0,NULL,0}
    };

    int option_index = 0;
    int argument = getopt_long(argc, argv, short_options_parameters, long_options_parameters, &option_index);

    while (argument != -1){
        switch(argument){

            case 'f': {
                if (optarg != NULL) fColor = optarg;
                else std::cout << "[ERROR] You send key 'f' without argument! It will be set by default." << std::endl;
                break;
            }

            case 'r': {
                if (optarg != NULL) rColor = optarg;
                else std::cout << "[ERROR] You send key 'r' without argument! It will be set by default." << std::endl;
                break;
            }

        }
        argument = getopt_long(argc, argv, short_options_parameters, long_options_parameters, &option_index);
    }

    bool result = checkCorrectColor(fColor);
    if (!result) {std::cout << "[ERROR] fColor is not correct! It will be set by default." << std::endl; fColor = "black";}
    result = checkCorrectColor(rColor);
    if (!result) {std::cout << "[ERROR] rColor is not correct! It will be set by default." << std::endl; rColor = "white";}

    if (!strcmp(fColor, rColor)){
        fColor = "black";
        rColor = "white";
        std::cout << "[ERROR] fColor and rColor are identity! Their values will be set by default." << std::endl;
    }

    findRectanglesModeParameters parameters = {setColorByString(fColor), setColorByString(rColor)};

    std::cout << std::endl;
    std::cout << "[LOG] Information about function parameters:" << std::endl;
    std::cout << "[LOG] fColor is " << fColor << std::endl;
    std::cout << "[LOG] rColor is " << rColor << std::endl;

    Rectangle rectangle = findRectangles(image, parameters.fColor);
    if (rectangle.square) setRectangleColor(rectangle, parameters.rColor, image);
    else std::cout << "[LOG] There are not rectangles of your color on the picture!" << std::endl;
    return true;
}


bool setCollageMode(int argc, char* argv[], Image* image){

    int N = 1;
    int M = 1;

    char* tempN = nullptr;
    char* tempM = nullptr;

    const char* short_options_parameters = "w:h:";
    const struct option long_options_parameters[] = {
            {"width", required_argument,NULL,'w'},
            {"height", required_argument,NULL,'h'},
            {NULL,0,NULL,0}
    };

    int option_index = 0;
    int argument = getopt_long(argc, argv, short_options_parameters, long_options_parameters, &option_index);

    while (argument != -1){
        switch (argument){

            case 'w': {
                if (optarg != NULL) tempN = optarg;
                else std::cout << "[ERROR] You send key 'w' without argument! It will be set by default." << std::endl;
            }

            case 'h': {
                if (optarg != NULL) tempM = optarg;
                else std::cout << "[ERROR] You send key 'h' without argument! It will be set by default." << std::endl;
            }

        }
        argument = getopt_long(argc, argv, short_options_parameters, long_options_parameters, &option_index);
    }


    int intTempN = 0;
    int intTempM = 0;

    if (tempN) {
        int result = sscanf(tempN, "%d", &intTempN);
        if (result == 1) N = intTempN;
        else std::cout << "[ERROR] Bad argument for weigth! It will be set by default." << std::endl;
    }

    if (tempM) {
        int result = sscanf(tempM, "%d", &intTempM);
        if (result == 1) M = intTempM;
        else std::cout << "[ERROR] Bad argument for height! It will be set by default." << std::endl;
    }

    if (N <= 0){
        std::cout << "[ERROR] Bad argument for weight! It will be set by default." << std::endl;
        N = 1;
    }

    if (M <= 0){
        std::cout << "[ERROR] Bad argument for height! It will be set by default." << std::endl;
        M = 1;
    }

    setCollageModeParameters parameters = {N, M};
    std::cout << std::endl;
    std::cout << "[LOG] Information about function parameters:" << std::endl;
    std::cout << "[LOG] Weight = " << N << std::endl;
    std::cout << "[LOG] Height = " << M << std::endl;

    std::cout << "[LOG] Please, be patient. This process is hard." << std::endl;
    setCollage(image, parameters);
    return true;
}



int main (int argc, char *argv[]){

    const char* short_options_mode = "rsc";
    char* path = argv[1];
    optopt++;

    std::cout << "[LOG] Path of the file is " << path << std::endl;

    Image* image = new Image;
    read_png_file(path, image);

    std::cout << "[LOG] Information about image:" << std::endl;
    std::cout << "[LOG] Image's size is " << image->width << "x" << image->height << std::endl;
    std::cout << "[LOG] Image's bit depth is " << (int)image->bit_depth << std::endl;
    std::cout << "[LOG] Image's color type is " << (int)image->color_type << std::endl;

    const struct option long_options_mode[] = {
            {"findRectangles", no_argument,NULL,'r'},
            {"setTriangle", no_argument,NULL,'s'},
            {"createCollage", no_argument,NULL,'c'},
            {NULL,0,NULL,0}
    };

    int argument;
    int option_index;

    argument = getopt_long(argc, argv, short_options_mode, long_options_mode, &option_index);
    int mode = 0;

    if (argument != -1){
        switch (argument){
            case 's': {mode = 1; break;}
            case 'r': {mode = 2; break;}
            case 'c': {mode = 3; break;}
            case '?': {std::cout << "[ERROR] Unknown argument!" << std::endl; break;}
        }
    } else {
        std::cout << "[ERROR] There no arguments!" << std::endl;
    }

    if (!mode){
        std::cout << "[ERROR] Cant define mode!" << std::endl;
        return 0;
    } else {
        std::cout << "[LOG] Mode: " << mode << std::endl;
    }

    bool result = true;
    mode == 1 ? result = setTriangleMode(argc, argv, image) : false;
    mode == 2 ? result = setFindRectanglesMode(argc, argv, image) : false;
    mode == 3 ? result = setCollageMode(argc, argv, image) : false;
    if (!result) std::cout << "[ERROR] Bad arguments were given!" << std::endl;
    else {std::cout << "[LOG] Success action!" << std::endl; write_png_file("OUT_FILE.png", image);}

    return 0;
};