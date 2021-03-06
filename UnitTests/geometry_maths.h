
#ifndef GEOMETRY_MATHS_H
#define GEOMETRY_MATHS_H

#include <math.h>

template <typename T>
bool point_right_of_or_on_line(T x1, T y1, T x2, T y2,
    T point_x, T point_y)
{
    T dx = x2 - x1;
    T dy = y2 - y1;
    T px = point_x - x1;
    T py = point_y - y1;
    T cross = dx * py - dy * px;
    return cross <= 0;
}

template <typename T>
bool point_right_of_line(T x1, T y1, T x2, T y2,
    T point_x, T point_y)
{
    T dx = x2 - x1;
    T dy = y2 - y1;
    T px = point_x - x1;
    T py = point_y - y1;
    T cross = dx * py - dy * px;
    return cross < 0;
}

template <typename T>
bool square_right_of_line(T x1, T y1, T x2, T y2,
    T square_x, T square_y, T square_width, bool* out_complete_hit)
{
    bool top_left_hit = point_right_of_or_on_line(x1, y1, x2, y2, square_x, square_y);
    bool top_right_hit = point_right_of_or_on_line(x1, y1, x2, y2, square_x + square_width - 1, square_y);
    bool bottom_left_hit = point_right_of_or_on_line(x1, y1, x2, y2, square_x, square_y + square_width - 1);
    bool bottom_right_hit = point_right_of_or_on_line(x1, y1, x2, y2, square_x + square_width - 1, square_y + square_width - 1);

    bool top_left_hit_complete = point_right_of_line(x1, y1, x2, y2, square_x, square_y);
    bool top_right_hit_complete = point_right_of_line(x1, y1, x2, y2, square_x + square_width - 1, square_y);
    bool bottom_left_hit_complete = point_right_of_line(x1, y1, x2, y2, square_x, square_y + square_width - 1);
    bool bottom_right_hit_complete = point_right_of_line(x1, y1, x2, y2, square_x + square_width - 1, square_y + square_width - 1);
    *out_complete_hit = top_left_hit_complete && top_right_hit_complete && bottom_left_hit_complete && bottom_right_hit_complete;
    return top_left_hit || top_right_hit || bottom_left_hit || bottom_right_hit;
}

template <typename T>
T area_of_polygon(T* x_points, T* y_points, int num_points)
{
    // https://en.wikipedia.org/wiki/Shoelace_formula
    T area = 0;
    int i = 0;
    for (; i < num_points - 1; i++)
    {
        area += x_points[i] * y_points[i + 1] - y_points[i] * x_points[i + 1];
    }
    area += x_points[i] * y_points[0] - y_points[i] * x_points[0];
    return area / (T)2;
}

enum geometry_intersection_type
{
    intersection, overlap, miss
};

// Returns the type of intersection between 2 line segments.
// Distinguishes if there is a overlap, in which case the intersection point is undefined.
template <typename T>
geometry_intersection_type line_line_intersection(T a_x1, T a_y1, T a_x2, T a_y2, T b_x1, T b_y1, T b_x2, T b_y2, T* out_x, T* out_y)
{
    if (a_x1 == a_x2 && a_x1 == b_x1 && a_x1 == b_x2)
    {
        return overlap;
    }
    if (a_y1 == a_y2 && a_y1 == b_y1 && a_y1 == b_y2)
    {
        return overlap;
    }

    // https://en.wikipedia.org/wiki/Line%E2%80%93line_intersection
    T a_dx = a_x2 - a_x1;
    T a_dy = a_y2 - a_y1;
    T t_numerator   = (a_x1 - b_x1) * (b_y1 - b_y2) - (a_y1 - b_y1) * (b_x1 - b_x2);
    T t_denominator = (a_x1 - a_x2) * (b_y1 - b_y2) - (a_y1 - a_y2) * (b_x1 - b_x2);

    if (t_denominator != 0)
    {
        *out_x = (a_x1 * t_denominator + a_dx * t_numerator) / t_denominator;
        *out_y = (a_y1 * t_denominator + a_dy * t_numerator) / t_denominator;
    }
    // 0 <= t <= 1
    if ((t_numerator * t_denominator) >= 0 && (t_numerator <= t_denominator))
        return intersection;
    else
        return miss;
}

