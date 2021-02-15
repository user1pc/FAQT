
/// square_right_of_line_traverser.c
/// Provides functions which will traverse a quadtree, searching only the squares that are to the right of a given line.

#include "square_right_of_line_traverser.h"

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
