
#include "pch.h"
#include "square_right_of_line_traverser.h"

#include <random>

typedef struct
{
    int32_t square_x, square_y, square_width;
    bool go_forward;
} TestSquareRightOfLine_NodeAction;

typedef struct
{
    uint8_t* pixels;
    int32_t width, height;
    TestSquareRightOfLine_NodeAction* actions;
    int32_t num_actions;
}TestSquareRightOfLineTraverser_FullCoverageInfo;

void test_square_right_of_line_traversal_fill_square(uint8_t* pixels, int32_t width, int32_t height,
    int32_t square_x, int32_t square_y, int32_t square_width, uint8_t color)
{
    for (int32_t y = square_y; y < square_y + square_width; y++)
    {
        for (int32_t x = square_x; x < square_x + square_width; x++)
        {
            if (y >= 0 && y < height && x >= 0 && x < width)
                pixels[y * width + x] = color;
        }
    }
}


bool correct_point_right_of_or_on_line_traverser(int64_t x1, int64_t y1, int64_t x2, int64_t y2,
    int64_t point_x, int64_t point_y)
{
    int64_t dx = x2 - x1;
    int64_t dy = y2 - y1;
    int64_t px = point_x - x1;
    int64_t py = point_y - y1;
    int64_t cross = dx * py - dy * px;
    return cross <= 0;
}

bool correct_point_right_of_line_traverser(int64_t x1, int64_t y1, int64_t x2, int64_t y2,
    int64_t point_x, int64_t point_y)
{
    int64_t dx = x2 - x1;
    int64_t dy = y2 - y1;
    int64_t px = point_x - x1;
    int64_t py = point_y - y1;
    int64_t cross = dx * py - dy * px;
    return cross < 0;
}

bool correct_square_right_of_line_traverser(int64_t x1, int64_t y1, int64_t x2, int64_t y2,
    int64_t square_x, int64_t square_y, int64_t square_width, bool* out_complete_hit)
{
    bool top_left_hit = correct_point_right_of_or_on_line_traverser(x1, y1, x2, y2, square_x, square_y);
    bool top_right_hit = correct_point_right_of_or_on_line_traverser(x1, y1, x2, y2, square_x + square_width, square_y);
    bool bottom_left_hit = correct_point_right_of_or_on_line_traverser(x1, y1, x2, y2, square_x, square_y + square_width);
    bool bottom_right_hit = correct_point_right_of_or_on_line_traverser(x1, y1, x2, y2, square_x + square_width, square_y + square_width);

    bool top_left_hit_complete = correct_point_right_of_line_traverser(x1, y1, x2, y2, square_x, square_y);
    bool top_right_hit_complete = correct_point_right_of_line_traverser(x1, y1, x2, y2, square_x + square_width, square_y);
    bool bottom_left_hit_complete = correct_point_right_of_line_traverser(x1, y1, x2, y2, square_x, square_y + square_width);
    bool bottom_right_hit_complete = correct_point_right_of_line_traverser(x1, y1, x2, y2, square_x + square_width, square_y + square_width);
    *out_complete_hit = top_left_hit_complete && top_right_hit_complete && bottom_left_hit_complete && bottom_right_hit_complete;
    return top_left_hit || top_right_hit || bottom_left_hit || bottom_right_hit;
}

void test_square_right_of_line_traverser_fill_complete_correct(int32_t x1, int32_t y1, int32_t x2, int32_t y2,
    int32_t square_x, int32_t square_y,int32_t square_width_log2,
    uint8_t* pixels, int32_t width, int32_t height)
{
    int32_t original_square_width = 1 << square_width_log2;
    square_width_log2 = 0;
    for (int32_t square_width = 1; square_width < original_square_width; square_width *= 2)
    {
        for (int32_t test_y = square_y; test_y + (square_width - 1) < square_y + original_square_width; test_y += square_width)
        {
            for (int32_t test_x = square_x; test_x + (square_width - 1) < square_x + original_square_width; test_x += square_width)
            {
                bool complete_hit;
                bool partial_hit = correct_square_right_of_line_traverser(x1, y1, x2, y2, test_x, test_y, square_width, &complete_hit);
                if (complete_hit)
                    test_square_right_of_line_traversal_fill_square(pixels, width, height, test_x, test_y, square_width, square_width_log2);
            }
        }
        square_width_log2++;
    }
}

