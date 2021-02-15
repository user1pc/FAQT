
#include "square_right_of_line.h"
#include "square_right_of_line_traverser.h"

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
    int64_t square_x, int64_t square_y, int64_t square_width, bool *out_complete_hit)
{
    bool top_left_hit = correct_point_right_of_or_on_line(x1, y1, x2, y2, square_x, square_y);
    bool top_right_hit = correct_point_right_of_or_on_line(x1, y1, x2, y2, square_x + square_width, square_y);
    bool bottom_left_hit = correct_point_right_of_or_on_line(x1, y1, x2, y2, square_x, square_y + square_width);
    bool bottom_right_hit = correct_point_right_of_or_on_line(x1, y1, x2, y2, square_x + square_width, square_y + square_width);

    bool top_left_hit_complete = correct_point_right_of_line(x1, y1, x2, y2, square_x, square_y);
    bool top_right_hit_complete = correct_point_right_of_line(x1, y1, x2, y2, square_x + square_width, square_y);
    bool bottom_left_hit_complete = correct_point_right_of_line(x1, y1, x2, y2, square_x, square_y + square_width);
    bool bottom_right_hit_complete = correct_point_right_of_line(x1, y1, x2, y2, square_x + square_width, square_y + square_width);
    *out_complete_hit = top_left_hit_complete && top_right_hit_complete && bottom_left_hit_complete && bottom_right_hit_complete;
    return top_left_hit || top_right_hit || bottom_left_hit || bottom_right_hit;
}


uint32_t fill_color = (255 << 24) | 255;
uint32_t depth_0_color = (255 << 24) | (255 << 16) | (255 << 8) | (255 << 0);
uint32_t depth_1_color = (255 << 24) | (150 << 16) | (150 << 8) | (150 << 0);
uint32_t depth_2_color = (255 << 24) | (85 << 16) | (85 << 8) | (85 << 0);
uint32_t depth_3_color = (255 << 24) | (30 << 16) | (30 << 8) | (30 << 0);

void fill_square_outline(uint32_t* pixels, int32_t width, int32_t height, int32_t square_x, int32_t square_y, int32_t square_width, uint32_t color)
{
    for (int32_t y = square_y; y < square_y + square_width; y++)
    {
        for (int32_t x = square_x; x < square_x + square_width; x++)
        {
            pixels[((height - 1) - y) * width + x] = color;
        }
    }
}

