
#include "pch.h"
#include "square_right_of_line.h"

#include <random>

typedef struct
{
    int32_t x1, y1, x2, y2;
    int32_t square_x, square_y, square_width;
    bool* pixels;
    int32_t width, height;
}TestSquareRightOfLineCallbackInfo;

bool correct_point_right_of_or_on_line(int64_t x1, int64_t y1, int64_t x2, int64_t y2,
    int64_t point_x, int64_t point_y)
{
    int64_t dx = x2 - x1;
    int64_t dy = y2 - y1;
    int64_t px = point_x - x1;
    int64_t py = point_y - y1;
    int64_t cross = dx * py - dy * px;
    return cross <= 0;
}

bool correct_point_right_of_line(int64_t x1, int64_t y1, int64_t x2, int64_t y2,
    int64_t point_x, int64_t point_y)
{
    int64_t dx = x2 - x1;
    int64_t dy = y2 - y1;
    int64_t px = point_x - x1;
    int64_t py = point_y - y1;
    int64_t cross = dx * py - dy * px;
    return cross < 0;
}

bool correct_square_right_of_line(int64_t x1, int64_t y1, int64_t x2, int64_t y2,
    int64_t square_x, int64_t square_y, int64_t square_width, bool* out_complete_hit)
{
    bool top_left_hit = correct_point_right_of_or_on_line(x1, y1, x2, y2, square_x, square_y);
    bool top_right_hit = correct_point_right_of_or_on_line(x1, y1, x2, y2, square_x + square_width - 1, square_y);
    bool bottom_left_hit = correct_point_right_of_or_on_line(x1, y1, x2, y2, square_x, square_y + square_width - 1);
    bool bottom_right_hit = correct_point_right_of_or_on_line(x1, y1, x2, y2, square_x + square_width - 1, square_y + square_width - 1);

    bool top_left_hit_complete = correct_point_right_of_line(x1, y1, x2, y2, square_x, square_y);
    bool top_right_hit_complete = correct_point_right_of_line(x1, y1, x2, y2, square_x + square_width - 1, square_y);
    bool bottom_left_hit_complete = correct_point_right_of_line(x1, y1, x2, y2, square_x, square_y + square_width - 1);
    bool bottom_right_hit_complete = correct_point_right_of_line(x1, y1, x2, y2, square_x + square_width - 1, square_y + square_width - 1);
    *out_complete_hit = top_left_hit_complete && top_right_hit_complete && bottom_left_hit_complete && bottom_right_hit_complete;
    return top_left_hit || top_right_hit || bottom_left_hit || bottom_right_hit;
}


bool test_square_right_of_line_callback(void* user_data, int32_t square_x, int32_t square_y, int32_t square_width, bool complete_hit)
{
    return true;
}

void test_verify_SquareRightOfLineTester_init(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t square_x, int32_t square_y, int32_t square_width)
{
    SquareRightOfLineTester test_me = SquareRightOfLineTester_init(x1, y1, x2, y2, square_x, square_y, square_width);
    bool complete_hit;
    bool partial_hit = correct_square_right_of_line(x1, y1, x2, y2, square_x, square_y, square_width, &complete_hit);
    if (partial_hit)
    {
        EXPECT_LE(test_me.far_value, 0);
        if (complete_hit)
        {
            EXPECT_LT(test_me.near_value, 0);
        }
    }
    else
    {
        EXPECT_GT(test_me.far_value, 0);
    }
}

