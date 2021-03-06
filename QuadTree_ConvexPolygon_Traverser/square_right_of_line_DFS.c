
#include "square_right_of_line_DFS.h"

/*

SquareRightOfLineDFS_LevelTraverser SquareRightOfLineDFS_LevelTraverser_init_shallow_pos_dx(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t square_width)
{
    // p2-p1
    int64_t dx = (int64_t)x2 - (int64_t)x1;
    int64_t dy = (int64_t)y2 - (int64_t)y1;

    SquareRightOfLineDFS_LevelTraverser level_traverser;
    level_traverser.x = x1;
    level_traverser.y = y1 - 1;
    level_traverser.dy_y = (dy >= 0) ? 1 : -1;
    level_traverser.error = 2 * dy - dx;
    level_traverser.dx_error = 2 * dy;
    level_traverser.dy_error = -2 * dx;

    // This traverser works with a lookahead value, so lookahead first.
    level_traverser.x++;
    level_traverser.prev_y = level_traverser.y;
    if (level_traverser.error > 0)
    {
        level_traverser.y++;
        level_traverser.error += level_traverser.dy_error;
    }
    level_traverser.error += level_traverser.dx_error;
    return level_traverser;
}

int64_t dbg_calc_clockwiseness(int64_t x1, int64_t y1, int64_t x2, int64_t y2, int64_t x, int64_t y,
    int64_t normal_x_sign, int64_t normal_y_sign, int64_t square_width)
{
    int64_t dx = x2 - x1;
    int64_t dy = y2 - y1;
    int64_t box_near_point_x = (normal_x_sign >= 0) ? (int64_t)(x) : (int64_t)(x + square_width - 1);
    int64_t box_near_point_y = (normal_y_sign >= 0) ? (int64_t)(y) : (int64_t)(y + square_width - 1);
    box_near_point_x -= x1;
    box_near_point_y -= y1;
    return dx * box_near_point_y - dy * box_near_point_x;
}

bool SquareRightOfLineDFS_LevelTraverser_get(SquareRightOfLineDFS_LevelTraverser* p_traverser, int32_t *out_x, int32_t *out_y)
{
    if (!p_traverser->working_on_extra_point)
    {
        if (p_traverser->x & 1)
        {
            // Of the current square, we are on the right side
            bool hit = (p_traverser->prev_y & 1) != 1;
        }
        else
        {
            // Of the current square, we are on the left side
            if ((p_traverser->y & ~1L) == (p_traverser->prev_y & ~1L))
            {
                // This and next pixel shared the same y block
                bool hit = (p_traverser->prev_y & 1) != 1;
            }
            else
            {
                // The next y block is 1 lower than this y block
                bool hit = true;
                p_traverser->working_on_extra_point = true;
            }
        }
    }
    else
    {
        p_traverser->working_on_extra_point = false;
    }
}

bool SquareRightOfLineDFS_LevelTraverser_IsDone(const SquareRightOfLineDFS_LevelTraverser* p_traverser)
{
    if (p_traverser->x == p_traverser->end_x || p_traverser->y == p_traverser->end_y)
        return true;
    else
        return false;
}

void SquareRightOfLineDFS_LevelTraverser_update_next(SquareRightOfLineDFS_LevelTraverser* p_traverser)
{
    if (p_traverser->inner_y == 1 && p_traverser->inner_x == 1)
    {
        // Done with this 2x2 set, go onto the next.
        if (((p_traverser->near_clockwise + p_traverser->go_x_test) ^ p_traverser->go_x_test_sign_swap) > 0)
        {
            p_traverser->near_clockwise += p_traverser->dx_near_clockwise * 2;
            p_traverser->x += p_traverser->dx_x * 2;
            p_traverser->inner_x = 0;
            p_traverser->inner_y = 0;
        }
        else
        {
            p_traverser->near_clockwise += p_traverser->dy_near_clockwise * 2;
            p_traverser->y += p_traverser->dy_y * 2;
            p_traverser->inner_x = 0;
            p_traverser->inner_y = 0;
        }
    }
    else
    {
        p_traverser->inner_x++;
        if (p_traverser->inner_x > 1)
            p_traverser->inner_y++;
        p_traverser->inner_x &= 1;
    }
}

*/