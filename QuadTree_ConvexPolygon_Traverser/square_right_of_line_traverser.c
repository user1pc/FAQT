
/// square_right_of_line_traverser.c
/// Provides functions which will traverse a quadtree, searching only the squares that are to the right of a given line.

#include "square_right_of_line_traverser.h"
#include "square_right_of_line_DFS.h"
#include <math.h>

void SquareRightOfLineTraversePreOrder(int32_t x1, int32_t y1, int32_t x2, int32_t y2,
    int32_t starting_x, int32_t starting_y, int32_t starting_width_log2,
    SquareRightOfLineTraverserPreOrderCallback callback, void* user_data)
{
    int32_t width = 1 << starting_width_log2;
    int32_t half_width = 1 << (starting_width_log2 - 1);
    int32_t depth = 1;
    SquareRightOfLineTester tester = SquareRightOfLineTester_init(x1, y1, x2, y2, starting_x, starting_y, width);
    int32_t current_x = starting_x;
    int32_t current_y = starting_y;
    int32_t current_mask = width - 1;
    int32_t dx = 0;
    int32_t dy = 0;
    int32_t half_width_log2 = starting_width_log2 - 1;

    bool last_iter_was_overflow = false;
    while (depth != 0)
    {
        if (last_iter_was_overflow)
        {
            // Traversed all 4 child nodes, go back up the tree.
            half_width <<= 1;
            half_width_log2++;
            depth--;
            current_x &= ~current_mask;
            current_y &= ~current_mask;
            current_mask = (current_mask << 1) | 1;
            dx = current_x & current_mask;
            dy = current_y & current_mask;
            SquareRightOfLineTester_StepOutFromResult(&tester, dx, dy, depth);
            goto inc_to_next_node_region;
        }

        SquareRightOfLineTesterResult result = SquareRightOfLineTester_TestNextStep(&tester, dx, dy, depth);
        bool go_deeper = false;
        if (result.partial_hit)
            go_deeper = callback(user_data, current_x, current_y, half_width, depth, result.complete_hit);
        go_deeper &= half_width > 1; // if half_width == 1, we are at a leaf node. Can't go deeper.

        if (go_deeper)
        {
            // Go into this (dx,dy) child node.
            half_width >>= 1;
            half_width_log2--;
            depth++;
            current_mask >>= 1;
            dx = 0;
            dy = 0;
            SquareRightOfLineTester_StepInFromResult(&tester, &result);
        }
        else
        {
            inc_to_next_node_region:
            int32_t next_dx = dx + half_width;
            bool next_dx_overflow = next_dx > current_mask;
            int32_t next_dy = dy + ((next_dx_overflow) ? half_width : 0);
            bool next_dy_overflow = next_dy > current_mask;
            last_iter_was_overflow = next_dy_overflow;
            // Go to the next child of this node.
            dx = next_dx & current_mask;
            dy = next_dy & current_mask;
            current_x = (current_x & ~current_mask) | dx;
            current_y = (current_y & ~current_mask) | dy;
        }
    }
}