TEST(SquareRightOfLine, InitializationTest) {
    int32_t very_large = 1024 * 1024 * 1024;
    int32_t very_small = -1024 * 1024 * 1024;

    // Test 8 lines "flat" and outside the bounds of the square
    test_verify_SquareRightOfLineTester_init(0, 0, 0, 100, 10, 10, 32);
    test_verify_SquareRightOfLineTester_init(0, 100, 0, 0, 10, 10, 32);
    test_verify_SquareRightOfLineTester_init(0, 100, 100, 100, 10, 10, 32);
    test_verify_SquareRightOfLineTester_init(100, 100, 0, 100, 10, 10, 32);
    test_verify_SquareRightOfLineTester_init(100, 100, 100, 0, 10, 10, 32);
    test_verify_SquareRightOfLineTester_init(100, 0, 100, 100, 10, 10, 32);
    test_verify_SquareRightOfLineTester_init(100, 0, 0, 0, 10, 10, 32);
    test_verify_SquareRightOfLineTester_init(0, 0, 100, 0, 10, 10, 32);

    // Test the same 8 lines with shight tilts
    test_verify_SquareRightOfLineTester_init(1, 2, 0, 100, 10, 10, 32);
    test_verify_SquareRightOfLineTester_init(3, 100, 0, 1, 10, 10, 32);
    test_verify_SquareRightOfLineTester_init(-2, 100, 100, 98, 10, 10, 32);
    test_verify_SquareRightOfLineTester_init(101, 98, -2, 99, 10, 10, 32);
    test_verify_SquareRightOfLineTester_init(101, 90, 100, 1, 10, 10, 32);
    test_verify_SquareRightOfLineTester_init(110, 3, 95, 96, 10, 10, 32);
    test_verify_SquareRightOfLineTester_init(103, 1, 4, 5, 10, 10, 32);
    test_verify_SquareRightOfLineTester_init(-1, 1, 102, 1, 10, 10, 32);

    // Test some verticle/horizontal lines intersecting the square
    test_verify_SquareRightOfLineTester_init(20, 2, 20, 21, 12, 11, 32);
    test_verify_SquareRightOfLineTester_init(20, 21, 20, 2, 12, 11, 32);
    test_verify_SquareRightOfLineTester_init(-1, 20, 20, 21, 12, 11, 32);
    test_verify_SquareRightOfLineTester_init(20, 21 , -1, 20, 12, 11, 32);

    // Some nice diagonals
    test_verify_SquareRightOfLineTester_init(1, 0, 0, 1, 100, 100, 256);
    test_verify_SquareRightOfLineTester_init(1, 0, 0, 1, -130, -129, 256);
    test_verify_SquareRightOfLineTester_init(0, 1, 1, 0, 100, 100, 256);
    test_verify_SquareRightOfLineTester_init(0, 1, 1, 0, -128, -131, 256);
    test_verify_SquareRightOfLineTester_init(-1, 0, 0, 1, 100, 100, 256);
    test_verify_SquareRightOfLineTester_init(-1, 0, 0, 1, -129, -130, 128);
    test_verify_SquareRightOfLineTester_init(0, 1, -1, 0, 100, 100, 256);
    test_verify_SquareRightOfLineTester_init(0, 1, -1, 0, -131, -128, 512);

    // Some scary large values
    test_verify_SquareRightOfLineTester_init(very_large, 0, 0, very_large, 0, 0, 1024 * 1024);
    test_verify_SquareRightOfLineTester_init(0, very_large, very_large, 0, 0, 0, 1024 * 1024);
    test_verify_SquareRightOfLineTester_init(very_small, 0, 0, very_large, 0, 0, 1024 * 1024);
    test_verify_SquareRightOfLineTester_init(0, very_large, very_small, 0, 0, 0, 1024 * 1024);
    test_verify_SquareRightOfLineTester_init(0, 0, very_large, very_large, 0, 0, 1024 * 1024);

    // Lines that lie exactly on the square
    test_verify_SquareRightOfLineTester_init(0, 0, 0, 1024 * 1024, 0, 0, 1024 * 1024);
    test_verify_SquareRightOfLineTester_init(1024 * 1024, 0, 0, 0, 0, 0, 1024 * 1024);
    test_verify_SquareRightOfLineTester_init(0, 0, 1024 * 1024, 0, 0, 0, 1024 * 1024);
    test_verify_SquareRightOfLineTester_init(1024 * 1024, 0, 0, 0, 0, 0, 1024 * 1024);
    test_verify_SquareRightOfLineTester_init(0, 1024 * 1024, 1024 * 1024, 1024 * 1024, 0, 0, 1024 * 1024);
    test_verify_SquareRightOfLineTester_init(1024 * 1024, 1024 * 1024, 0, 1024 * 1024, 0, 0, 1024 * 1024);
    test_verify_SquareRightOfLineTester_init(1024 * 1024, 1024 * 1024, 1024 * 1024, 0, 0, 0, 1024 * 1024);
    test_verify_SquareRightOfLineTester_init(1024 * 1024, 0, 1024 * 1024, 1024 * 1024, 0, 0, 1024 * 1024);
    test_verify_SquareRightOfLineTester_init(0, 1024 * 1024, 0, 0, 0, 0, 1024 * 1024);
    test_verify_SquareRightOfLineTester_init(0, 0, 0, 1024 * 1024, 0, 0, 1024 * 1024);
    test_verify_SquareRightOfLineTester_init(0, 0, 1024 * 1024, 1024 * 1024, 0, 0, 1024 * 1024);
    test_verify_SquareRightOfLineTester_init(1024 * 1024, 1024 * 1024, 0, 0, 0, 0, 1024 * 1024);
    test_verify_SquareRightOfLineTester_init(0, 1024 * 1024, 1024 * 1024, 0, 0, 0, 1024 * 1024);
    test_verify_SquareRightOfLineTester_init(1024 * 1024, 0, 0, 1024 * 1024, 0, 0, 1024 * 1024);

    // Buncha random
    srand(0);
    for (int i = 0; i < 100000; i++)
    {
        int width_log2 = rand() % 29;
        int32_t square_width = 1 << width_log2;
        int32_t random_range = square_width * 2;
        int32_t x1 = (rand() % random_range) - (random_range / 2);
        int32_t y1 = (rand() % random_range) - (random_range / 2);
        int32_t x2 = (rand() % random_range) - (random_range / 2);
        int32_t y2 = (rand() % random_range) - (random_range / 2);
        int32_t square_x = (rand() % random_range) - (random_range / 2);
        int32_t square_y = (rand() % random_range) - (random_range / 2);
        test_verify_SquareRightOfLineTester_init(x1, y1, x2, y2, square_x, square_y, square_width);
    }
}

