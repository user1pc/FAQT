
/// test_square_right_of_line_gui.h
/// A win32 app showing a very simple & small example/test of the quadtree square right of line traversal.
/// This isn't well documented and isn't too important to read. Its just for a demo.

#ifndef TEST_SQUARE_RIGHT_OF_LINE_H
#define TEST_SQUARE_RIGHT_OF_LINE_H

#include <stdint.h>

void test_square_right_of_line_gui(uint32_t* pixels, int32_t width, int32_t height,
    int32_t x1, int32_t y1, int32_t x2, int32_t y2,
    int32_t square_x, int32_t square_y, int32_t square_width);

#endif // TEST_SQUARE_RIGHT_OF_LINE_H