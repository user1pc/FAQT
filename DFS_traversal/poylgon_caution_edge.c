
/// polygon_caution_edge.c
/// When traversing a grid over a line A, it may be helpful
/// to know if a grid on that line intersects another line B.
/// This provides a system for checking that.

#include "polygon_caution_edge.h"
#include "../line_traverser/line_traverser.h"

#ifndef min
#define min(a, b) (((a) < (b)) ? (a) : (b))
#endif
#ifndef max
#define max(a, b) (((a) > (b)) ? (a) : (b))
#endif

PolygonEdgeTraverserCautionEdge PolygonEdgeTraverserCautionEdge_init(
    int32_t x1, int32_t y1, int32_t x2, int32_t y2,
    int32_t square_x, int32_t square_y, int32_t square_width,
    int32_t x_move_sign, int32_t y_move_sign)
{
    PolygonEdgeTraverserCautionEdge caution_edge;
    square_x *= square_width;
    square_y *= square_width;
    int64_t dx = (int64_t)x2 - (int64_t)x1;
    int64_t dy = (int64_t)y2 - (int64_t)y1;

    int64_t normal_x_sign = dy;
    int64_t normal_y_sign = -dx;
    int64_t box_near_point_x = ((normal_x_sign >= 0) ? ((int64_t)square_x) : ((int64_t)square_x + (int64_t)square_width)) - x1;
    int64_t box_near_point_y = ((normal_y_sign >= 0) ? ((int64_t)square_y) : ((int64_t)square_y + (int64_t)square_width)) - y1;
    int64_t box_far_point_x = ((normal_x_sign >= 0) ? ((int64_t)square_x + (int64_t)square_width) : ((int64_t)square_x)) - x1;
    int64_t box_far_point_y = ((normal_y_sign >= 0) ? ((int64_t)square_y + (int64_t)square_width) : ((int64_t)square_y)) - y1;

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

    caution_edge.near_value = dx * box_near_point_y - dy * box_near_point_x;
    caution_edge.far_value = dx * box_far_point_y - dy * box_far_point_x;
    caution_edge.dx_value = (x_move_sign >= 0) ? (-dy * square_width) : (dy * square_width);
    caution_edge.dy_value = (y_move_sign >= 0) ? (dx * square_width) : (-dx * square_width);
    int32_t ep1x, ep1y, ep2x, ep2y;
    LineTraverser_get_endpoints(x1, y1, x2, y2, square_width, &ep1x, &ep1y, &ep2x, &ep2y);
    caution_edge.min_x = min(ep1x, ep2x);
    caution_edge.max_x = max(ep1x, ep2x);
    caution_edge.min_y = min(ep1y, ep2y);
    caution_edge.max_y = max(ep1y, ep2y);
    return caution_edge;
}

bool PolygonEdgeTraverserCautionEdge_get(const PolygonEdgeTraverserCautionEdge *p_caution, int32_t x, int32_t y)
{
    if ((x < p_caution->min_x || x > p_caution->max_x) || (y < p_caution->min_y || y > p_caution->max_y))
        return true;
    return p_caution->near_value <= 0 || p_caution->far_value >= 0;
}

void PolygonEdgeTraverserCautionEdge_move_x(PolygonEdgeTraverserCautionEdge *p_caution)
{
    p_caution->near_value += p_caution->dx_value;
    p_caution->far_value += p_caution->dx_value;
}

void PolygonEdgeTraverserCautionEdge_move_y(PolygonEdgeTraverserCautionEdge *p_caution)
{
    p_caution->near_value += p_caution->dy_value;
    p_caution->far_value += p_caution->dy_value;
}