bool test_verify_SquareRightOfLineTeseter_GoForward_recursive(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t square_x, int32_t square_y, int32_t square_width,
    SquareRightOfLineTester tester, int depth)
{
    SquareRightOfLineTesterResult result;
    SquareRightOfLineTester next_step;
    bool partial_hit, complete_hit;
    int32_t half_width = square_width / 2;
    if (square_width > 1)
    {
        // Verify top-left works properly
        result = SquareRightOfLineTester_TestNextStep(&tester, 0, 0, depth);
        partial_hit = correct_square_right_of_line(x1, y1, x2, y2, square_x, square_y, half_width, &complete_hit);
        if (result.partial_hit != partial_hit)
            return false;
        next_step = tester;
        SquareRightOfLineTester_StepInFromResult(&next_step, &result);
        test_verify_SquareRightOfLineTeseter_GoForward_recursive(x1, y1, x2, y2, square_x, square_y, half_width, next_step, depth + 1);

        // Verify top-right works properly
        result = SquareRightOfLineTester_TestNextStep(&tester, 1, 0, depth);
        partial_hit = correct_square_right_of_line(x1, y1, x2, y2, square_x + half_width, square_y, half_width, &complete_hit);
        if (result.partial_hit != partial_hit)
            return false;
        next_step = tester;
        SquareRightOfLineTester_StepInFromResult(&next_step, &result);
        test_verify_SquareRightOfLineTeseter_GoForward_recursive(x1, y1, x2, y2, square_x + half_width, square_y, half_width, next_step, depth + 1);

        // Verify bottom-left works properly
        result = SquareRightOfLineTester_TestNextStep(&tester, 0, 1, depth);
        partial_hit = correct_square_right_of_line(x1, y1, x2, y2, square_x, square_y + half_width, half_width, &complete_hit);
        if (result.partial_hit != partial_hit)
            return false;
        next_step = tester;
        SquareRightOfLineTester_StepInFromResult(&next_step, &result);
        test_verify_SquareRightOfLineTeseter_GoForward_recursive(x1, y1, x2, y2, square_x, square_y + half_width, half_width, next_step, depth + 1);

        // Verify top-right works properly
        result = SquareRightOfLineTester_TestNextStep(&tester, 1, 1, depth);
        partial_hit = correct_square_right_of_line(x1, y1, x2, y2, square_x + half_width, square_y + half_width, half_width, &complete_hit);
        if (result.partial_hit != partial_hit)
            return false;
        next_step = tester;
        SquareRightOfLineTester_StepInFromResult(&next_step, &result);
        test_verify_SquareRightOfLineTeseter_GoForward_recursive(x1, y1, x2, y2, square_x + half_width, square_y + half_width, half_width, next_step, depth + 1);
    }
    return true;
}

void test_verify_SquareRightOfLineTeseter_GoForward(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t square_x, int32_t square_y, int32_t square_width)
{
    SquareRightOfLineTester tester = SquareRightOfLineTester_init(x1, y1, x2, y2, square_x, square_y, square_width);
    int depth = 1;
    bool success = test_verify_SquareRightOfLineTeseter_GoForward_recursive(x1, y1, x2, y2, square_x, square_y, square_width, tester, depth);
    EXPECT_TRUE(success);
}

