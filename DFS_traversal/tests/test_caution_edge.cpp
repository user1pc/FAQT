
#include "../polygon_caution_edge.h"
#include "gtest/gtest.h"
#include "../../line_traverser/line_traverser.h"
#include "../../line_traverser/draw_line.h"
#include <vector>

typedef struct
{
    uint32_t *image_pixels;
    int image_width, image_height;
    PolygonEdgeTraverserCautionEdge caution_edge;
    uint32_t avoid_color;
    int i;
    int32_t last_x, last_y;
    bool success;
} AutoVerifyCautionEdgeInfo;

void verify_caution_edge_lines_correct_callback(int32_t x, int32_t y, void *user_data)
{
    AutoVerifyCautionEdgeInfo *p_info = (AutoVerifyCautionEdgeInfo*)user_data;
    if (p_info->i == 0)
    {
        p_info->last_x = x;
        p_info->last_y = y;
    }
    else
    {
        if (x != p_info->last_x)
            PolygonEdgeTraverserCautionEdge_move_x(&(p_info->caution_edge));
        if (y != p_info->last_y)
            PolygonEdgeTraverserCautionEdge_move_y(&(p_info->caution_edge));
        p_info->last_x = x;
        p_info->last_y = y;
    }
    bool test_should_avoid = !PolygonEdgeTraverserCautionEdge_get(&(p_info->caution_edge), x, y);
    bool correct_should_avoid = false;
    if (x >= 0 && y >= 0 && x < p_info->image_width && y < p_info->image_height)
        correct_should_avoid = p_info->image_pixels[y * p_info->image_height + x] == p_info->avoid_color;

    if (test_should_avoid != correct_should_avoid)
        p_info->success = false;
    p_info->i++;
}

bool verify_caution_edge_lines_correct_inner(int32_t a_x1, int32_t a_y1, int32_t a_x2, int32_t a_y2,
    int32_t b_x1, int32_t b_y1, int32_t b_x2, int32_t b_y2, int32_t square_width,
    int sandbox_width, int sandbox_height)
{
    int32_t dx = a_x2 - a_x1;
    int32_t dy = a_y2 - a_y1;
    uint32_t *pixels = new uint32_t[sandbox_width * sandbox_height];
    memset(pixels, 0, sandbox_width * sandbox_height * sizeof(uint32_t));
    drawline_include_endpoints(b_x1, b_y1, b_x2, b_y2, square_width, 1, pixels, sandbox_width, sandbox_height);
    AutoVerifyCautionEdgeInfo info;
    info.image_pixels = pixels;
    info.image_width = sandbox_width;
    info.image_height = sandbox_height;
    info.i = 0;
    info.success = true;
    info.avoid_color = 1;
    int32_t ep1x, ep1y, ep2x, ep2y;
    LineTraverser_get_endpoints(a_x1, a_y1, a_x2, a_y2, square_width, &ep1x, &ep1y, &ep2x, &ep2y);
    info.caution_edge = PolygonEdgeTraverserCautionEdge_init(b_x1, b_y1, b_x2, b_y2, ep1x, ep1y,
        square_width, (dx >= 0) ? 1 : -1, (dy >= 0) ? 1 : -1);
    LineTraverser_traverse_include_endpoints(a_x1, a_y1, a_x2, a_y2, square_width, verify_caution_edge_lines_correct_callback, &info);
    delete[] pixels;
    return info.success;
}

