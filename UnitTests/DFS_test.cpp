
#include "pch.h"
#include "square_right_of_line_DFS.h"
#include "geometry_maths.h"
#include <math.h>
/*
double distance_between_line_and_point(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x, int32_t y)
{
    int32_t dx = x2 - x1;
    int32_t dy = y2 - y1;
    int32_t dx2 = dx * dx;
    int32_t dy2 = dy * dy;
    return (double)abs(dx * (y1 - y) - dy * (x1 - x)) / sqrtl(dx2 + dy2);
}

void closest_point_on_square_to_line(int32_t x1, int32_t y1, int32_t x2, int32_t y2,
    int32_t square_x, int32_t square_y, int32_t square_width,
    int32_t *out_x, int32_t *out_y)
{
    int64_t dx = (int64_t)x2 - (int64_t)x1;
    int64_t dy = (int64_t)y2 - (int64_t)y1;

    int32_t normal_x_sign = (int32_t)dy;
    int32_t normal_y_sign = (int32_t)-dx;

    *out_x = (normal_x_sign >= 0) ? (square_x) : (square_x + square_width - 1);
    *out_y = (normal_y_sign >= 0) ? (square_y) : (square_y + square_width - 1);
}

void verify_line_travseral(uint8_t *pixels, int width, int height,
    int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t square_width)
{
    bool success = true;
    for (int32_t outer_y = 0; outer_y < height; outer_y += 2)
    {
        for (int32_t outer_x = 0; outer_x < width ; outer_x += 2)
        {
            bool complete_hit;
            bool partial_hit = square_right_of_line(x1, y1, x2, y2, outer_x * square_width, outer_y * square_width, square_width * 2, &complete_hit);
            if (partial_hit && !complete_hit)
            {
                for (int32_t inner_y = outer_y; inner_y < outer_y + 2; inner_y++)
                {
                    for (int32_t inner_x = outer_x; inner_x < outer_x + 2; inner_x++)
                    {
                        bool inner_complete_hit;
                        if (square_right_of_line(x1, y1, x2, y2, inner_x * square_width, inner_y * square_width, square_width, &inner_complete_hit))
                        {
                            if (inner_complete_hit)
                            {
                                if (pixels[inner_y * width + inner_x] != 255)
                                    success = false;
                            }
                            else
                            {
                                if (pixels[inner_y * width + inner_x] != 0)
                                    success = false;
                            }
                        }
                        else
                        {
                            if (pixels[inner_y * width + inner_x] != 0)
                                success = false;
                        }
                    }
                }
            }
            else
            {
                if (pixels[outer_y * width + outer_x] != 0)
                    success = false;
            }
        }
    }
    EXPECT_TRUE(success);
}


void test_line_travseral(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t square_width, int32_t num_nodes_wide)
{
    int32_t dx = x2 - x1;
    int32_t dy = y2 - y1;
    bool is_steep = abs(dx) < abs(dy);
    int32_t width = 1024;
    int32_t height = 1024;
    uint8_t* test_pixels = new uint8_t[1024 * 1024];
    memset(test_pixels, 0, 1024 * 1024);
    SquareRightOfLineDFS_LevelTraverser traverser = SquareRightOfLineDFS_LevelTraverser_init(x1, y1, x2, y2, square_width);
    bool keep_looping = true;
    while (keep_looping)
    {
        int32_t x, y;
        bool hit = SquareRightOfLineDFS_LevelTraverser_get(&traverser, &x, &y);
        if (hit && traverser.x >= 0 && traverser.x < width && traverser.y >= 0 && traverser.y < height)
            test_pixels[traverser.y * width + traverser.x] = 255;
        keep_looping = !SquareRightOfLineDFS_LevelTraverser_IsDone(&traverser);
        SquareRightOfLineDFS_LevelTraverser_update_next(&traverser);
    }
    verify_line_travseral(test_pixels, width, height, x1, y1, x2, y2, square_width);
    delete[] test_pixels;
}


TEST(SquareRightOfLineDFS, LevelTest) {
    //test_line_travseral(0, 0, 32, 32, 1, 32);
    test_line_travseral(8, 8, 0, 0, 1, 8);

    test_line_travseral(1, 0, 32, 32, 1, 32);
}
*/