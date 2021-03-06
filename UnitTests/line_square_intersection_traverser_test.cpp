
#include "pch.h"
#include "line_square_intersection_traverser.h"
#include "draw_line.h"

#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif
#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#endif

#include <random>

void get_line_bounds(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t square_width, bool line_includes_endpoints,
    int32_t* out_x_min, int32_t* out_x_max, int32_t* out_y_min, int32_t* out_y_max)
{
    int32_t dx = x2 - x1;
    int32_t dy = y2 - y1;
    int32_t endpoint1_x = x1 / square_width;
    int32_t endpoint1_y = y1 / square_width;
    int32_t endpoint2_x = x2 / square_width;
    int32_t endpoint2_y = y2 / square_width;
    if (dx > 0)
    {
        if (line_includes_endpoints)
        {
            if ((x2 % square_width) == 0)
                endpoint2_x--;
        }
        else
        {
            if ((x2 % square_width) == 0)
                endpoint2_x--;
            endpoint2_x--;
            endpoint1_x++;
        }
    }
    else if (dx < 0)
    {
        if (line_includes_endpoints)
        {
            if ((x1 % square_width) == 0)
                endpoint1_x--;
        }
        else
        {
            if ((x1 % square_width) == 0)
                endpoint1_x--;
            endpoint1_x--;
            endpoint2_x++;
        }
    }

    if (dy > 0)
    {
        if (line_includes_endpoints)
        {
            if ((y2 % square_width) == 0)
                endpoint2_y--;
        }
        else
        {
            if ((y2 % square_width) == 0)
                endpoint2_y--;
            endpoint2_y--;
            endpoint1_y++;
        }
    }
    else if (dy < 0)
    {
        if (line_includes_endpoints)
        {
            if ((y1 % square_width) == 0)
                endpoint1_y--;
        }
        else
        {
            if ((y1 % square_width) == 0)
                endpoint1_y--;
            endpoint1_y--;
            endpoint2_y++;
        }
    }
    *out_x_min = min(endpoint1_x, endpoint2_x);
    *out_x_max = max(endpoint1_x, endpoint2_x);
    *out_y_min = min(endpoint1_y, endpoint2_y);
    *out_y_max = max(endpoint1_y, endpoint2_y);
}

bool pixel_is_endpoint(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t square_x, int32_t square_y, int32_t square_width, bool line_includes_endpoints)
{
    int32_t dx = x2 - x1;
    int32_t dy = y2 - y1;
    int32_t endpoint1_x = x1 / square_width;
    int32_t endpoint1_y = y1 / square_width;
    int32_t endpoint2_x = x2 / square_width;
    int32_t endpoint2_y = y2 / square_width;
    if (dx > 0)
    {
        if (line_includes_endpoints)
        {
            if ((x2 % square_width) == 0)
                endpoint2_x--;
        }
        else
        {
            if ((x2 % square_width) == 0)
                endpoint2_x--;
            endpoint2_x--;
            endpoint1_x++;
        }
    }
    else if (dx < 0)
    {
        if (line_includes_endpoints)
        {
            if ((x1 % square_width) == 0)
                endpoint1_x--;
        }
        else
        {
            if ((x1 % square_width) == 0)
                endpoint1_x--;
            endpoint1_x--;
            endpoint2_x++;
        }
    }

    if (dy > 0)
    {
        if (line_includes_endpoints)
        {
            if ((y2 % square_width) == 0)
                endpoint2_y--;
        }
        else
        {
            if ((y2 % square_width) == 0)
                endpoint2_y--;
            endpoint2_y--;
            endpoint1_y++;
        }
    }
    else if (dy < 0)
    {
        if (line_includes_endpoints)
        {
            if ((y1 % square_width) == 0)
                endpoint1_y--;
        }
        else
        {
            if ((y1 % square_width) == 0)
                endpoint1_y--;
            endpoint1_y--;
            endpoint2_y++;
        }
    }
    return (square_x == endpoint1_x && square_y == endpoint1_y) || (square_x == endpoint2_x && square_y == endpoint2_y);
}

bool verify_line_square_intersection_traverser_match_line_algo(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t square_width)
{
    int width = 1024;
    int height = 1024;
    uint32_t* pixels = new uint32_t[width * height];
    memset(pixels, 0, width * height * sizeof(uint32_t));
    drawline_exclude_endpoints(x1, y1, x2, y2, square_width, 1, pixels, width, height);
    int safe_x_begin, safe_x_end, safe_y_begin, safe_y_end;
    get_line_bounds(x1, y1, x2, y2, square_width, false, &safe_x_begin, &safe_x_end, &safe_y_begin, &safe_y_end);
    for (int y = 0; y < height / square_width; y++)
    {
        LineSquareIntersectionTraverser trav = LineSquareIntersectionTraverser_init(x1, y1, x2, y2, 0, y * square_width, square_width, 1, 1);
        for (int x = 0; x < width / square_width; x++)
        {
            if (x == 30 && y == 125)
            {
                int g = 0;
            }
            bool is_hit = pixels[y * width + x] != 0;
            bool test_says_hit = LineSquareIntersectionTraverser_move_get(&trav);
            bool in_checking_bounds = x >= safe_x_begin && x <= safe_x_end &&
                y >= safe_y_begin && y <= safe_y_end;
            in_checking_bounds &= !pixel_is_endpoint(x1, y1, x2, y2, x, y, square_width, false);
            if (x >= safe_x_begin && x < safe_x_end &&
                y >= safe_y_begin && y < safe_y_end)
            {
                if (is_hit != test_says_hit)
                {
                    delete[] pixels;
                    return false;
                }
            }
            LineSquareIntersectionTraverser_move_x(&trav);
        }
    }
    delete[] pixels;
    return true;
}

TEST(LineSquareIntersectionTest, VerifyMatchesLineTraversal) {
    int32_t x1, y1, x2, y2, square_width;
    bool success;
    x1 = 120;
    y1 = 500;
    x2 = 1023 - 120;
    y2 = 600;
    square_width = 4;
    success = verify_line_square_intersection_traverser_match_line_algo(x1, y1, x2, y2, square_width);
    EXPECT_TRUE(success);
    srand(0);
    for (int rand_test_i = 0; rand_test_i < 10000; rand_test_i++)
    {
        int32_t y1 = rand() % 1024;
        int32_t y2 = rand() % 1024;
        success = verify_line_square_intersection_traverser_match_line_algo(x1, y1, x2, y2, square_width);
        success = verify_line_square_intersection_traverser_match_line_algo(x1, y1, x2, y2, 1);
        success = verify_line_square_intersection_traverser_match_line_algo(x1, y1, x2, y2, 2);
        EXPECT_TRUE(success);
    }
}