bool verify_caution_edge_lines_correct(int32_t a_x1, int32_t a_y1, int32_t a_x2, int32_t a_y2,
    int32_t b_x1, int32_t b_y1, int32_t b_x2, int32_t b_y2, int32_t square_width,
    int sandbox_width, int sandbox_height)
{
    bool success = verify_caution_edge_lines_correct_inner(a_x1, a_y1, a_x2, a_y2,
        b_x1, b_y1, b_x2, b_y2, square_width, sandbox_width, sandbox_height);    
    success &= verify_caution_edge_lines_correct_inner(a_x2, a_y2, a_x1, a_y1,
        b_x1, b_y1, b_x2, b_y2, square_width, sandbox_width, sandbox_height);
    success &= verify_caution_edge_lines_correct_inner(a_x1, a_y1, a_x2, a_y2,
        b_x2, b_y2, b_x1, b_y1, square_width, sandbox_width, sandbox_height);
    success &= verify_caution_edge_lines_correct_inner(a_x2, a_y2, a_x1, a_y1,
        b_x2, b_y2, b_x1, b_y1, square_width, sandbox_width, sandbox_height);

    success &= verify_caution_edge_lines_correct_inner(b_x1, b_y1, b_x2, b_y2,
        a_x1, a_y1, a_x2, a_y2, square_width, sandbox_width, sandbox_height);
    success &= verify_caution_edge_lines_correct_inner(b_x2, b_y2, b_x1, b_y1,
        a_x1, a_y1, a_x2, a_y2, square_width, sandbox_width, sandbox_height);
    success &= verify_caution_edge_lines_correct_inner(b_x1, b_y1, b_x2, b_y2,
        a_x2, a_y2, a_x1, a_y1, square_width, sandbox_width, sandbox_height);
    success &= verify_caution_edge_lines_correct_inner(b_x2, b_y2, b_x1, b_y1,
        a_x2, a_y2, a_x1, a_y1, square_width, sandbox_width, sandbox_height);
    return success;
}

