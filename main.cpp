#include "./functional.h"

void pngSetPixel(int x, int y, Image* image, Color color)
{
    if (image->width < x + 1) return;
    if (image->height < y + 1) return;

    if (x < 0) return;
    if (y < 0) return;

    int size = 1;
    if (image->color_type == PNG_COLOR_TYPE_RGB) size = 3;
    if (image->color_type == PNG_COLOR_TYPE_RGBA) size = 4;

    image->row_pointers[image->height - y - 1][x * size + 0] = color.red;
    image->row_pointers[image->height - y - 1][x * size + 1] = color.green;
    image->row_pointers[image->height - y - 1][x * size + 2] = color.blue;
    if (size == 4) image->row_pointers[image->height - y - 1][x * size + 3] = color.alpha;

}

void setLine(Point point_1, Point point_2, Image* image, int thickness, Color color){
    int deltaX = point_2.x - point_1.x;
    int deltaY = point_2.y - point_1.y;

    int const absDeltaX = abs(deltaX);
    int const absDeltaY = abs(deltaY);

    float accretion = 0;

    int out = 0;
    int inter = 0;

    if (thickness % 2 == 1){

        out = (thickness - 1) / 2;
        inter = out;

    } else {
        out = (thickness) / 2 - 1;
        inter = thickness - out - 1;

    }

    if (absDeltaX > absDeltaY) {
        double const error = (double)absDeltaY / (double)absDeltaX;
        int y = point_1.y;
        const int direction = deltaY != 0 ? (deltaY > 0 ? 1 : -1) : 0;
        for (int x = point_1.x; deltaX > 0 ? x <= point_2.x : x >= point_2.x; deltaX > 0 ? x++ : x--) {

            pngSetPixel(x, y, image, color);

            for (int i = 1; i <= out; i++){
                pngSetPixel(x, y + i, image, color);
            }

            for (int i = 1; i <= inter; i++){
                pngSetPixel(x, y - i, image, color);
            }

            accretion += error;

            if (accretion >= 1.0f) {
                accretion -= 1.0f;
                y += direction;
            }
        }
    } else {
        double const error = (double)absDeltaX / (double)absDeltaY;
        int x = point_1.x;
        const int direction = deltaX != 0 ? (deltaX > 0 ? 1 : -1) : 0;
        for (int y = point_1.y; deltaY > 0 ? y <= point_2.y : y >= point_2.y; deltaY > 0 ? y++ : y--){
            pngSetPixel(x, y, image, color);

            for (int i = 1; i <= out; i++){
                pngSetPixel(x + i, y, image, color);
            }

            for (int i = 1; i <= inter; i++){
                pngSetPixel(x - i, y, image, color);
            }

            accretion += error;

            if (accretion >= 1.0f){
                accretion -= 1.0f;
                x += direction;
            }
        }
    }
}

void fillTriangle(Triangle triangle, Image* image){
    Point point_1 = triangle.point_1;
    Point point_2 = triangle.point_2;
    Point point_3 = triangle.point_3;
    Color color = triangle.backgroundColor;

    int LeftX = std::min(point_1.x, std::min(point_2.x, point_3.x));
    int RightX = std::max(point_1.x, std::max(point_2.x, point_3.x));
    int TopY = std::max(point_1.y, std::max(point_2.y, point_3.y));
    int BottomY = std::min(point_1.y, std::min(point_2.y, point_3.y));

    for (int y = BottomY; y <= TopY; ++y){
        for (int x = LeftX; x < RightX; ++x){

            int s_0 = abs((point_2.x - point_1.x) * (point_3.y - point_1.y) - (point_3.x - point_1.x) * (point_2.y - point_1.y));
            int s_1 = abs((point_2.x - point_1.x) * (y - point_1.y) - (x - point_1.x) * (point_2.y - point_1.y));
            int s_2 = abs((x - point_1.x) * (point_3.y - point_1.y) - (point_3.x - point_1.x) * (y - point_1.y));
            int s_3 = abs((point_2.x - x) * (point_3.y - y) - (point_3.x - x) * (point_2.y - y));

            if (s_1 + s_2 + s_3 == s_0){
                pngSetPixel(x, y, image, color);
            }
        }
    }

}

void setTriangle(Triangle triangle, Image* image){
    int isFilled = triangle.is_filled;
    if (isFilled) fillTriangle(triangle, image);
    setLine(triangle.point_1, triangle.point_2, image, triangle.thickness, triangle.lineColor);
    setLine(triangle.point_1, triangle.point_3, image, triangle.thickness, triangle.lineColor);
    setLine(triangle.point_2, triangle.point_3, image, triangle.thickness, triangle.lineColor);
}