TEST(SquareRightOfLine, GoForwardTest) {
    int32_t very_large = 1024 * 1024 * 1024;
    int32_t very_small = -1024 * 1024 * 1024;

    // Test 8 lines "flat" and outside the bounds of the square
    test_verify_SquareRightOfLineTeseter_GoForward(0, 0, 0, 100, 10, 10, 32);
    test_verify_SquareRightOfLineTeseter_GoForward(0, 100, 0, 0, 10, 10, 32);
    test_verify_SquareRightOfLineTeseter_GoForward(0, 100, 100, 100, 10, 10, 32);
    test_verify_SquareRightOfLineTeseter_GoForward(100, 100, 0, 100, 10, 10, 32);
    test_verify_SquareRightOfLineTeseter_GoForward(100, 100, 100, 0, 10, 10, 32);
    test_verify_SquareRightOfLineTeseter_GoForward(100, 0, 100, 100, 10, 10, 32);
    test_verify_SquareRightOfLineTeseter_GoForward(100, 0, 0, 0, 10, 10, 32);
    test_verify_SquareRightOfLineTeseter_GoForward(0, 0, 100, 0, 10, 10, 32);

    // Test the same 8 lines with shight tilts
    test_verify_SquareRightOfLineTeseter_GoForward(1, 2, 0, 100, 10, 10, 32);
    test_verify_SquareRightOfLineTeseter_GoForward(3, 100, 0, 1, 10, 10, 32);
    test_verify_SquareRightOfLineTeseter_GoForward(-2, 100, 100, 98, 10, 10, 32);
    test_verify_SquareRightOfLineTeseter_GoForward(101, 98, -2, 99, 10, 10, 32);
    test_verify_SquareRightOfLineTeseter_GoForward(101, 90, 100, 1, 10, 10, 32);
    test_verify_SquareRightOfLineTeseter_GoForward(110, 3, 95, 96, 10, 10, 32);
    test_verify_SquareRightOfLineTeseter_GoForward(103, 1, 4, 5, 10, 10, 32);
    test_verify_SquareRightOfLineTeseter_GoForward(-1, 1, 102, 1, 10, 10, 32);

    // Test some verticle/horizontal lines intersecting the square
    test_verify_SquareRightOfLineTeseter_GoForward(20, 2, 20, 21, 12, 11, 32);
    test_verify_SquareRightOfLineTeseter_GoForward(20, 21, 20, 2, 12, 11, 32);
    test_verify_SquareRightOfLineTeseter_GoForward(-1, 20, 20, 21, 12, 11, 32);
    test_verify_SquareRightOfLineTeseter_GoForward(20, 21, -1, 20, 12, 11, 32);

    // Some nice diagonals
    test_verify_SquareRightOfLineTeseter_GoForward(1, 0, 0, 1, 100, 100, 256);
    test_verify_SquareRightOfLineTeseter_GoForward(1, 0, 0, 1, -130, -129, 256);
    test_verify_SquareRightOfLineTeseter_GoForward(0, 1, 1, 0, 100, 100, 256);
    test_verify_SquareRightOfLineTeseter_GoForward(0, 1, 1, 0, -128, -131, 256);
    test_verify_SquareRightOfLineTeseter_GoForward(-1, 0, 0, 1, 100, 100, 256);
    test_verify_SquareRightOfLineTeseter_GoForward(-1, 0, 0, 1, -129, -130, 128);
    test_verify_SquareRightOfLineTeseter_GoForward(0, 1, -1, 0, 100, 100, 256);
    test_verify_SquareRightOfLineTeseter_GoForward(0, 1, -1, 0, -131, -128, 512);

    // Some scary large values
    test_verify_SquareRightOfLineTeseter_GoForward(very_large, 0, 0, very_large, 0, 0, 1024);
    test_verify_SquareRightOfLineTeseter_GoForward(0, very_large, very_large, 0, 0, 0, 1024);
    test_verify_SquareRightOfLineTeseter_GoForward(very_small, 0, 0, very_large, 0, 0, 1024);
    test_verify_SquareRightOfLineTeseter_GoForward(0, very_large, very_small, 0, 0, 0, 1024);
    test_verify_SquareRightOfLineTeseter_GoForward(0, 0, very_large, very_large, 0, 0, 1024);

    // Lines that lie exactly on the square
    test_verify_SquareRightOfLineTeseter_GoForward(0, 0, 0, 1024 * 1024, 0, 0, 1024);
    test_verify_SquareRightOfLineTeseter_GoForward(1024 * 1024, 0, 0, 0, 0, 0, 1024);
    test_verify_SquareRightOfLineTeseter_GoForward(0, 0, 1024 * 1024, 0, 0, 0, 1024);
    test_verify_SquareRightOfLineTeseter_GoForward(1024 * 1024, 0, 0, 0, 0, 0, 1024);
    test_verify_SquareRightOfLineTeseter_GoForward(0, 1024 * 1024, 1024 * 1024, 1024 * 1024, 0, 0, 1024);
    test_verify_SquareRightOfLineTeseter_GoForward(1024 * 1024, 1024 * 1024, 0, 1024 * 1024, 0, 0, 1024);
    test_verify_SquareRightOfLineTeseter_GoForward(1024 * 1024, 1024 * 1024, 1024 * 1024, 0, 0, 0, 1024);
    test_verify_SquareRightOfLineTeseter_GoForward(1024 * 1024, 0, 1024 * 1024, 1024 * 1024, 0, 0, 1024);
    test_verify_SquareRightOfLineTeseter_GoForward(0, 1024 * 1024, 0, 0, 0, 0, 1024);
    test_verify_SquareRightOfLineTeseter_GoForward(0, 0, 0, 1024 * 1024, 0, 0, 1024);
    test_verify_SquareRightOfLineTeseter_GoForward(0, 0, 1024 * 1024, 1024 * 1024, 0, 0, 1024);
    test_verify_SquareRightOfLineTeseter_GoForward(1024 * 1024, 1024 * 1024, 0, 0, 0, 0, 1024);
    test_verify_SquareRightOfLineTeseter_GoForward(0, 1024 * 1024, 1024 * 1024, 0, 0, 0, 1024);
    test_verify_SquareRightOfLineTeseter_GoForward(1024 * 1024, 0, 0, 1024 * 1024, 0, 0, 1024);

    // Lines that are near the edges of the square (vertical)
    test_verify_SquareRightOfLineTeseter_GoForward(-1, 0, -1, 1, 0, 0, 8);
    test_verify_SquareRightOfLineTeseter_GoForward(-1, 0, -1, -1, 0, 0, 8);
    test_verify_SquareRightOfLineTeseter_GoForward(0, 0, 0, 1, 0, 0, 8);
    test_verify_SquareRightOfLineTeseter_GoForward(0, 0, 0, -1, 0, 0, 8);
    test_verify_SquareRightOfLineTeseter_GoForward(1, 0, 1, 1, 0, 0, 8);
    test_verify_SquareRightOfLineTeseter_GoForward(1, 0, 1, -1, 0, 0, 8);
    test_verify_SquareRightOfLineTeseter_GoForward(7, 0, 7, 1, 0, 0, 8);
    test_verify_SquareRightOfLineTeseter_GoForward(7, 0, 7, -1, 0, 0, 8);
    test_verify_SquareRightOfLineTeseter_GoForward(8, 0, 8, 1, 0, 0, 8);
    test_verify_SquareRightOfLineTeseter_GoForward(8, 0, 8, -1, 0, 0, 8);
    test_verify_SquareRightOfLineTeseter_GoForward(9, 0, 9, 1, 0, 0, 8);
    test_verify_SquareRightOfLineTeseter_GoForward(9, 0, 9, -1, 0, 0, 8);

    // Lines that are near the edges of the square (horizontal)
    test_verify_SquareRightOfLineTeseter_GoForward(0, -1, 1, -1, 0, 0, 8);
    test_verify_SquareRightOfLineTeseter_GoForward(0, -1, -1, -1, 0, 0, 8);
    test_verify_SquareRightOfLineTeseter_GoForward(0, 0, 1, 0, 0, 0, 8);
    test_verify_SquareRightOfLineTeseter_GoForward(0, 0, -1, 0, 0, 0, 8);
    test_verify_SquareRightOfLineTeseter_GoForward(0, 1, 1, 1, 0, 0, 8);
    test_verify_SquareRightOfLineTeseter_GoForward(0, 1, -1, 1, 0, 0, 8);
    test_verify_SquareRightOfLineTeseter_GoForward(0, 7, 1, 7, 0, 0, 8);
    test_verify_SquareRightOfLineTeseter_GoForward(0, 7, -1, 7, 0, 0, 8);
    test_verify_SquareRightOfLineTeseter_GoForward(0, 8, 1, 8, 0, 0, 8);
    test_verify_SquareRightOfLineTeseter_GoForward(0, 8, -1, 8, 0, 0, 8);
    test_verify_SquareRightOfLineTeseter_GoForward(0, 9, 1, 9, 0, 0, 8);
    test_verify_SquareRightOfLineTeseter_GoForward(0, 9, -1, 9, 0, 0, 8);

    // Lines that are near the middle of the square (vertical)
    test_verify_SquareRightOfLineTeseter_GoForward(3, 0, 3, 1, 0, 0, 8);
    test_verify_SquareRightOfLineTeseter_GoForward(3, 0, 3, -1, 0, 0, 8);
    test_verify_SquareRightOfLineTeseter_GoForward(4, 0, 4, 1, 0, 0, 8);
    test_verify_SquareRightOfLineTeseter_GoForward(4, 0, 4, -1, 0, 0, 8);
    test_verify_SquareRightOfLineTeseter_GoForward(5, 0, 5, 1, 0, 0, 8);
    test_verify_SquareRightOfLineTeseter_GoForward(5, 0, 5, -1, 0, 0, 8);

    // Lines that are near the middle of the square (horizontal)
    test_verify_SquareRightOfLineTeseter_GoForward(0, 3, 1, 3, 0, 0, 8);
    test_verify_SquareRightOfLineTeseter_GoForward(0, 3, -1, 3, 0, 0, 8);
    test_verify_SquareRightOfLineTeseter_GoForward(0, 4, 1, 4, 0, 0, 8);
    test_verify_SquareRightOfLineTeseter_GoForward(0, 4, -1, 4, 0, 0, 8);
    test_verify_SquareRightOfLineTeseter_GoForward(0, 5, 1, 5, 0, 0, 8);
    test_verify_SquareRightOfLineTeseter_GoForward(0, 5, -1, 5, 0, 0, 8);



    // Lines that are near the quarter-middles of the square (vertical)
    test_verify_SquareRightOfLineTeseter_GoForward(3, 0, 3, 1, 0, 0, 16);
    test_verify_SquareRightOfLineTeseter_GoForward(3, 0, 3, -1, 0, 0, 16);
    test_verify_SquareRightOfLineTeseter_GoForward(4, 0, 4, 1, 0, 0, 16);
    test_verify_SquareRightOfLineTeseter_GoForward(4, 0, 4, -1, 0, 0, 16);
    test_verify_SquareRightOfLineTeseter_GoForward(5, 0, 5, 1, 0, 0, 16);
    test_verify_SquareRightOfLineTeseter_GoForward(5, 0, 5, -1, 0, 0, 16);
    test_verify_SquareRightOfLineTeseter_GoForward(11, 0, 11, 1, 0, 0, 16);
    test_verify_SquareRightOfLineTeseter_GoForward(11, 0, 11, -1, 0, 0, 16);
    test_verify_SquareRightOfLineTeseter_GoForward(12, 0, 12, 1, 0, 0, 16);
    test_verify_SquareRightOfLineTeseter_GoForward(12, 0, 12, -1, 0, 0, 16);
    test_verify_SquareRightOfLineTeseter_GoForward(13, 0, 13, 1, 0, 0, 16);
    test_verify_SquareRightOfLineTeseter_GoForward(13, 0, 13, -1, 0, 0, 16);

    // Lines that are near the quarter-middles of the square (horizontal)
    test_verify_SquareRightOfLineTeseter_GoForward(0, 3, 1, 3, 0, 0, 16);
    test_verify_SquareRightOfLineTeseter_GoForward(0, 3, -1, 3, 0, 0, 16);
    test_verify_SquareRightOfLineTeseter_GoForward(0, 4, 1, 4, 0, 0, 16);
    test_verify_SquareRightOfLineTeseter_GoForward(0, 4, -1, 4, 0, 0, 16);
    test_verify_SquareRightOfLineTeseter_GoForward(0, 5, 1, 5, 0, 0, 16);
    test_verify_SquareRightOfLineTeseter_GoForward(0, 5, -1, 5, 0, 0, 16);
    test_verify_SquareRightOfLineTeseter_GoForward(0, 11, 1, 11, 0, 0, 16);
    test_verify_SquareRightOfLineTeseter_GoForward(0, 11, -1, 11, 0, 0, 16);
    test_verify_SquareRightOfLineTeseter_GoForward(0, 12, 1, 12, 0, 0, 16);
    test_verify_SquareRightOfLineTeseter_GoForward(0, 12, -1, 12, 0, 0, 16);
    test_verify_SquareRightOfLineTeseter_GoForward(0, 13, 1, 13, 0, 0, 16);
    test_verify_SquareRightOfLineTeseter_GoForward(0, 13, -1, 13, 0, 0, 16);

    // Buncha random
    srand(1000);
    for (int i = 0; i < 100; i++)
    {
        int width_log2 = rand() % 10;
        int32_t square_width = 1 << width_log2;
        int32_t random_range = square_width * 2;
        int32_t x1 = (rand() % random_range) - (random_range / 2);
        int32_t y1 = (rand() % random_range) - (random_range / 2);
        int32_t x2 = (rand() % random_range) - (random_range / 2);
        int32_t y2 = (rand() % random_range) - (random_range / 2);
        int32_t square_x = (rand() % random_range) - (random_range / 2);
        int32_t square_y = (rand() % random_range) - (random_range / 2);
        test_verify_SquareRightOfLineTeseter_GoForward(x1, y1, x2, y2, square_x, square_y, square_width);
    }
}