/*

void SquareRightOfLineTraverseDepthFirst(int32_t x1, int32_t y1, int32_t x2, int32_t y2,
    int32_t starting_x, int32_t starting_y, int32_t starting_width_log2,
    SquareRightOfLineTraverserDFSCallback callback, void* user_data)
{
    int32_t square_width = 1 << starting_width_log2;
    int32_t num_deep = starting_width_log2;
    int32_t num_nodes_wide = 2;
    int32_t width_of_node = 1 << (starting_width_log2 - 1);

    // We need the line to project "infinitely", but the given points create a line segment.
    // We need a defined starting and ending point.
    // So, just find the intersection of the line and the given square.
    int32_t dx = x2 - x1;
    int32_t dy = y2 - y1;
    int32_t t1x_numerator = ((dx >= 0) ? 0 : (square_width - 1)) - x1;
    int32_t t1x_denominator = dx;
    int32_t t1y_numerator = ((dy >= 0) ? 0 : (square_width) - 1) - y1;
    int32_t t1y_denominator = dy;
    int32_t t2x_numerator = ((dx >= 0) ? (square_width - 1) : 0) - x1;
    int32_t t2x_denominator = dx;
    int32_t t2y_numerator = ((dy >= 0) ? (square_width - 1) : 0) - y1;
    int32_t t2y_denominator = dy;

    // Calculate parametric t values for near & far intersection of the square and the line.
    int32_t near_t_numerator = t1x_numerator;
    int32_t near_t_denominator = t1x_denominator;
    if ((int64_t)t1y_numerator * (int64_t)near_t_denominator > (int64_t)near_t_numerator * (int64_t)t1y_denominator)
    {
        near_t_numerator = t1y_numerator;
        near_t_denominator = t1y_denominator;
    }
    int32_t far_t_numerator = t2x_numerator;
    int32_t far_t_denominator = t2x_denominator;
    if ((int64_t)t2y_numerator * (int64_t)far_t_denominator < (int64_t)far_t_numerator * (int64_t)t2y_denominator)
    {
        far_t_numerator = t2y_numerator;
        far_t_denominator = t2y_denominator;
    }

    // Now, calculate the exact x,y coordinates of both the near & far point.
    // near_x = x1 + (dx * near_t_numerator) / near_t_denominator
    // near_y = y1 + (dy * near_t_numerator) / near_t_denominator
    // far_x = x1 + (dx * far_t_numerator) / far_t_denominator
    // far_y = y1 + (dy * far_t_numerator) / far_t_denominator
    // But we don't want to do division. We don't really need to, becuase we need this result for
    // every depth in the tree. In the 1st iteration, there are only 2x2 nodes. In the 2nd iteration 4x4 nodes, 3rd = 8x8, etc.
    //This means it is possible to basically merge a binary search into the depth loop.
    //   x1 + (dx * near_t_numerator) / near_t_denominator >= half_width
    // = (x1 * near_t_denominator) + (dx * near_t_numerator) >= half_width * near_t_denominator
    int64_t near_x_selector = ((int64_t)x1 * near_t_denominator + (int64_t)dx * near_t_numerator) - ((int64_t)width_of_node * near_t_denominator);
    int64_t near_y_selector = ((int64_t)y1 * near_t_denominator + (int64_t)dy * near_t_numerator) - ((int64_t)width_of_node * near_t_denominator);
    int64_t near_selector_delta = -(int64_t)(width_of_node >> 1) * near_t_denominator;
    int32_t near_x = 0;
    int32_t near_y = 0;
    if (near_x_selector >= 0)
    {
        near_x |= 1;
        near_x_selector += near_selector_delta;
    }
    if (near_y_selector >= 0)
    {
        near_y |= 1;
        near_y_selector += near_selector_delta;
    }
    near_selector_delta >>= 1;

    int64_t far_x_selector = ((int64_t)x1 * far_t_denominator + (int64_t)dx * far_t_numerator) - ((int64_t)width_of_node * far_t_denominator);
    int64_t far_y_selector = ((int64_t)y1 * far_t_denominator + (int64_t)dy * far_t_numerator) - ((int64_t)width_of_node * far_t_denominator);
    int64_t far_selector_delta = -(int64_t)(width_of_node >> 1) * far_t_denominator;
    int32_t far_x = 0;
    int32_t far_y = 0;
    if (far_x_selector >= 0)
    {
        far_x |= 1;
        far_x_selector += far_selector_delta;
    }
    if (far_y_selector >= 0)
    {
        far_y |= 1;
        far_y_selector += far_selector_delta;
    }
    far_selector_delta >>= 1;

    bool is_steep = abs(dx) < abs(dy);
    for (int32_t depth = 0; depth < num_deep; depth++)
    {
        SquareRightOfLineDFS_LevelTraverser traverser = SquareRightOfLineDFS_LevelTraverser_init(
            near_x, near_y, far_x, far_y, width_of_node, num_nodes_wide);
        while (!SquareRightOfLineDFS_LevelTraverser_IsDone(&traverser))
        {
            bool point_is_right_of_line = SquareRightOfLineDFS_LevelTraverser_get(&traverser);
            if (point_is_right_of_line)
                callback(user_data, traverser.x, traverser.y, width_of_node, depth);
            SquareRightOfLineDFS_LevelTraverser_update_next(&traverser, point_is_right_of_line);
        }

        width_of_node >>= 1;
        num_nodes_wide <<= 1;
        // Update near/far_x/y points 1 more iterations worth
        near_x <<= 1;
        if (near_x_selector >= 0)
        {
            near_x |= 1;
            near_x_selector += near_selector_delta;
        }
        near_y <<= 1;
        if (near_y_selector >= 0)
        {
            near_y |= 1;
            near_y_selector += near_selector_delta;
        }
        near_selector_delta >>= 1;

        far_x <<= 1;
        if (far_x_selector >= 0)
        {
            far_x |= 1;
            far_x_selector += far_selector_delta;
        }
        far_y <<= 1;
        if (far_y_selector >= 0)
        {
            far_y |= 1;
            far_y_selector += far_selector_delta;
        }
        far_selector_delta >>= 1;
    }
}
*/