bool test_square_right_of_line_traverser_fill_complete_callback(void *user_data, int32_t square_x, int32_t square_y, int32_t square_width, int32_t square_width_log2, bool complete_hit)
{
    TestSquareRightOfLineTraverser_FullCoverageInfo* p_info = (TestSquareRightOfLineTraverser_FullCoverageInfo*)user_data;
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
    SquareRightOfLineTraverse(x1, y1, x2, y2, square_x, square_y, square_width_log2, test_square_right_of_line_traverser_fill_complete_callback, &info);

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


























void test_square_right_of_line_traverser_fill_random_correct(int32_t x1, int32_t y1, int32_t x2, int32_t y2,
    int32_t square_x, int32_t square_y, int32_t square_width_log2,
    uint8_t* pixels, int32_t width, int32_t height,
    TestSquareRightOfLine_NodeAction *out_actions, int32_t alloc_num_actions, int32_t *out_num_actions)
{
    int32_t original_square_width = 1 << square_width_log2;
    square_width_log2 = 0;
    *out_num_actions = 0;
    for (int32_t square_width = 1; square_width < original_square_width; square_width *= 2)
    {
        for (int32_t test_y = square_y; test_y + (square_width - 1) < square_y + original_square_width; test_y += square_width)
        {
            for (int32_t test_x = square_x; test_x + (square_width - 1) < square_x + original_square_width; test_x += square_width)
            {
                bool complete_hit;
                bool partial_hit = correct_square_right_of_line_traverser(x1, y1, x2, y2, test_x, test_y, square_width, &complete_hit);

                bool go_deeper = false;
                if (partial_hit)
                    go_deeper = rand() % 2;
                if (square_width == 1)
                    go_deeper = false;

                TestSquareRightOfLine_NodeAction new_action;
                new_action.go_forward = go_deeper;
                new_action.square_x = test_x;
                new_action.square_y = test_y;
                new_action.square_width = square_width;
                out_actions[*out_num_actions] = new_action;
                out_num_actions[0] += 1;

                if (!go_deeper && partial_hit)
                    test_square_right_of_line_traversal_fill_square(pixels, width, height, test_x, test_y, square_width, square_width_log2);
            }
        }
        square_width_log2++;
    }
}

bool test_square_right_of_line_traverser_fill_random_callback(void* user_data, int32_t square_x, int32_t square_y, int32_t square_width, int32_t square_width_log2, bool complete_hit)
{
    TestSquareRightOfLineTraverser_FullCoverageInfo* p_info = (TestSquareRightOfLineTraverser_FullCoverageInfo*)user_data;
    // Search for an action
    for (int32_t i = 0; i < p_info->num_actions; i++)
    {
        if (p_info->actions[i].square_x == square_x &&
            p_info->actions[i].square_y == square_y &&
            p_info->actions[i].square_width == square_width)
        {
            if (p_info->actions[i].go_forward)
            {
                return true;
            }
            else
            {
                test_square_right_of_line_traversal_fill_square(p_info->pixels, p_info->width, p_info->height, square_x, square_y, square_width, square_width_log2);
                return false;
            }
        }
    }
    return true;
}

void test_square_right_of_line_traverser_fill_random(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t square_x, int32_t square_y, int32_t square_width_log2)
{
    int32_t square_width = 1 << square_width_log2;
    int32_t width = square_x + square_width * 2;
    int32_t height = square_y + square_width * 2;
    uint8_t* correct_pixels = new uint8_t[width * height];
    uint8_t* test_pixels = new uint8_t[width * height];
    memset(test_pixels, 255, width * height);
    memset(correct_pixels, 255, width * height);
    TestSquareRightOfLine_NodeAction *actions = new TestSquareRightOfLine_NodeAction[1024*1024*64];
    int32_t num_actions = 0;
    test_square_right_of_line_traverser_fill_random_correct(x1, y1, x2, y2, square_x, square_y, square_width_log2, correct_pixels, width, height, actions, 1024*1024*64, &num_actions);
    TestSquareRightOfLineTraverser_FullCoverageInfo info;
    info.pixels = test_pixels;
    info.width = width;
    info.height = height;
    info.actions = actions;
    info.num_actions = num_actions;
    SquareRightOfLineTraverse(x1, y1, x2, y2, square_x, square_y, square_width_log2, test_square_right_of_line_traverser_fill_random_callback, &info);

    bool all_success = true;
    for (int32_t y = 0; y < height; y++)
    {
        for (int32_t x = 0; x < width; x++)
        {
            uint8_t test_px = test_pixels[y * width + x];
            uint8_t correct_px = correct_pixels[y * width + x];
            if (test_px != correct_px)
            {
                all_success = false;
            }
        }
    }
    delete[] actions;
    delete[] correct_pixels;
    delete[] test_pixels;
    EXPECT_TRUE(all_success);
}

TEST(SquareRightOfLineTraverser, RandomTraversal)
{
    test_square_right_of_line_traverser_fill_random(-1, 1000, 1000, 1000, 0, 0, 3);
    test_square_right_of_line_traverser_fill_random(0, 0, 1, 1, 0, 0, 3);
    test_square_right_of_line_traverser_fill_random(0, 0, 1, 2, 0, 0, 8);

    int32_t width = 1024;
    srand(4);
    for (int i = 0; i < 100; i++)
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
        test_square_right_of_line_traverser_fill_random(x1, y1, x2, y2, square_x, square_y, square_width_log2);
    }
}
