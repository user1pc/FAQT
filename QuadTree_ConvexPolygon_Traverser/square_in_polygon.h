
/*

/// square_in_polygon.h
/// Defines a system for quickly testing if a square (must be a power of 2) in a quadtree traversal is inside a convex polygon.

#ifndef SQUARE_IN_POlYGON_H
#define SQUARE_IN_POlYGON_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

typedef struct
{
    int64_t near_values, far_values;
    int64_t 
    int64_t near_value, far_value;
    int64_t dx, dy;
    int64_t normal_x_sign, normal_y_sign;
} SquareInConvexPolygon;

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
        int32_t square_x, int32_t square_y, int32_t square_width);

    typedef struct
    {
        int64_t near_value, far_value;
        bool partial_hit, complete_hit;
    } SquareRightOfLineTesterResult;

    /// Gets the intersection result (partial hit + complete hit) of the given SquareRightOfLineTester and
    /// one of the 4 possible next nodes.
    /// @param p_tester pointer to the tester to test on.
    /// @param dx if zero, will test the left side of this current square node. Otherwise if nonzero tests the right side.
    /// @param dy if zero, will test the top side of this current square node. Otherwise if nonzero tests the bottom side.
    /// @param node_depth Depth in the tree from initialization. 1 to get from the 1st (root )node to the 2nd level.
    /// @returns a SquareRightOfLineTesterResult struct defining if there is a partial/complete hit + some other internal use
    /// data.
    SquareRightOfLineTesterResult SquareRightOfLineTester_TestNextStep(SquareRightOfLineTester* p_tester, int dx, int dy, int depth);

    /// Steps down into the next node during traversal of a SquareRightOfLineTester.
    /// @param p_tester The current testing struct.
    /// @p_result The SquareRightOfLineTesterResult which defines which node to take. Obtained by calling SquareRightOfLineTester_TestNextStep().
    void SquareRightOfLineTester_StepInFromResult(SquareRightOfLineTester* p_tester, const SquareRightOfLineTesterResult* p_result);

    /// Steps up in the tree to a parent node.
    /// @param p_tester the tester to step up to its parent node.
    /// @param dx nonzero if the node in p_tester is a right-child of its parent. Zero if it is a left-child of its parent.
    /// @param dy nonzero if the node in p_tester is a bottom-child of its parent. Zero if it is a top-child of its parent.
    /// @param parent_depth the depth of the parent node. Measured starting with 1 on the root node.
    void SquareRightOfLineTester_StepOutFromResult(SquareRightOfLineTester* p_tester, int dx, int dy, int parent_depth);

#ifdef __cplusplus
}
#endif

#endif // SQUARE_IN_POlYGON_H

*/