// Returns intersection if the line is completely inside the square,
// overlap if the line intersects one or two edges of the sqaure,
// miss if the line segment doesn't ever touch or lie inside the square.
template <typename T>
geometry_intersection_type linesegment_square_intersection(T x1, T y1, T x2, T y2, T square_x, T square_y, T square_width, T* out_x1, T* out_y1, T *out_x2, T *out_y2)
{
    T dx = x2 - x1;
    T dy = y2 - y1;
    T t1x_numerator = ((dx >= 0) ? (square_x) : (square_x + square_width)) - x1;
    T t1x_denominator = dx;
    T t1y_numerator = ((dy >= 0) ? (square_y) : (square_y + square_width)) - y1;
    T t1y_denominator = dy;

    T t2x_numerator = ((dx >= 0) ? (square_x + square_width) : (square_x)) - x1;
    T t2x_denominator = dx;
    T t2y_numerator = ((dy >= 0) ? (square_y + square_width) : (square_y)) - y1;
    T t2y_denominator = dy;

    T near_numerator = t1x_numerator;
    T near_denominator = t1x_denominator;
    if (t1y_numerator * near_denominator > near_numerator * t1y_denominator)
    {
        near_numerator = t1y_numerator;
        near_denominator = t1y_denominator;
    }

    T far_numerator = t2x_numerator;
    T far_denominator = t2x_denominator;
    if (t2y_numerator * far_denominator < far_numerator * t2y_denominator)
    {
        far_numerator = t2y_numerator;
        far_denominator = t2y_denominator;
    }


    // Only a possible hit if near <= far
    if (far_numerator * near_denominator >= near_numerator * far_denominator)
    {
        // line_begin < square_begin && line_end > square_end
        if (near_numerator * near_denominator > 0 && far_numerator < far_denominator)
        {
            *out_x1 = (x1 * near_denominator + dx * near_numerator) / near_denominator;
            *out_y1 = (y1 * near_denominator + dy * near_numerator) / near_denominator;
            *out_x2 = (x2 * far_denominator + dx * far_numerator) / far_denominator;
            *out_y2 = (y2 * far_denominator + dy * far_numerator) / far_denominator;
            return overlap;
        }
        // line_begin < square_begin && line_end <= square_end
        else if (near_numerator * near_denominator > 0 && far_numerator >= far_denominator)
        {
            *out_x1 = (x1 * near_denominator + dx * near_numerator) / near_denominator;
            *out_y1 = (y1 * near_denominator + dy * near_numerator) / near_denominator;
            *out_x2 = x2;
            *out_y2 = y2;
            return overlap;
        }
        // line_begin >= square_begin && line_end > square_end
        else if (near_numerator * near_denominator <= 0 && far_numerator < far_denominator)
        {
            *out_x1 = x1;
            *out_y1 = y1;
            *out_x2 = (x2 * far_denominator + dx * far_numerator) / far_denominator;
            *out_y2 = (y2 * far_denominator + dy * far_numerator) / far_denominator;
            return overlap;
        }
        // line_begin >= square_begin && line_end <= square_end
        else if (near_numerator * near_denominator <= 0 && far_numerator < far_denominator)
        {
            *out_x1 = x1;
            *out_y1 = y1;
            *out_x2 = x2;
            *out_y2 = y2;
            return intersection;
        }
        else
        {
            *out_x1 = x1;
            *out_y1 = y1;
            *out_x2 = x2;
            *out_y2 = y2;
            return miss;
        }
    }
    else
    {
        *out_x1 = x1;
        *out_y1 = y1;
        *out_x2 = x2;
        *out_y2 = y2;
        return miss;
    }
}

template <typename T>
geometry_intersection_type point_in_convex_polygon(T x, T y, T* poly_xs, T* poly_ys, int poly_num_points)
{
    int i = 0;
    for (; i < poly_num_points - 1; i++)
    {
        T dx = poly_xs[i + 1] - poly_xs[i];
        T dy = poly_ys[i + 1] - poly_ys[i];

        T px = x - poly_xs[i];
        T py = y - poly_ys[i];
        T clockwiseness = dx * py - dy * px;
        if (clockwiseness < 0)
            return miss;
        else if (clockwiseness == 0)
            return overlap;
    }

    T dx = poly_xs[0] - poly_xs[i];
    T dy = poly_ys[0] - poly_ys[i];

    T px = x - poly_xs[i];
    T py = y - poly_ys[i];
    T clockwiseness = dx * py - dy * px;
    if (clockwiseness < 0)
        return miss;
    else if (clockwiseness == 0)
        return overlap;

    return intersection;
}

