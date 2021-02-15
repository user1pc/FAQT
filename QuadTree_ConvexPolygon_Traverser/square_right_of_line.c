
/// square_right_of_line.c
/// Defines a system for quickly testing if a square (must be a power of 2) in a quadtree traversal is right of (or on) a line.

#include "square_right_of_line.h"

/// Sets up a system for testing if a square (as part of a quadtree traversal) will be on the
/// right side of an infinite line (and also distinguishes if the square intersects the line).
/// The line is treated as infinite. Right of the line is considered from the point p1 to the direction
/// defined by p2-p1.
/// @param x1 x coordinate of the 1st point of the line.
/// @param y1 y coordinate of the 1st point of the line.
/// @param x2 x coordinate of the 2nd point of the line.
/// @param y2 y coordinate of the 2nd point of the line.
/// @param square_x starting x coordinate of the square to traverse.
/// @param square_y starting y coordinate of the square to traverse.
/// @param square_width starting width of the square to traverse.
/// @returns a SquareRightOfLineTester for traversing this geometry.
SquareRightOfLineTester SquareRightOfLineTester_init(int32_t x1, int32_t y1, int32_t x2, int32_t y2,
    int32_t square_x, int32_t square_y, int32_t square_width)
{
    // p2-p1
    int64_t dx = (int64_t)x2 - (int64_t)x1;
    int64_t dy = (int64_t)y2 - (int64_t)y1;

    // To perform this test, we need to test two points on the square.
    // The first point, used to test for partial intersection, is the point on the square
    // that is right of the line, and closest to the line.
    // The other point used to test for complete intersection, is the point on the square
    // that is right of the line, and farthest from the line.
    // For these calculations, we consider the normal of the line to be a vector
    // pointing 90 degrees clockwise (to the right) of the line defined by p2-p1.
    // To decide which two points on the square to use from the 4 possible points, we will
    // consider the signs of this normal vector.
    int64_t normal_x_sign = dy;
    int64_t normal_y_sign = -dx;
    int64_t box_near_point_x = ((normal_x_sign >= 0) ? ((int64_t)square_x) : ((int64_t)square_x + (int64_t)square_width)) - x1;
    int64_t box_near_point_y = ((normal_y_sign >= 0) ? ((int64_t)square_y) : ((int64_t)square_y + (int64_t)square_width)) - y1;
    int64_t box_far_point_x = ((normal_x_sign >= 0) ? ((int64_t)square_x + (int64_t)square_width) : ((int64_t)square_x)) - x1;
    int64_t box_far_point_y = ((normal_y_sign >= 0) ? ((int64_t)square_y + (int64_t)square_width) : ((int64_t)square_y)) - y1;

    // Now that we have the line and the points, simply test if the point is clockwise of the line.
    // How? Each, the sign of the 2D cross product.
    // If dx*box_y - dy*box_x >= 0, the point is clockwise/right (or on) the line.
    // Otherwise is is anticlockwise/left of the line.
    SquareRightOfLineTester tester;
    tester.near_value = dx * box_near_point_y - dy * box_near_point_x;
    tester.far_value = dx * box_far_point_y - dy * box_far_point_x;

    // Now do the same thing but for the delta values.
    int64_t half_width = (int64_t)square_width / 2;
    tester.normal_x_sign = normal_x_sign;
    tester.dx = -dy * square_width;
    tester.normal_y_sign = normal_y_sign;
    tester.dy = dx * square_width;
    return tester;
}

/// Gets the intersection result (partial hit + complete hit) of the given SquareRightOfLineTester and
/// one of the 4 possible next nodes.
/// @param p_tester pointer to the tester to test on.
/// @param dx if zero, will test the left side of this current square node. Otherwise if nonzero tests the right side.
/// @param dy if zero, will test the top side of this current square node. Otherwise if nonzero tests the bottom side.
/// @param node_depth Depth in the tree from initialization. 1 to get from the 1st (root )node to the 2nd level.
/// @returns a SquareRightOfLineTesterResult struct defining if there is a partial/complete hit + some other internal use
/// data.
SquareRightOfLineTesterResult SquareRightOfLineTester_TestNextStep(SquareRightOfLineTester* p_tester, int dx, int dy, int node_depth)
{
    int64_t local_dx = p_tester->dx >> (int64_t)node_depth;
    int64_t local_dy = p_tester->dy >> (int64_t)node_depth;
    SquareRightOfLineTesterResult result;
    result.near_value = p_tester->near_value;
    result.far_value = p_tester->far_value;
    if (dx > 0)
    {
        if (p_tester->normal_x_sign >= 0)
            result.near_value += local_dx;
        else
            result.far_value += local_dx;
    }
    else
    {
        if (p_tester->normal_x_sign >= 0)
            result.far_value -= local_dx;
        else
            result.near_value -= local_dx;
    }

    if (dy > 0)
    {
        if (p_tester->normal_y_sign >= 0)
            result.near_value += local_dy;
        else
            result.far_value += local_dy;
    }
    else
    {
        if (p_tester->normal_y_sign >= 0)
            result.far_value -= local_dy;
        else
            result.near_value -= local_dy;
    }

    // if result.near_value == 0, the line intersects a side of the box.
    // I consider this to be touching the square, and therefore the square isn't completely to the right
    // of the line.
    result.complete_hit = result.near_value < 0;
    result.partial_hit = result.far_value <= 0;
    return result;
}

/// Steps down into the next node during traversal of a SquareRightOfLineTester.
/// @param p_tester The current testing struct.
/// @p_result The SquareRightOfLineTesterResult which defines which node to take. Obtained by calling SquareRightOfLineTester_TestNextStep().
void SquareRightOfLineTester_StepInFromResult(SquareRightOfLineTester* p_tester, const SquareRightOfLineTesterResult* p_result)
{
    p_tester->near_value = p_result->near_value;
    p_tester->far_value = p_result->far_value;
}

/// Steps up in the tree to a parent node.
/// @param p_tester the tester to step up to its parent node.
/// @param dx nonzero if the node in p_tester is a right-child of its parent. Zero if it is a left-child of its parent.
/// @param dy nonzero if the node in p_tester is a bottom-child of its parent. Zero if it is a top-child of its parent.
/// @param parent_depth the depth of the parent node. Measured starting with 1 on the root node.
void SquareRightOfLineTester_StepOutFromResult(SquareRightOfLineTester* p_tester, int dx, int dy, int parent_depth)
{
    int64_t local_dx = p_tester->dx >> (int64_t)parent_depth;
    int64_t local_dy = p_tester->dy >> (int64_t)parent_depth;
    if (dx > 0)
    {
        if (p_tester->normal_x_sign >= 0)
            p_tester->near_value -= local_dx;
        else
            p_tester->far_value -= local_dx;
    }
    else
    {
        if (p_tester->normal_x_sign >= 0)
            p_tester->far_value += local_dx;
        else
            p_tester->near_value += local_dx;
    }

    if (dy > 0)
    {
        if (p_tester->normal_y_sign >= 0)
            p_tester->near_value -= local_dy;
        else
            p_tester->far_value -= local_dy;
    }
    else
    {
        if (p_tester->normal_y_sign >= 0)
            p_tester->far_value += local_dy;
        else
            p_tester->near_value += local_dy;
    }
}