bool test_verify_SquareRightOfLineTeseter_GoBack_recursive(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t square_x, int32_t square_y, int32_t square_width,
    SquareRightOfLineTester tester_correct, SquareRightOfLineTester *p_tester, int depth)
{
    SquareRightOfLineTesterResult result;
    SquareRightOfLineTester next_step;
    bool partial_hit, complete_hit;
    int32_t half_width = square_width / 2;
    srand(1);
    if (square_width > 1)
    {
        int dx = rand() % 2;
        int dy = rand() % 2;
        result = SquareRightOfLineTester_TestNextStep(p_tester, dx, dy, depth);
        next_step = *p_tester;
        SquareRightOfLineTester_StepInFromResult(p_tester, &result);
        test_verify_SquareRightOfLineTeseter_GoBack_recursive(x1, y1, x2, y2, square_x + (dx * half_width), square_y + (dy * half_width), half_width,
            next_step, p_tester, depth + 1);
        SquareRightOfLineTester_StepOutFromResult(p_tester, dx, dy, depth);
        if (tester_correct.dx != p_tester->dx)
            return false;
        if (tester_correct.dy != p_tester->dy)
            return false;
        if (tester_correct.far_value != p_tester->far_value)
            return false;
        if (tester_correct.near_value != p_tester->near_value)
            return false;
        if (tester_correct.normal_x_sign != p_tester->normal_x_sign)
            return false;
        if (tester_correct.normal_y_sign != p_tester->normal_y_sign)
            return false;
    }
    return true;
}


