
/// polygon_caution_edge.h
/// When traversing a grid over a line A, it may be helpful
/// to know if a grid on that line intersects another line B.
/// This provides a system for checking that.

#ifndef POLYGON_CAUTION_EDGE_H
#define POLYGON_CAUTION_EDGE_H

#include <stdint.h>
#include <stdbool.h>

typedef struct 
{
    int64_t near_value;
    int64_t far_value;
    int64_t dx_value;
    int64_t dy_value;
    int32_t min_x, max_x, min_y, max_y;
} PolygonEdgeTraverserCautionEdge;

/// Initializes a PolygonEdgeTraverserCautionEdge to avoid multiple edge traversal between lines.
/// @param x1 is the 1st x coordinate of line B.
/// @param y1 is the 1st y coordinate of line B.
/// @param x2 is the 2nd x coordinate of line B.
/// @param y2 is the 2nd y coordinate of line B.
/// @param square_x is the initial grid coordinate of line A being traversed.
/// @param square_y is the initial grid coordinate of line A being traversed.
/// @param square_width is the width of a grid square.
/// @param x_move_sign is the x sign of the line A delta-x movement.
/// @param y_move_sign is the x sign of the line A delta-y movement.
/// @returns a PolygonEdgeTraverserCautionEdge which can be used to detect intersections between lines.
/// @remarks When traversing line A, it may be helpful to know if the grid coordinates intersects
/// line B. This system can detect such an intersection.
PolygonEdgeTraverserCautionEdge PolygonEdgeTraverserCautionEdge_init(
    int32_t x1, int32_t y1, int32_t x2, int32_t y2,
    int32_t square_x, int32_t square_y, int32_t square_width,
    int32_t x_move_sign, int32_t y_move_sign);

/// Tests if the current square intersects the caution edge.
/// @param p_caution is a pointer to a PolygonEdgeTraverserCautionEdge to test.
/// @param x is the x grid coordinate of the edge "a" traverser.
/// @param y is the y grid coordinate of the edge "a" traverser.
/// @returns true if the current square is right of and not in bounds of the line.
/// False otherwise. In other words, returns true if the current edge should processed
/// the current square, false indicates that the caution edge already processed it.
bool PolygonEdgeTraverserCautionEdge_get(const PolygonEdgeTraverserCautionEdge *p_caution, int32_t x, int32_t y);

/// Updates the current grid square in a PolygonEdgeTraverserCautionEdge by the x direction.
/// @param p_caution is a pointer to the PolygonEdgeTraverserCautionEdge to move on the x-axis.
/// @remarks The direction of the x movement is determined by the x_move_sign paramater passed to
/// PolygonEdgeTraverserCautionEdge_init().
void PolygonEdgeTraverserCautionEdge_move_x(PolygonEdgeTraverserCautionEdge *p_caution);

/// Updates the current grid square in a PolygonEdgeTraverserCautionEdge by the y direction.
/// @param p_caution is a pointer to the PolygonEdgeTraverserCautionEdge to move on the y-axis.
/// @remarks The direction of the y movement is determined by the y_move_sign paramater passed to
/// PolygonEdgeTraverserCautionEdge_init().
void PolygonEdgeTraverserCautionEdge_move_y(PolygonEdgeTraverserCautionEdge *p_caution);

#endif // POLYGON_CAUTION_EDGE_H