void setCollage(Image* image, setCollageModeParameters parameters){
    png_uint_16 new_width = image->width * parameters.N;
    png_uint_16 new_height = image->height * parameters.M;

    png_uint_16 old_width = image->width;
    png_uint_16 old_height = image->height;

    std::cout << "New width: " << new_width << std::endl;
    std::cout << "New height: " << new_height << std::endl;

    int size = 3;
    if (image->color_type == PNG_COLOR_TYPE_RGB) size = 3;
    if (image->color_type == PNG_COLOR_TYPE_RGBA) size = 4;

    png_bytepp new_pixels = (png_bytepp)realloc(image->row_pointers, sizeof(png_bytep) * new_height);
    if (new_pixels == NULL){
        std::cout << "Are you crazy?" << std::endl;
        return;
    }

    for (int i = 0; i < image->height; ++i){
        new_pixels[i] = (png_bytep)realloc((new_pixels)[i], size * new_width);
    }

    for (int i = image->height; i < new_height; ++i){
        new_pixels[i] = (png_bytep)malloc(size * new_width);
    }

    for (int i = 0; i < parameters.M; ++i){
        for (int j = 0; j < parameters.N; ++j){

            for (int y = i * old_height; y < (i + 1) * old_height; ++y){
                for (int x = j * old_width; x < (j + 1) * old_width; ++x){

                    new_pixels[y][size*x] = new_pixels[y - i * old_height][size*(x - j * old_width)];
                    new_pixels[y][size*x+1] = new_pixels[y - i * old_height][size*(x - j * old_width)+1];
                    new_pixels[y][size*x+2] = new_pixels[y - i * old_height][size*(x - j * old_width)+2];
                    if (size == 4) new_pixels[y][size*x+3] = new_pixels[y - i * old_height][size*(x - j * old_width)+3];

                }
            }
        }
    }

    image->row_pointers = new_pixels;
    image->height = new_height;
    image->width = new_width;
    return;
}

Color definePixelColor(Image* image, int x, int y){

    int size = 3;
    if (image->color_type == PNG_COLOR_TYPE_RGBA) size = 4;

    int red = (int)image->row_pointers[image->height - y - 1][x * size];
    int green = (int)image->row_pointers[image->height - y - 1][x * size + 1];
    int blue = (int)image->row_pointers[image->height - y - 1][x * size + 2];

    int alpha;
    if (size == 4) alpha = (int)image->row_pointers[image->height - y - 1][x * size + 3];
    else alpha = 255;

    Color color = {red, green, blue, alpha};
    return color;
}

bool compareColors(Color color_1, Color color_2){
    return color_1.red == color_2.red && color_1.green == color_2.green && color_1.blue == color_2.blue && color_1.alpha == color_2.alpha;
}

bool checkRectangle(Image* image, Rectangle rectangle){

    if (rectangle.leftBottom.x == rectangle.rightTop.x) return false;
    if (rectangle.leftBottom.y == rectangle.rightTop.y) return false;

    for (int x = rectangle.leftBottom.x; x <= rectangle.rightTop.x; ++x){
        Color pixelColorRectangleBottom = definePixelColor(image, x, rectangle.leftBottom.y);
        Color pixelColorRectangleTop = definePixelColor(image, x, rectangle.rightTop.y);
        if (!compareColors(pixelColorRectangleBottom, rectangle.color) || !compareColors(pixelColorRectangleTop, rectangle.color)){
            return false;
        }
    }

    for (int y = rectangle.leftBottom.y; y <= rectangle.rightTop.y; ++y){
        Color pixelColorRectangleBottom = definePixelColor(image, rectangle.leftBottom.x, y);
        Color pixelColorRectangleTop = definePixelColor(image, rectangle.rightTop.x, y);
        if (!compareColors(pixelColorRectangleBottom, rectangle.color) || !compareColors(pixelColorRectangleTop, rectangle.color)){
            return false;
        }
    }

    return true;

}

Rectangle findRectangles(Image* image, Color color){
    int max_size = 0;
    Rectangle max_rectangle{};

    int** pixels_map = (int**)malloc(sizeof(int*) * image->height);
    for (int i = 0; i < image->height; ++i){
        pixels_map[i] = (int*)calloc(image->width, sizeof(int) * image->width);
    }

    for (int x = 1; x < image->width - 1; ++x){
        for (int y = 1; y < image->height - 1; ++y){

            Color pixelColor = definePixelColor(image, x, y);

            if (compareColors(pixelColor, color) && pixels_map[y][x] == 0){
                pixels_map[y][x] = 1;

                int start_x = x + 1;
                int start_y = y;

                Color currentPixelColor = definePixelColor(image, start_x, start_y);
                Color topPixelColor = definePixelColor(image, start_x, start_y+1);
                Color leftPixelColor = definePixelColor(image, start_x - 1, start_y);

                //Двигаем направо, пока цвет соответствует
                while (compareColors(currentPixelColor, color) && pixels_map[start_y][start_x] == 0 && start_x < image->width - 1){
                    if (compareColors(topPixelColor, color)){
                        //Но тут мы замечаем цвет сверху
                        //Погнали епта посмотрим, что там сверху
                        while (compareColors(topPixelColor, color) && pixels_map[start_y][start_x] == 0 && start_y < image->height - 1){
                            if (compareColors(leftPixelColor, color)){
                                //Но тут мы заметили еще и пиксель слева
                                //Тут мы прикинули примерные положения левого нижнего угла прямоугольника
                                //И прикинули правого верхнего угла прямоугольника
                                //Давайте же проверим, реально ли там прямоугольник

                                Point leftBottom = {x, y};
                                Point rightTop = {start_x, start_y};
                                int square = (start_x - x + 1) * (start_y - y + 1);
                                Rectangle rectangle = {leftBottom, rightTop, color, square};

                                //АГА, ПОПАЛСЯ ПРЯМОУГОЛЬНИК!!!
                                bool isRectangle = checkRectangle(image, rectangle);
                                if (isRectangle){
                                    if (square > max_size){
                                        max_size = square;
                                        max_rectangle = rectangle;
                                    }
                                }
                            }

                            start_y++;
                            topPixelColor = definePixelColor(image, start_x, start_y);
                            leftPixelColor = definePixelColor(image, start_x - 1, start_y);
                        }
                        start_y = y;
                    }
                    pixels_map[start_y][start_x] = 1;
                    start_x++;

                    currentPixelColor = definePixelColor(image, start_x, start_y);
                    topPixelColor = definePixelColor(image, start_x, start_y+1);
                    leftPixelColor = definePixelColor(image, start_x - 1, start_y);
                }
            }
        }
    }
    return max_rectangle;
}