void test_square_right_of_line_gui_recursive(uint32_t* pixels, int32_t width, int32_t height,
    int32_t x1, int32_t y1, int32_t x2, int32_t y2,
    int32_t square_x, int32_t square_y, int32_t square_width,
    SquareRightOfLineTester *p_tester, int depth)
{
    int64_t half_width = square_width / 2;
    if (square_width == 1)
    {
        pixels[((height - 1) - square_y) * width + square_x] = fill_color;
        return;
    }

    // Values being tested
    SquareRightOfLineTesterResult top_left_hit_result_test = SquareRightOfLineTester_TestNextStep(p_tester, 0, 0, depth);
    SquareRightOfLineTesterResult top_right_hit_result_test = SquareRightOfLineTester_TestNextStep(p_tester, 1, 0, depth);
    SquareRightOfLineTesterResult bottom_left_hit_result_test = SquareRightOfLineTester_TestNextStep(p_tester, 0, 1, depth);
    SquareRightOfLineTesterResult bottom_right_hit_result_test = SquareRightOfLineTester_TestNextStep(p_tester, 1, 1, depth);

    // Values assumed correct
    bool top_left_complete_hit;
    bool top_left_partial_hit = correct_square_right_of_line(x1, y1, x2, y2, square_x, square_y, half_width, &top_left_complete_hit);
    bool top_right_complete_hit;
    bool top_right_partial_hit = correct_square_right_of_line(x1, y1, x2, y2, square_x + half_width, square_y, half_width, &top_right_complete_hit);
    bool bottom_left_complete_hit;
    bool bottom_left_partial_hit = correct_square_right_of_line(x1, y1 , x2, y2, square_x, square_y + half_width, half_width, &bottom_left_complete_hit);
    bool bottom_right_complete_hit;
    bool bottom_right_partial_hit = correct_square_right_of_line(x1, y1, x2, y2, square_x + half_width, square_y + half_width, half_width, &bottom_right_complete_hit);

    if (top_left_complete_hit != top_left_hit_result_test.complete_hit ||
        top_left_partial_hit != top_left_hit_result_test.partial_hit)
    {
        // Test failure!
        __debugbreak();
    }
    if (top_right_complete_hit != top_right_hit_result_test.complete_hit ||
        top_right_partial_hit != top_right_hit_result_test.partial_hit)
    {
        // Test failure!
        __debugbreak();
    }

    if (bottom_left_complete_hit != bottom_left_hit_result_test.complete_hit ||
        bottom_left_partial_hit != bottom_left_hit_result_test.partial_hit)
    {
        // Test failure!
        __debugbreak();
    }
    if (bottom_right_complete_hit != bottom_right_hit_result_test.complete_hit ||
        bottom_right_partial_hit != bottom_right_hit_result_test.partial_hit)
    {
        // Test failure!
        __debugbreak();
    }

    if (top_left_partial_hit && half_width >= 1)
    {
        SquareRightOfLineTester_StepInFromResult(p_tester, &top_left_hit_result_test);
        test_square_right_of_line_gui_recursive(pixels, width, height, x1, y1, x2, y2,
            square_x, square_y, half_width, p_tester, depth + 1);
        SquareRightOfLineTester_StepOutFromResult(p_tester, 0, 0, depth);

        if (top_left_complete_hit && square_width >= 256)
            fill_square_outline(pixels, width, height, square_x, square_y, half_width, depth_0_color);
        else if (top_left_complete_hit && square_width >= 128)
            fill_square_outline(pixels, width, height, square_x, square_y, half_width, depth_1_color);
        else if (top_left_complete_hit && square_width >= 64)
            fill_square_outline(pixels, width, height, square_x, square_y, half_width, depth_2_color);
        else if (top_left_complete_hit && square_width >= 32)
            fill_square_outline(pixels, width, height, square_x, square_y, half_width, depth_3_color);
    }
    if (top_right_partial_hit && half_width >= 1)
    {
        SquareRightOfLineTester_StepInFromResult(p_tester, &top_right_hit_result_test);
        test_square_right_of_line_gui_recursive(pixels, width, height, x1, y1, x2, y2,
            square_x + half_width, square_y, half_width, p_tester, depth + 1);
        SquareRightOfLineTester_StepOutFromResult(p_tester, 1, 0, depth);
        if (top_right_complete_hit && square_width >= 256)
            fill_square_outline(pixels, width, height, square_x + half_width, square_y, half_width, depth_0_color);
        else if (top_right_complete_hit && square_width >= 128)
            fill_square_outline(pixels, width, height, square_x + half_width, square_y, half_width, depth_1_color);
        else if (top_right_complete_hit && square_width >= 64)
            fill_square_outline(pixels, width, height, square_x + half_width, square_y, half_width, depth_2_color);
        else if (top_right_complete_hit && square_width >= 32)
            fill_square_outline(pixels, width, height, square_x + half_width, square_y, half_width, depth_3_color);
    }


    if (bottom_left_partial_hit && half_width >= 1)
    {
        SquareRightOfLineTester_StepInFromResult(p_tester, &bottom_left_hit_result_test);
        test_square_right_of_line_gui_recursive(pixels, width, height, x1, y1, x2, y2,
            square_x, square_y + half_width, half_width, p_tester, depth + 1);
        SquareRightOfLineTester_StepOutFromResult(p_tester, 0, 1, depth);
        if (bottom_left_complete_hit && square_width >= 256)
            fill_square_outline(pixels, width, height, square_x, square_y + half_width, half_width, depth_0_color);
        else if (bottom_left_complete_hit && square_width >= 128)
            fill_square_outline(pixels, width, height, square_x, square_y + half_width, half_width, depth_1_color);
        else if (bottom_left_complete_hit && square_width >= 64)
            fill_square_outline(pixels, width, height, square_x, square_y + half_width, half_width, depth_2_color);
        else if (bottom_left_complete_hit && square_width >= 32)
            fill_square_outline(pixels, width, height, square_x, square_y + half_width, half_width, depth_3_color);
    }
    if (bottom_right_partial_hit && half_width >= 1)
    {
        SquareRightOfLineTester_StepInFromResult(p_tester, &bottom_right_hit_result_test);
        test_square_right_of_line_gui_recursive(pixels, width, height, x1, y1, x2, y2,
            square_x + half_width, square_y + half_width, half_width, p_tester, depth + 1);
        SquareRightOfLineTester_StepOutFromResult(p_tester, 1, 1, depth);
        if (bottom_right_complete_hit && square_width >= 256)
            fill_square_outline(pixels, width, height, square_x + half_width, square_y + half_width, half_width, depth_0_color);
        else if (bottom_right_complete_hit && square_width >= 128)
            fill_square_outline(pixels, width, height, square_x + half_width, square_y + half_width, half_width, depth_1_color);
        else if (bottom_right_complete_hit && square_width >= 64)
            fill_square_outline(pixels, width, height, square_x + half_width, square_y + half_width, half_width, depth_2_color);
        else if (bottom_right_complete_hit && square_width >= 32)
            fill_square_outline(pixels, width, height, square_x + half_width, square_y + half_width, half_width, depth_3_color);
    }
}

