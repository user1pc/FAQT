
#include "pch.h"
/*
#include <stdint.h>
#include <math.h>
#include <random>

template <typename T>
bool point_right_of_or_on_line(T x1, T y1, T x2, T y2,
    T point_x, T point_y)
{
    T dx = x2 - x1;
    T dy = y2 - y1;
    T px = point_x - x1;
    T py = point_y - y1;
    T cross = dx * py - dy * px;
    return cross <= 0;
}

template <typename T>
bool point_right_of_line(T x1, T y1, T x2, T y2,
    T point_x, T point_y)
{
    T dx = x2 - x1;
    T dy = y2 - y1;
    T px = point_x - x1;
    T py = point_y - y1;
    T cross = dx * py - dy * px;
    return cross < 0;
}

bool point_in_or_right_of_lines(int64_t x, int64_t y, int64_t *x1s, int64_t *y1s, int64_t *x2s, int64_t *y2s, int num_lines, int64_t scale)
{
    for (int i = 0; i < num_lines; i++)
    {
        if (!point_right_of_or_on_line(x1s[i] * scale, y1s[i] * scale, x2s[i] * scale, y2s[i] * scale, x * scale, y * scale))
            return false;
    }
    return true;
}

double calculate_relative_area(int64_t square_x, int64_t square_y, int64_t square_width,
    int64_t* x1s, int64_t* y1s, int64_t *x2s, int64_t *y2s, int num_lines)
{
    int64_t scale = 256;
    square_x *= scale;
    square_y *= scale;
    square_width *= scale;

    int num_points_in = 0, num_points_out = 0;
    srand(1);
    for (int i = 0; i < 10000; i++)
    {
        int64_t test_x = (rand() % square_width) + square_x;
        int64_t test_y = (rand() % square_width) + square_y;
        if (point_in_or_right_of_lines(test_x, test_y, x1s, y1s, x2s, y2s, num_lines, scale))
            num_points_in++;
        else
            num_points_out++;
    }
    return (double)num_points_in / (double)num_points_out;
}


void test_square_right_of_line_area_fill_complete_correct(int32_t x1, int32_t y1, int32_t x2, int32_t y2,
    int32_t square_x, int32_t square_y, int32_t square_width_log2,
    uint8_t* pixels, int32_t width, int32_t height)
{
    for (int32_t y = 0; y < height; y++)
    {
        for (int32_t x = 0; x < width; x++)
        {
            int64_t x1_ = x1;
            int64_t y1_ = y1;
            int64_t x2_ = x2;
            int64_t y2_ = y2;
            double area_f = calculate_relative_area(x, y, 1, &x1_, &y1_, &x2_, &y2_, 1);
            int32_t area = (int32_t)(255.0 * area_f);
            area = (area > 255) ? 255 : area;
            area = (area < 0) ? 0 : area;
            pixels[y * width + x] = (uint8_t)area;
        }
    }
}

bool test_square_right_of_line_area_fill_complete_callback(void* user_data, int32_t square_x, int32_t square_y, int32_t square_width, int32_t depth, bool complete_hit)
{
    TestSquareRightOfLineTraverser_FullCoverageInfo* p_info = (TestSquareRightOfLineTraverser_FullCoverageInfo*)user_data;
    int32_t square_width_log2 = p_info->original_width_log2 - (depth - 1);
    if (complete_hit)
    {
        test_square_right_of_line_traversal_fill_square(p_info->pixels, p_info->width, p_info->height, square_x, square_y, square_width, square_width_log2);
        return false;
    }
    else
    {
        return true;
    }
}

void test_square_right_of_line_traverser_fill_complete(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t square_x, int32_t square_y, int32_t square_width_log2)
{
    int32_t square_width = 1 << square_width_log2;
    int32_t width = square_x + square_width * 2;
    int32_t height = square_y + square_width * 2;
    uint8_t* correct_pixels = new uint8_t[width * height];
    uint8_t* test_pixels = new uint8_t[width * height];
    memset(test_pixels, 255, width * height);
    memset(correct_pixels, 255, width * height);
    test_square_right_of_line_traverser_fill_complete_correct(x1, y1, x2, y2, square_x, square_y, square_width_log2, correct_pixels, width, height);
    TestSquareRightOfLineTraverser_FullCoverageInfo info;
    info.pixels = test_pixels;
    info.width = width;
    info.height = height;
    info.actions = nullptr;
    info.num_actions = 0;
    info.original_width_log2 = square_width_log2 - 1;
    SquareRightOfLineTraversePreOrder(x1, y1, x2, y2, square_x, square_y, square_width_log2, test_square_right_of_line_traverser_fill_complete_callback, &info);

    for (int32_t y = 0; y < height; y++)
    {
        for (int32_t x = 0; x < width; x++)
        {
            uint8_t test_px = test_pixels[y * width + x];
            uint8_t correct_px = correct_pixels[y * width + x];
            EXPECT_EQ(test_px, correct_px);
        }
    }
}

TEST(SquareRightOfLineTraverser, CompleteTraversal)
{
    test_square_right_of_line_traverser_fill_complete(-1, 1000, 1000, 1000, 0, 0, 3);
    test_square_right_of_line_traverser_fill_complete(0, 0, 1, 1, 0, 0, 3);
    test_square_right_of_line_traverser_fill_complete(0, 0, 1, 2, 0, 0, 8);

    int32_t width = 1024;
    for (int i = 0; i < 1000; i++)
    {
        int32_t x1 = rand() % 256;
        int32_t y1 = rand() % 256;
        int32_t x2 = rand() % 256;
        int32_t y2 = rand() % 256;
        int32_t square_width_log2 = rand() % 8;
        if (square_width_log2 <= 0)
            square_width_log2 = 1;
        int32_t square_width = 1 << square_width_log2;

        int32_t unit_size = width / square_width;
        int32_t square_x = (rand() % unit_size) * square_width;
        int32_t square_y = (rand() % unit_size) * square_width;
        test_square_right_of_line_traverser_fill_complete(x1, y1, x2, y2, square_x, square_y, square_width_log2);
    }
}
*/