TEST(AutoTests, CautionEdgeTest)
{
    int width = 1024, height = 1024;
    srand(0);
    int32_t a_x1, a_y1, a_x2, a_y2, b_x1, b_y1, b_x2, b_y2;
    int square_width;

    square_width = 4;
    a_x1 = 0;
    a_y1 = 0;
    a_x2 = 16;
    a_y2 = 0;
    b_x1 = 0;
    b_y1 = 0;
    b_x2 = 16;
    b_y2 = 0;
    verify_caution_edge_lines_correct(a_x1, a_y1, a_x2, a_y2, b_x1, b_y1, b_x2, b_y2, square_width,
            width, height);

    square_width = 4;
    a_x1 = 0;
    a_y1 = 0;
    a_x2 = 16;
    a_y2 = 0;
    b_x1 = 0;
    b_y1 = 1;
    b_x2 = 16;
    b_y2 = 1;
    verify_caution_edge_lines_correct(a_x1, a_y1, a_x2, a_y2, b_x1, b_y1, b_x2, b_y2, square_width,
            width, height);

    square_width = 4;
    a_x1 = 0;
    a_y1 = 0;
    a_x2 = 16;
    a_y2 = 0;
    b_x1 = 0;
    b_y1 = 2;
    b_x2 = 16;
    b_y2 = 2;
    verify_caution_edge_lines_correct(a_x1, a_y1, a_x2, a_y2, b_x1, b_y1, b_x2, b_y2, square_width,
            width, height);

    square_width = 4;
    a_x1 = 0;
    a_y1 = 0;
    a_x2 = 16;
    a_y2 = 0;
    b_x1 = 0;
    b_y1 = 3;
    b_x2 = 16;
    b_y2 = 3;
    verify_caution_edge_lines_correct(a_x1, a_y1, a_x2, a_y2, b_x1, b_y1, b_x2, b_y2, square_width,
            width, height);

    square_width = 4;
    a_x1 = 0;
    a_y1 = 0;
    a_x2 = 16;
    a_y2 = 0;
    b_x1 = 16;
    b_y1 = 0;
    b_x2 = 32;
    b_y2 = 0;
    verify_caution_edge_lines_correct(a_x1, a_y1, a_x2, a_y2, b_x1, b_y1, b_x2, b_y2, square_width,
            width, height);
    square_width = 4;
    a_x1 = 0;
    a_y1 = 0;
    a_x2 = 16;
    a_y2 = 0;
    b_x1 = 17;
    b_y1 = 0;
    b_x2 = 32;
    b_y2 = 0;
    verify_caution_edge_lines_correct(a_x1, a_y1, a_x2, a_y2, b_x1, b_y1, b_x2, b_y2, square_width,
            width, height);


    square_width = 4;
    a_x1 = 0;
    a_y1 = 0;
    a_x2 = 0;
    a_y2 = 16;
    b_x1 = 0;
    b_y1 = 0;
    b_x2 = 0;
    b_y2 = 16;
    verify_caution_edge_lines_correct(a_x1, a_y1, a_x2, a_y2, b_x1, b_y1, b_x2, b_y2, square_width,
            width, height);

    square_width = 4;
    a_x1 = 1;
    a_y1 = 0;
    a_x2 = 1;
    a_y2 = 16;
    b_x1 = 1;
    b_y1 = 0;
    b_x2 = 1;
    b_y2 = 16;
    verify_caution_edge_lines_correct(a_x1, a_y1, a_x2, a_y2, b_x1, b_y1, b_x2, b_y2, square_width,
            width, height);

    square_width = 4;
    a_x1 = 2;
    a_y1 = 0;
    a_x2 = 2;
    a_y2 = 16;
    b_x1 = 2;
    b_y1 = 0;
    b_x2 = 2;
    b_y2 = 16;
    verify_caution_edge_lines_correct(a_x1, a_y1, a_x2, a_y2, b_x1, b_y1, b_x2, b_y2, square_width,
            width, height);

    square_width = 4;
    a_x1 = 3;
    a_y1 = 0;
    a_x2 = 3;
    a_y2 = 16;
    b_x1 = 3;
    b_y1 = 0;
    b_x2 = 3;
    b_y2 = 16;
    verify_caution_edge_lines_correct(a_x1, a_y1, a_x2, a_y2, b_x1, b_y1, b_x2, b_y2, square_width,
            width, height);

    square_width = 4;
    a_x1 = 0;
    a_y1 = 0;
    a_x2 = 0;
    a_y2 = 16;
    b_x1 = 0;
    b_y1 = 16;
    b_x2 = 0;
    b_y2 = 32;
    verify_caution_edge_lines_correct(a_x1, a_y1, a_x2, a_y2, b_x1, b_y1, b_x2, b_y2, square_width,
            width, height);

    square_width = 4;
    a_x1 = 0;
    a_y1 = 0;
    a_x2 = 0;
    a_y2 = 16;
    b_x1 = 0;
    b_y1 = 17;
    b_x2 = 0;
    b_y2 = 32;
    verify_caution_edge_lines_correct(a_x1, a_y1, a_x2, a_y2, b_x1, b_y1, b_x2, b_y2, square_width,
            width, height);

    for (int i = 0; i < 1000; i++)
    {
        for (square_width = 1; square_width <= 64; square_width *= 2)
        {
            a_x1 = rand() % width;
            a_y1 = rand() % height;
            a_x2 = rand() % width;
            a_y2 = rand() % height;
            b_x1 = rand() % width;
            b_y1 = rand() % height;
            b_x2 = rand() % width;
            b_y2 = rand() % height;
            verify_caution_edge_lines_correct(a_x1, a_y1, a_x2, a_y2, b_x1, b_y1, b_x2, b_y2, square_width,
            width, height);
        }
    }
}

typedef struct
{
    std::vector<bool> known_correct;
    PolygonEdgeTraverserCautionEdge caution_edge;
    int i;
    int32_t last_x, last_y;
    bool success;
} VerifyCautionEdgeInfo;

void verify_caution_edge_manual_callback(int32_t x, int32_t y, void *user_data)
{
    VerifyCautionEdgeInfo *p_info = (VerifyCautionEdgeInfo*)user_data;
    if (p_info->i == 0)
    {
        p_info->last_x = x;
        p_info->last_y = y;
    }
    else
    {
        if (x != p_info->last_x)
            PolygonEdgeTraverserCautionEdge_move_x(&(p_info->caution_edge));
        if (y != p_info->last_y)
            PolygonEdgeTraverserCautionEdge_move_y(&(p_info->caution_edge));
        p_info->last_x = x;
        p_info->last_y = y;
    }
    bool test_value = PolygonEdgeTraverserCautionEdge_get(&(p_info->caution_edge), x, y);
    bool correct_value = true;
    if (p_info->i < p_info->known_correct.size()) // allows end avoiding many trues at the end of the bool array.
        correct_value = p_info->known_correct[p_info->i];
    if (test_value != correct_value)
        p_info->success = false;
    p_info->i++;
}