typedef struct
{
    int32_t x1, y1, x2, y2;
    uint32_t* pixels;
    int32_t width, height;
} TestSquareRightOfLineGUIStruct;

bool test_square_right_of_line_gui_callback(void* user_data, int32_t square_x, int32_t square_y, int32_t square_width, int32_t depth, bool complete_hit)
{
    TestSquareRightOfLineGUIStruct* p_info = (TestSquareRightOfLineGUIStruct*)user_data;

    bool is_complete_hit;
    bool is_partial_hit = correct_square_right_of_line(p_info->x1, p_info->y1, p_info->x2, p_info->y2, square_x, square_y, square_width, &is_complete_hit);

    if (square_width == 1)
    {
        p_info->pixels[((p_info->height - 1) - square_y) * p_info->width + square_x] = fill_color;
    }

    if (!is_partial_hit)
    {
        // Test failure!
        //__debugbreak();
        int g = 0; // TODO
    }
    if (complete_hit != is_complete_hit)
    {
        // Test failure!
        //__debugbreak();
        int g = 0; // todo
    }

    if (complete_hit && square_width >= 256)
        fill_square_outline(p_info->pixels, p_info->width, p_info->height, square_x, square_y, square_width, depth_0_color);
    else if (complete_hit && square_width >= 128)
        fill_square_outline(p_info->pixels, p_info->width, p_info->height, square_x, square_y, square_width, depth_1_color);
    else if (complete_hit && square_width >= 64)
        fill_square_outline(p_info->pixels, p_info->width, p_info->height, square_x, square_y, square_width, depth_2_color);
    else if (complete_hit && square_width >= 32)
        fill_square_outline(p_info->pixels, p_info->width, p_info->height, square_x, square_y, square_width, depth_3_color);

    if (complete_hit && square_width >= 32)
        return false;
    else
        return true;
}

void test_square_right_of_line_gui(uint32_t* pixels, int32_t width, int32_t height,
    int32_t x1, int32_t y1, int32_t x2, int32_t y2,
    int32_t square_x, int32_t square_y, int32_t square_width_log2)
{
    //SquareRightOfLineTester tester = SquareRightOfLineTester_init(x1, y1, x2, y2, square_x, square_y, square_width);
    //test_square_right_of_line_gui_recursive(pixels, width, height, x1, y1, x2, y2, square_x, square_y, square_width, &tester);
    TestSquareRightOfLineGUIStruct info;
    info.x1 = x1;
    info.y1 = y1;
    info.x2 = x2;
    info.y2 = y2;
    info.pixels = pixels;
    info.width = width;
    info.height = height;
    SquareRightOfLineTraversePreOrder(x1, y1, x2, y2, square_x, square_y, square_width_log2, test_square_right_of_line_gui_callback, &info);
}