void setRectangleColor(Rectangle rectangle, Color color, Image* image){
    for (int x = rectangle.leftBottom.x; x <= rectangle.rightTop.x; ++x){
        pngSetPixel(x, rectangle.leftBottom.y, image, color);
        pngSetPixel(x, rectangle.rightTop.y, image, color);
    }

    for (int y = rectangle.leftBottom.y; y <= rectangle.rightTop.y; ++y){
        pngSetPixel(rectangle.leftBottom.x, y, image, color);
        pngSetPixel(rectangle.rightTop.x, y, image, color);
    }
}

void read_png_file(char *file_name, struct Image* image) {
    int x,y;
    png_byte header[8];    // 8 is the maximum size that can be checked

    /* open file and test for it being a png */
    FILE *fp = fopen(file_name, "rb");
    if (!fp){
        // Some error handling: file could not be opened
    }

    fread(header, 1, 8, fp);
    if (png_sig_cmp(header, 0, 8)){
        // Some error handling: file is not recognized as a PNG
    }

    /* initialize stuff */
    image->png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

    if (!image->png_ptr){
        // Some error handling: png_create_read_struct failed
    }

    image->info_ptr = png_create_info_struct(image->png_ptr);
    if (!image->info_ptr){
        // Some error handling: png_create_info_struct failed
    }

    if (setjmp(png_jmpbuf(image->png_ptr))){
        // Some error handling: error during init_io
    }

    png_init_io(image->png_ptr, fp);
    png_set_sig_bytes(image->png_ptr, 8);

    png_read_info(image->png_ptr, image->info_ptr);

    image->width = png_get_image_width(image->png_ptr, image->info_ptr);
    image->height = png_get_image_height(image->png_ptr, image->info_ptr);
    image->color_type = png_get_color_type(image->png_ptr, image->info_ptr);
    image->bit_depth = png_get_bit_depth(image->png_ptr, image->info_ptr);

    image->number_of_passes = png_set_interlace_handling(image->png_ptr);
    png_read_update_info(image->png_ptr, image->info_ptr);

    /* read file */
    if (setjmp(png_jmpbuf(image->png_ptr))){
        // Some error handling: error during read_image
    }

    image->row_pointers = (png_bytep *) malloc(sizeof(png_bytep) * image->height);
    for (y = 0; y < image->height; y++)
        image->row_pointers[y] = (png_byte *) malloc(png_get_rowbytes(image->png_ptr, image->info_ptr));

    png_read_image(image->png_ptr, image->row_pointers);

    fclose(fp);
}

void write_png_file(char *file_name, struct Image* image) {
    int x,y;
    /* create file */
    FILE *fp = fopen(file_name, "wb");
    if (!fp){
        // Some error handling: file could not be opened
    }

    /* initialize stuff */
    image->png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

    if (!image->png_ptr){
        // Some error handling: png_create_write_struct failed
    }

    image->info_ptr = png_create_info_struct(image->png_ptr);
    if (!image->info_ptr){
        // Some error handling: png_create_info_struct failed
    }

    if (setjmp(png_jmpbuf(image->png_ptr))){
        // Some error handling: error during init_io
    }

    png_init_io(image->png_ptr, fp);


    /* write header */
    if (setjmp(png_jmpbuf(image->png_ptr))){
        // Some error handling: error during writing header
    }

    png_set_IHDR(image->png_ptr, image->info_ptr, image->width, image->height,
                 image->bit_depth, image->color_type, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    png_write_info(image->png_ptr, image->info_ptr);


    /* write bytes */
    if (setjmp(png_jmpbuf(image->png_ptr))){
        // Some error handling: error during writing bytes
    }

    png_write_image(image->png_ptr, image->row_pointers);


    /* end write */
    if (setjmp(png_jmpbuf(image->png_ptr))){
        // Some error handling: error during end of write
    }

    png_write_end(image->png_ptr, NULL);

    /* cleanup heap allocation */
    for (y = 0; y < image->height; y++)
        free(image->row_pointers[y]);
    free(image->row_pointers);

    fclose(fp);
}