template <typename T>
geometry_intersection_type square_inside_polygon_test(T square_x, T square_y, T square_width, T* x_points, T* y_points, int num_points)
{
    bool is_completely_inside = true;
    int i = 0;
    for (; i < poly_num_points - 1; i++)
    {
        bool completely_right;
        if (square_right_of_line(poly_xs[i], poly_ys[i], poly_xs[i + 1], poly_ys[i + 1], square_x, square_y, square_width, &completely_right))
        {
            if (!completely_right)
                is_completely_inside = false;
        }
        else
        {
            return miss;
        }
    }

    bool completely_right;
    if (square_right_of_line(poly_xs[i], poly_ys[i], poly_xs[0], poly_ys[0], square_x, square_y, square_width, &completely_right))
    {
        if (!completely_right)
            is_completely_inside = false;
    }
    else
    {
        return miss;
    }

    if (is_completely_inside)
        return intersection;
    else
        return overlap;
}

enum side_of_square
{
    left, top, right, bottom
};

template <typename T>
side_of_square closest_side_of_square_to_point(T x, T y, T square_x, T square_y, T square_width)
{
    // TODO: may be problem with floats
    T min_dist = abs(x - square_x);
    side_of_square ret = left;
    if (abs(y - square_y) < min_dist)
    {
        min_dist = abs(y - square_y);
        ret = bottom;
    }
    if (abs(y - (square_y + square_width)) < min_dist)
    {
        min_dist = abs(y - (square_y + square_width));
        ret = top;
    }
    if (abs(x - (square_x + square_width)) < min_dist)
    {
        min_dist = abs(x - (square_x + square_width));
        ret = right;
    }
    return ret;
}

template <typename T>
int polygon_square_intersection(T* in_x_points, T* in_y_points, int num_in_points, T square_x, T square_y, T square_width, T* out_x_points, T* out_y_points)
{
    // 1st, find the first line which intersects or is inside the square
    int i = 0;
    T intersection_x1, intersection_y1, intersection_x2, intersection_y2;
    geometry_intersection_type first_intersection_type = miss;
    for (; i < num_in_points - 1; i++)
    {
        T x1 = in_x_points[i];
        T y1 = in_y_points[i];
        T x2 = in_x_points[i + 1];
        T y2 = in_y_points[i + 1];
        T dx = x2 - x1;
        T dy = y2 - y1;
        geometry_intersection_type intersection_type =
            linesegment_square_intersection(x1, y1, x2, y2, square_x, square_y, square_width, &intersection_x1, &intersection_y1, &intersection_x2, &intersection_y2);
        if (intersection_type != miss)
        {
            first_intersection_type = intersection_type;
            break;
        }
    }
    if (first_intersection_type == miss)
    {
        // Last attempt with the last line
        T x1 = in_x_points[i];
        T y1 = in_y_points[i];
        T x2 = in_x_points[0];
        T y2 = in_y_points[0];
        T dx = x2 - x1;
        T dy = y2 - y1;
        geometry_intersection_type intersection_type =
            linesegment_square_intersection(x1, y1, x2, y2, square_x, square_y, square_width, &intersection_x1, &intersection_y1, &intersection_x2, &intersection_y2);
        if (intersection_type != miss)
        {
            first_intersection_type = intersection_type;
            break;
        }
    }

    if (first_intersection_type != miss)
    {
        int num_out_points = 0;
        T last_x, last_y;
        side_of_square last_side_of_square = closest_side_of_square_to_point()
        out_x_points[num_out_points] = intersection_x1;
        out_y_points[num_out_points] = intersection_y1;
        out_x_points[num_out_points + 1] = intersection_x2;
        out_y_points[num_out_points + 1] = intersection_y2;
        for (int j = i + 1; j != i; j++)
        {
            if (j >= num_in_points)
                j = 0;


        }
    }
}

#endif // GEOMETRY_MATHS_H
