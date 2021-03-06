
#include "line_square_intersection_traverser.h"

LineSquareIntersectionTraverser LineSquareIntersectionTraverser_init(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t square_x, int32_t square_y, int32_t square_width, int32_t x_move_sign, int32_t y_move_sign)
{
    LineSquareIntersectionTraverser result;

    int64_t dx = (int64_t)x2 - (int64_t)x1;
    int64_t dy = (int64_t)y2 - (int64_t)y1;

    int64_t normal_x_sign = dy;
    int64_t normal_y_sign = -dx;
    int64_t box_near_point_x = ((normal_x_sign >= 0) ? ((int64_t)square_x) : ((int64_t)square_x + (int64_t)square_width - 0)) - x1;
    int64_t box_near_point_y = ((normal_y_sign >= 0) ? ((int64_t)square_y) : ((int64_t)square_y + (int64_t)square_width - 0)) - y1;
    int64_t box_far_point_x = ((normal_x_sign >= 0) ? ((int64_t)square_x + (int64_t)square_width - 0) : ((int64_t)square_x)) - x1;
    int64_t box_far_point_y = ((normal_y_sign >= 0) ? ((int64_t)square_y + (int64_t)square_width - 0) : ((int64_t)square_y)) - y1;

    if (dy == 0)
    {
        if (dx >= 0)
            box_far_point_y++;
        else
            box_near_point_y--;
    }
    if (dx == 0)
    {
        if (dy >= 0)
            box_near_point_x++;
        else
            box_far_point_y--;
    }

    result.near_value = dx * box_near_point_y - dy * box_near_point_x;
    result.far_value = dx * box_far_point_y - dy * box_far_point_x;
    result.dx = (x_move_sign >= 0) ? (-dy * square_width) : (dy * square_width);
    result.dy = (y_move_sign >= 0) ? (dx * square_width) : (-dx * square_width);
    return result;
}

bool LineSquareIntersectionTraverser_move_get(const LineSquareIntersectionTraverser* p_traverser)
{
    return p_traverser->near_value > 0 && p_traverser->far_value < 0;
}

void LineSquareIntersectionTraverser_move_x(LineSquareIntersectionTraverser* p_traverser)
{
    p_traverser->near_value += p_traverser->dx;
    p_traverser->far_value += p_traverser->dx;
}

void LineSquareIntersectionTraverser_move_y(LineSquareIntersectionTraverser* p_traverser)
{
    p_traverser->near_value += p_traverser->dy;
    p_traverser->far_value += p_traverser->dy;
}