void test_verify_SquareRightOfLineTeseter_GoBack(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t square_x, int32_t square_y, int32_t square_width)
{
    SquareRightOfLineTester tester = SquareRightOfLineTester_init(x1, y1, x2, y2, square_x, square_y, square_width);
    int depth = 1;
    test_verify_SquareRightOfLineTeseter_GoBack_recursive(x1, y1, x2, y2, square_x, square_y, square_width, tester, &tester, depth);
}

TEST(SquareRightOfLine, GoBackwardsTest) {
    int32_t very_large = 1024 * 1024 * 1024;
    int32_t very_small = -1024 * 1024 * 1024;

    // Test 8 lines "flat" and outside the bounds of the square
    test_verify_SquareRightOfLineTeseter_GoForward(0, 0, 0, 100, 10, 10, 32);
    test_verify_SquareRightOfLineTeseter_GoForward(0, 100, 0, 0, 10, 10, 32);
    test_verify_SquareRightOfLineTeseter_GoForward(0, 100, 100, 100, 10, 10, 32);
    test_verify_SquareRightOfLineTeseter_GoForward(100, 100, 0, 100, 10, 10, 32);
    test_verify_SquareRightOfLineTeseter_GoForward(100, 100, 100, 0, 10, 10, 32);
    test_verify_SquareRightOfLineTeseter_GoForward(100, 0, 100, 100, 10, 10, 32);
    test_verify_SquareRightOfLineTeseter_GoForward(100, 0, 0, 0, 10, 10, 32);
    test_verify_SquareRightOfLineTeseter_GoForward(0, 0, 100, 0, 10, 10, 32);

    // Test the same 8 lines with shight tilts
    test_verify_SquareRightOfLineTeseter_GoForward(1, 2, 0, 100, 10, 10, 32);
    test_verify_SquareRightOfLineTeseter_GoForward(3, 100, 0, 1, 10, 10, 32);
    test_verify_SquareRightOfLineTeseter_GoForward(-2, 100, 100, 98, 10, 10, 32);
    test_verify_SquareRightOfLineTeseter_GoForward(101, 98, -2, 99, 10, 10, 32);
    test_verify_SquareRightOfLineTeseter_GoForward(101, 90, 100, 1, 10, 10, 32);
    test_verify_SquareRightOfLineTeseter_GoForward(110, 3, 95, 96, 10, 10, 32);
    test_verify_SquareRightOfLineTeseter_GoForward(103, 1, 4, 5, 10, 10, 32);
    test_verify_SquareRightOfLineTeseter_GoForward(-1, 1, 102, 1, 10, 10, 32);

    // Test some verticle/horizontal lines intersecting the square
    test_verify_SquareRightOfLineTeseter_GoForward(20, 2, 20, 21, 12, 11, 32);
    test_verify_SquareRightOfLineTeseter_GoForward(20, 21, 20, 2, 12, 11, 32);
    test_verify_SquareRightOfLineTeseter_GoForward(-1, 20, 20, 21, 12, 11, 32);
    test_verify_SquareRightOfLineTeseter_GoForward(20, 21, -1, 20, 12, 11, 32);

    // Some nice diagonals
    test_verify_SquareRightOfLineTeseter_GoForward(1, 0, 0, 1, 100, 100, 256);
    test_verify_SquareRightOfLineTeseter_GoForward(1, 0, 0, 1, -130, -129, 256);
    test_verify_SquareRightOfLineTeseter_GoForward(0, 1, 1, 0, 100, 100, 256);
    test_verify_SquareRightOfLineTeseter_GoForward(0, 1, 1, 0, -128, -131, 256);
    test_verify_SquareRightOfLineTeseter_GoForward(-1, 0, 0, 1, 100, 100, 256);
    test_verify_SquareRightOfLineTeseter_GoForward(-1, 0, 0, 1, -129, -130, 128);
    test_verify_SquareRightOfLineTeseter_GoForward(0, 1, -1, 0, 100, 100, 256);
    test_verify_SquareRightOfLineTeseter_GoForward(0, 1, -1, 0, -131, -128, 512);

    // Some scary large values
    test_verify_SquareRightOfLineTeseter_GoForward(very_large, 0, 0, very_large, 0, 0, 1024);
    test_verify_SquareRightOfLineTeseter_GoForward(0, very_large, very_large, 0, 0, 0, 1024);
    test_verify_SquareRightOfLineTeseter_GoForward(very_small, 0, 0, very_large, 0, 0, 1024);
    test_verify_SquareRightOfLineTeseter_GoForward(0, very_large, very_small, 0, 0, 0, 1024);
    test_verify_SquareRightOfLineTeseter_GoForward(0, 0, very_large, very_large, 0, 0, 1024);

    // Lines that lie exactly on the square
    test_verify_SquareRightOfLineTeseter_GoForward(0, 0, 0, 1024 * 1024, 0, 0, 1024);
    test_verify_SquareRightOfLineTeseter_GoForward(1024 * 1024, 0, 0, 0, 0, 0, 1024);
    test_verify_SquareRightOfLineTeseter_GoForward(0, 0, 1024 * 1024, 0, 0, 0, 1024);
    test_verify_SquareRightOfLineTeseter_GoForward(1024 * 1024, 0, 0, 0, 0, 0, 1024);
    test_verify_SquareRightOfLineTeseter_GoForward(0, 1024 * 1024, 1024 * 1024, 1024 * 1024, 0, 0, 1024);
    test_verify_SquareRightOfLineTeseter_GoForward(1024 * 1024, 1024 * 1024, 0, 1024 * 1024, 0, 0, 1024);
    test_verify_SquareRightOfLineTeseter_GoForward(1024 * 1024, 1024 * 1024, 1024 * 1024, 0, 0, 0, 1024);
    test_verify_SquareRightOfLineTeseter_GoForward(1024 * 1024, 0, 1024 * 1024, 1024 * 1024, 0, 0, 1024);
    test_verify_SquareRightOfLineTeseter_GoForward(0, 1024 * 1024, 0, 0, 0, 0, 1024);
    test_verify_SquareRightOfLineTeseter_GoForward(0, 0, 0, 1024 * 1024, 0, 0, 1024);
    test_verify_SquareRightOfLineTeseter_GoForward(0, 0, 1024 * 1024, 1024 * 1024, 0, 0, 1024);
    test_verify_SquareRightOfLineTeseter_GoForward(1024 * 1024, 1024 * 1024, 0, 0, 0, 0, 1024);
    test_verify_SquareRightOfLineTeseter_GoForward(0, 1024 * 1024, 1024 * 1024, 0, 0, 0, 1024);
    test_verify_SquareRightOfLineTeseter_GoForward(1024 * 1024, 0, 0, 1024 * 1024, 0, 0, 1024);

    // Buncha random
    srand(1000);
    for (int i = 0; i < 100; i++)
    {
        int width_log2 = rand() % 10;
        int32_t square_width = 1 << width_log2;
        int32_t random_range = square_width * 2;
        int32_t x1 = (rand() % random_range) - (random_range / 2);
        int32_t y1 = (rand() % random_range) - (random_range / 2);
        int32_t x2 = (rand() % random_range) - (random_range / 2);
        int32_t y2 = (rand() % random_range) - (random_range / 2);
        int32_t square_x = (rand() % random_range) - (random_range / 2);
        int32_t square_y = (rand() % random_range) - (random_range / 2);
        test_verify_SquareRightOfLineTeseter_GoForward(x1, y1, x2, y2, square_x, square_y, square_width);
    }
}