bool verify_caution_edge_manual(int32_t a_x1, int32_t a_y1, int32_t a_x2, int32_t a_y2,
    int32_t b_x1, int32_t b_y1, int32_t b_x2, int32_t b_y2,
    int32_t square_width,
    const std::vector<bool> &known_correct)
{
    int32_t dx = a_x2 - a_x1;
    int32_t dy = a_y2 - a_y1;
    VerifyCautionEdgeInfo info;
    info.i = 0;
    int32_t ep1x, ep1y, ep2x, ep2y;
    LineTraverser_get_endpoints(a_x1, a_y1, a_x2, a_y2, square_width, &ep1x, &ep1y, &ep2x, &ep2y);
    info.caution_edge = PolygonEdgeTraverserCautionEdge_init(b_x1, b_y1, b_x2, b_y2, ep1x, ep1y,
        square_width, (dx >= 0) ? 1 : -1, (dy >= 0) ? 1 : -1);
    info.success = true;
    info.known_correct = known_correct;
    LineTraverser_traverse_include_endpoints(a_x1, a_y1, a_x2, a_y2, square_width,
        verify_caution_edge_manual_callback, &info);

    // The same operation with b points swapped must give the same result,
    // so lets see if thats true.
    info.i = 0;
    info.caution_edge = PolygonEdgeTraverserCautionEdge_init(b_x2, b_y2, b_x1, b_y1, ep1x, ep1y,
        square_width, (dx >= 0) ? 1 : -1, (dy >= 0) ? 1 : -1);
    info.known_correct = known_correct;
    LineTraverser_traverse_include_endpoints(a_x1, a_y1, a_x2, a_y2, square_width,
        verify_caution_edge_manual_callback, &info);
    return info.success;
}

bool verify_caution_edge_manual(int32_t a_x1, int32_t a_y1, int32_t a_x2, int32_t a_y2,
    int32_t b_x1, int32_t b_y1, int32_t b_x2, int32_t b_y2,
    int32_t square_width,
    bool all_known_correct)
{
    int32_t dx = a_x2 - a_x1;
    int32_t dy = a_y2 - a_y1;
    VerifyCautionEdgeInfo info;
    info.i = 0;
    int32_t ep1x, ep1y, ep2x, ep2y;
    LineTraverser_get_endpoints(a_x1, a_y1, a_x2, a_y2, square_width, &ep1x, &ep1y, &ep2x, &ep2y);
    info.caution_edge = PolygonEdgeTraverserCautionEdge_init(b_x1, b_y1, b_x2, b_y2, ep1x, ep1y,
        square_width, (dx >= 0) ? 1 : -1, (dy >= 0) ? 1 : -1);
    info.success = true;
    for (int i = 0; i < 1000000; i++)
        info.known_correct.push_back(all_known_correct);
    LineTraverser_traverse_include_endpoints(a_x1, a_y1, a_x2, a_y2, square_width,
        verify_caution_edge_manual_callback, &info);

    // The same operation with b points swapped must give the same result,
    // so lets see if thats true.
    info.i = 0;
    info.caution_edge = PolygonEdgeTraverserCautionEdge_init(b_x2, b_y2, b_x1, b_y1, ep1x, ep1y,
        square_width, (dx >= 0) ? 1 : -1, (dy >= 0) ? 1 : -1);
    LineTraverser_traverse_include_endpoints(a_x1, a_y1, a_x2, a_y2, square_width,
        verify_caution_edge_manual_callback, &info);
    return info.success;
}

TEST(ManualTests, CautionEdgeTest)
{
    int32_t a_x1, a_y1, a_x2, a_y2;
    int32_t b_x1, b_y1, b_x2, b_y2;
    int32_t square_width;
    bool success;
    
    a_x1 = 13;
    a_y1 = 15;
    a_x2 = 27;
    a_y2 = 30;
    b_x1 = 44;
    b_y1 = 38;
    b_x2 = 22;
    b_y2 = 19;
    square_width = 64;
    success = verify_caution_edge_manual(a_x1, a_y1, a_x2, a_y2, b_x1, b_y1, b_x2, b_y2, square_width,
        false);
    EXPECT_TRUE(success);
    square_width = 32;
    success = verify_caution_edge_manual(a_x1, a_y1, a_x2, a_y2, b_x1, b_y1, b_x2, b_y2, square_width,
        std::vector<bool>{false, true, false});
    EXPECT_TRUE(success);
    square_width = 16;
    success = verify_caution_edge_manual(a_x1, a_y1, a_x2, a_y2, b_x1, b_y1, b_x2, b_y2, square_width,
        std::vector<bool>{true, true, false});
    EXPECT_TRUE(success);
    square_width = 8;
    success = verify_caution_edge_manual(a_x1, a_y1, a_x2, a_y2, b_x1, b_y1, b_x2, b_y2, square_width,
        std::vector<bool>{true, true, false, true, false});
    EXPECT_TRUE(success);
    square_width = 4;
    success = verify_caution_edge_manual(a_x1, a_y1, a_x2, a_y2, b_x1, b_y1, b_x2, b_y2, square_width,
        std::vector<bool>{true, true, true, true, false, true, false, true});
    EXPECT_TRUE(success);
    square_width = 2;
    success = verify_caution_edge_manual(a_x1, a_y1, a_x2, a_y2, b_x1, b_y1, b_x2, b_y2, square_width,
        true);
    EXPECT_TRUE(success);
    square_width = 1;
    success = verify_caution_edge_manual(a_x1, a_y1, a_x2, a_y2, b_x1, b_y1, b_x2, b_y2, square_width,
        true);
    EXPECT_TRUE(success);


    a_x1 = 44;
    a_y1 = 38;
    a_x2 = 22;
    a_y2 = 19;
    b_x1 = 13;
    b_y1 = 15;
    b_x2 = 27;
    b_y2 = 30;
    square_width = 64;
    success = verify_caution_edge_manual(a_x1, a_y1, a_x2, a_y2, b_x1, b_y1, b_x2, b_y2, square_width,
        false);
    EXPECT_TRUE(success);
    square_width = 32;
    success = verify_caution_edge_manual(a_x1, a_y1, a_x2, a_y2, b_x1, b_y1, b_x2, b_y2, square_width,
        std::vector<bool>{true, true, false});
    EXPECT_TRUE(success);
    square_width = 16;
    success = verify_caution_edge_manual(a_x1, a_y1, a_x2, a_y2, b_x1, b_y1, b_x2, b_y2, square_width,
        std::vector<bool>{true, true, false});
    EXPECT_TRUE(success);
    square_width = 8;
    success = verify_caution_edge_manual(a_x1, a_y1, a_x2, a_y2, b_x1, b_y1, b_x2, b_y2, square_width,
        std::vector<bool>{true, true, true, false, true, false});
    EXPECT_TRUE(success);
    square_width = 4;
    success = verify_caution_edge_manual(a_x1, a_y1, a_x2, a_y2, b_x1, b_y1, b_x2, b_y2, square_width,
        std::vector<bool>{true, true, true, true, true, true, true, false, true, false, true});
    EXPECT_TRUE(success);
    square_width = 2;
    success = verify_caution_edge_manual(a_x1, a_y1, a_x2, a_y2, b_x1, b_y1, b_x2, b_y2, square_width,
        true);
    EXPECT_TRUE(success);
    square_width = 1;
    success = verify_caution_edge_manual(a_x1, a_y1, a_x2, a_y2, b_x1, b_y1, b_x2, b_y2, square_width,
        true);
    EXPECT_TRUE(success);


    a_x1 = 13;
    a_y1 = 15;
    a_x2 = 27;
    a_y2 = 30;
    b_x1 = 27;
    b_y1 = 30;
    b_x2 = 44;
    b_y2 = 38;
    square_width = 64;
    success = verify_caution_edge_manual(a_x1, a_y1, a_x2, a_y2, b_x1, b_y1, b_x2, b_y2, square_width,
        false);
    EXPECT_TRUE(success);
    square_width = 32;
    success = verify_caution_edge_manual(a_x1, a_y1, a_x2, a_y2, b_x1, b_y1, b_x2, b_y2, square_width,
        std::vector<bool>{false, true, false});
    EXPECT_TRUE(success);
    square_width = 16;
    success = verify_caution_edge_manual(a_x1, a_y1, a_x2, a_y2, b_x1, b_y1, b_x2, b_y2, square_width,
        std::vector<bool>{true, true, false});
    EXPECT_TRUE(success);
    square_width = 8;
    success = verify_caution_edge_manual(a_x1, a_y1, a_x2, a_y2, b_x1, b_y1, b_x2, b_y2, square_width,
        std::vector<bool>{true, true, true, true, false});
    EXPECT_TRUE(success);


    a_x1 = 13;
    a_y1 = 15;
    a_x2 = 27;
    a_y2 = 30;
    b_x1 = 27;
    b_y1 = 30;
    b_x2 = 25;
    b_y2 = 38;
    square_width = 8;
    success = verify_caution_edge_manual(a_x1, a_y1, a_x2, a_y2, b_x1, b_y1, b_x2, b_y2, square_width,
        std::vector<bool>{true, true, true, true, false});
    EXPECT_TRUE(success);


    a_x1 = 13;
    a_y1 = 15;
    a_x2 = 27;
    a_y2 = 30;
    b_x1 = 27;
    b_y1 = 30;
    b_x2 = 20;
    b_y2 = 18;
    square_width = 8;
    success = verify_caution_edge_manual(a_x1, a_y1, a_x2, a_y2, b_x1, b_y1, b_x2, b_y2, square_width,
        std::vector<bool>{true, true, false, false, false});
    EXPECT_TRUE(success);
    square_width = 4;
    success = verify_caution_edge_manual(a_x1, a_y1, a_x2, a_y2, b_x1, b_y1, b_x2, b_y2, square_width,
        std::vector<bool>{true, true, true, true, false, false, false, false});
    EXPECT_TRUE(success);

    a_x1 = 16;
    a_y1 = 16;
    a_x2 = 32;
    a_y2 = 48;
    b_x1 = 36;
    b_y1 = 48;
    b_x2 = 20;
    b_y2 = 16;
    square_width = 8;
    success = verify_caution_edge_manual(a_x1, a_y1, a_x2, a_y2, b_x1, b_y1, b_x2, b_y2, square_width,
        std::vector<bool>{false, true, false, true});
    EXPECT_TRUE(success);
    square_width = 4;
    success = verify_caution_edge_manual(a_x1, a_y1, a_x2, a_y2, b_x1, b_y1, b_x2, b_y2, square_width,
        true);
    EXPECT_TRUE(success);
    square_width = 2;
    success = verify_caution_edge_manual(a_x1, a_y1, a_x2, a_y2, b_x1, b_y1, b_x2, b_y2, square_width,
        true);
    EXPECT_TRUE(success);
    square_width = 1;
    success = verify_caution_edge_manual(a_x1, a_y1, a_x2, a_y2, b_x1, b_y1, b_x2, b_y2, square_width,
        true);
    EXPECT_TRUE(success);

    a_x1 = 36;
    a_y1 = 48;
    a_x2 = 20;
    a_y2 = 16;
    b_x1 = 16;
    b_y1 = 16;
    b_x2 = 32;
    b_y2 = 48;
    square_width = 4;
    success = verify_caution_edge_manual(a_x1, a_y1, a_x2, a_y2, b_x1, b_y1, b_x2, b_y2, square_width,
        true);
    EXPECT_TRUE(success);
    square_width = 2;
    success = verify_caution_edge_manual(a_x1, a_y1, a_x2, a_y2, b_x1, b_y1, b_x2, b_y2, square_width,
        true);
    EXPECT_TRUE(success);
    square_width = 1;
    success = verify_caution_edge_manual(a_x1, a_y1, a_x2, a_y2, b_x1, b_y1, b_x2, b_y2, square_width,
        true);
    EXPECT_TRUE(success);


    a_x1 = 32;
    a_y1 = 16;
    a_x2 = 32-8;
    a_y2 = 48;
    b_x1 = 32;
    b_y1 = 48;
    b_x2 = 32+8;
    b_y2 = 16;
    square_width = 4;
    success = verify_caution_edge_manual(a_x1, a_y1, a_x2, a_y2, b_x1, b_y1, b_x2, b_y2, square_width,
        true);
    EXPECT_TRUE(success);
    square_width = 2;
    success = verify_caution_edge_manual(a_x1, a_y1, a_x2, a_y2, b_x1, b_y1, b_x2, b_y2, square_width,
        true);
    EXPECT_TRUE(success);
    square_width = 1;
    success = verify_caution_edge_manual(a_x1, a_y1, a_x2, a_y2, b_x1, b_y1, b_x2, b_y2, square_width,
        true);
    EXPECT_TRUE(success);


    a_x1 = 16;
    a_y1 = 16;
    a_x2 = 24;
    a_y2 = 8;
    b_x1 = 40;
    b_y1 = 24;
    b_x2 = 32;
    b_y2 = 32;
    square_width = 4;
    success = verify_caution_edge_manual(a_x1, a_y1, a_x2, a_y2, b_x1, b_y1, b_x2, b_y2, square_width,
        true);
    EXPECT_TRUE(success);
    square_width = 2;
    success = verify_caution_edge_manual(a_x1, a_y1, a_x2, a_y2, b_x1, b_y1, b_x2, b_y2, square_width,
        true);
    EXPECT_TRUE(success);
    square_width = 1;
    success = verify_caution_edge_manual(a_x1, a_y1, a_x2, a_y2, b_x1, b_y1, b_x2, b_y2, square_width,
        true);
    EXPECT_TRUE(success);

    a_x1 = 16;
    a_y1 = 16;
    a_x2 = 32;
    a_y2 = 32;
    b_x1 = 40;
    b_y1 = 24;
    b_x2 = 24;
    b_y2 = 8;
    square_width = 4;
    success = verify_caution_edge_manual(a_x1, a_y1, a_x2, a_y2, b_x1, b_y1, b_x2, b_y2, square_width,
        true);
    EXPECT_TRUE(success);
    square_width = 2;
    success = verify_caution_edge_manual(a_x1, a_y1, a_x2, a_y2, b_x1, b_y1, b_x2, b_y2, square_width,
        true);
    EXPECT_TRUE(success);
    square_width = 1;
    success = verify_caution_edge_manual(a_x1, a_y1, a_x2, a_y2, b_x1, b_y1, b_x2, b_y2, square_width,
        true);
    EXPECT_TRUE(success);

    a_x1 = 0;
    a_y1 = 0;
    a_x2 = 16;
    a_y2 = 16;
    b_x1 = 7;
    b_y1 = 9;
    b_x2 = 0;
    b_y2 = 16;
    square_width = 4;
    success = verify_caution_edge_manual(a_x1, a_y1, a_x2, a_y2, b_x1, b_y1, b_x2, b_y2, square_width,
        true);
    EXPECT_TRUE(success);

    a_x1 = 0;
    a_y1 = 1;
    a_x2 = 16;
    a_y2 = 17;
    b_x1 = 5;
    b_y1 = 11;
    b_x2 = 0;
    b_y2 = 16;
    square_width = 4;
    success = verify_caution_edge_manual(a_x1, a_y1, a_x2, a_y2, b_x1, b_y1, b_x2, b_y2, square_width,
        std::vector<bool>{true, true, true, false});
    EXPECT_TRUE(success);

    a_x1 = 0;
    a_y1 = 1;
    a_x2 = 16;
    a_y2 = 17;
    b_x1 = 0;
    b_y1 = 16;
    b_x2 = 5;
    b_y2 = 11;
    square_width = 4;
    success = verify_caution_edge_manual(a_x1, a_y1, a_x2, a_y2, b_x1, b_y1, b_x2, b_y2, square_width,
        std::vector<bool>{true, true, true, false});
    EXPECT_TRUE(success);

    a_x1 = 10;
    a_y1 = 8;
    a_x2 = 13;
    a_y2 = 19;
    b_x1 = 13;
    b_y1 = 19;
    b_x2 = 5;
    b_y2 = 7;
    square_width = 2;
    success = verify_caution_edge_manual(a_x1, a_y1, a_x2, a_y2, b_x1, b_y1, b_x2, b_y2, square_width,
        std::vector<bool>{true, true, true, false, true, false, false});
    EXPECT_TRUE(success);

    a_x1 = 10;
    a_y1 = 8;
    a_x2 = 0;
    a_y2 = 16;
    b_x1 = 0;
    b_y1 = 16;
    b_x2 = 5;
    b_y2 = 7;
    square_width = 2;
    success = verify_caution_edge_manual(a_x1, a_y1, a_x2, a_y2, b_x1, b_y1, b_x2, b_y2, square_width,
        std::vector<bool>{true, true, true, true, true, false, true, false});
    EXPECT_TRUE(success);

    a_x1 = 0;
    a_y1 = 0;
    a_x2 = 15;
    a_y2 = 15;
    b_x1 = 6;
    b_y1 = 0;
    b_x2 = 6;
    b_y2 = 10;
    square_width = 4;
    success = verify_caution_edge_manual(a_x1, a_y1, a_x2, a_y2, b_x1, b_y1, b_x2, b_y2, square_width,
        std::vector<bool>{true, false, true});
    EXPECT_TRUE(success);

    a_x1 = 0;
    a_y1 = 0;
    a_x2 = 15;
    a_y2 = 15;
    b_x1 = 4;
    b_y1 = 0;
    b_x2 = 4;
    b_y2 = 10;
    square_width = 4;
    success = verify_caution_edge_manual(a_x1, a_y1, a_x2, a_y2, b_x1, b_y1, b_x2, b_y2, square_width,
        std::vector<bool>{true, false, true});
    EXPECT_TRUE(success);

    a_x1 = 0;
    a_y1 = 0;
    a_x2 = 15;
    a_y2 = 15;
    b_x1 = 4;
    b_y1 = 10;
    b_x2 = 4;
    b_y2 = 0;
    square_width = 4;
    success = verify_caution_edge_manual(a_x1, a_y1, a_x2, a_y2, b_x1, b_y1, b_x2, b_y2, square_width,
        std::vector<bool>{true, false, true});
    EXPECT_TRUE(success);

    a_x1 = 0;
    a_y1 = 0;
    a_x2 = 15;
    a_y2 = 15;
    b_x1 = 0;
    b_y1 = 10;
    b_x2 = 16;
    b_y2 = 10;
    square_width = 4;
    success = verify_caution_edge_manual(a_x1, a_y1, a_x2, a_y2, b_x1, b_y1, b_x2, b_y2, square_width,
        std::vector<bool>{true, true, false, true});
    EXPECT_TRUE(success);

    a_x1 = 0;
    a_y1 = 0;
    a_x2 = 15;
    a_y2 = 15;
    b_x1 = 16;
    b_y1 = 10;
    b_x2 = 0;
    b_y2 = 10;
    square_width = 4;
    success = verify_caution_edge_manual(a_x1, a_y1, a_x2, a_y2, b_x1, b_y1, b_x2, b_y2, square_width,
        std::vector<bool>{true, true, false, true});
    EXPECT_TRUE(success);

    a_x1 = 0;
    a_y1 = 0;
    a_x2 = 15;
    a_y2 = 15;
    b_x1 = 0;
    b_y1 = 8;
    b_x2 = 16;
    b_y2 = 8;
    square_width = 4;
    success = verify_caution_edge_manual(a_x1, a_y1, a_x2, a_y2, b_x1, b_y1, b_x2, b_y2, square_width,
        std::vector<bool>{true, true, false, true});
    EXPECT_TRUE(success);

    a_x1 = 0;
    a_y1 = 0;
    a_x2 = 15;
    a_y2 = 15;
    b_x1 = 16;
    b_y1 = 8;
    b_x2 = 0;
    b_y2 = 8;
    square_width = 4;
    success = verify_caution_edge_manual(a_x1, a_y1, a_x2, a_y2, b_x1, b_y1, b_x2, b_y2, square_width,
        std::vector<bool>{true, true, false, true});
    EXPECT_TRUE(success);

}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
