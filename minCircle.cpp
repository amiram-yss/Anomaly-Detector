/*
 * miniCircle.cpp
 *
 * Author: 314970054 Ariel Barmats &
 *         314985474 Amiram Yassif
 */

#include "minCircle.h"
#include <vector>
#include <cmath>

using namespace std;

/**
 * Gets 2 points and calculates the distance between them.
 * @param p1    point
 * @param p2    point
 * @return      distance [square root of |x1-x2|^2 to |y1-y2|^2]
 */
float distance(const Point &p1, const Point &p2) {
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

/**
 * Is the point inside the given circle?
 * @param c     circle
 * @param p     point
 * @return      true if circle surround the point
 *              and false if the point is outside of it.
 */
bool is_pt_in_circ(const Circle &c, const Point &p) {
    //point is inside the circle if the distance
    // between the center and the point is smaller than the radius.
    return distance(c.center, p) <= c.radius;
}

/**
 * Gets x,y values of 2 of the points on the circle
 * (as p1 is normalized to (0,0)), and returns the center of the blocking
 * circle.
 * Decided to work with arguments (and not pointers), to reduce runtime.
 * @param p2x   p2.x
 * @param p2y   p2.y
 * @param p3x   p3.x
 * @param p3y   p3.y
 * @return      Center of circle.
 */
Point get_circle_center
        (const float &p2x, const float &p2y, const float &p3x, const float &p3y) {
    float p2dist = p2x * p2x + p2y * p2y;
    float p3dist = p3x * p3x + p3y * p3y;
    float delta = p2x * p3y - p2y * p3x;
    return {(p3y * p2dist - p2y * p3dist) / (2 * delta),
            (p2x * p3dist - p3x * p2dist) / (2 * delta)};
}

/**
 * Creates a circle blocking 3 points.
 * @param p1    point
 * @param p2    point
 * @param p3    point
 * @return      Circle that has 3 points on it.
 */
Circle circle_from
        (const Point &p1, const Point &p2, const Point &p3) {
    // Normalize circle so p1 is set to (0,0), so the center algo can work.
    Point ctr = get_circle_center
            (p2.x - p1.x,
             p2.y - p1.y,
             p3.x - p1.x,
             p3.y - p1.y);
    // Un-normalize the circle, return it to where it was.
    ctr.x += p1.x;
    ctr.y += p1.y;
    // return the circle.
    return {ctr, distance(ctr, p1)};
}

/**
 * Creates a circle that p1 and p2 are one of his diameters,
 * and the center sits in the middle of the 2.
 * @param p1    point one
 * @param p2    point two
 * @return      Smallest circle including the 2 points.
 */
Circle circle_from(const Point &p1, const Point &p2) {
    // Diameter = distance to p1 to p2, and center exactly between both.
    float x = (p1.x + p2.x) / 2;
    float y = (p1.y + p2.y) / 2;
    float r = distance(p1, p2) / 2;

    return {Point(x, y), r};
}

/**
 * Check if a circle contains all of the points in the set
 * @param c     circle
 * @param P     dataset of 2d points
 * @return      True if all points are inside the circle, false otherwise.
 */
bool is_valid_circle(const Circle &c,
                     const vector<Point> &P) {
    // Checks if there is a point that isn't inside the circle.
    for (const Point &point: P)
        if (!is_pt_in_circ(c, point))
            return false;
    // If not found one, ret true.
    return true;
}

/**
 * Creates a minimum-sized circle fitting all relevant points in |dataset|<=3
 * @param relevant_pts  max of 3 points
 * @return              smallest circle holding each one of the points.
 */
Circle min_circle_simple(vector<Point> &relevant_pts) {
    short pts_num = (short) relevant_pts.size();
    // If no points intended, return default circle.
    if (pts_num == 0) {
        Point p = {0, 0};
        return {p, 0};
    }
        // If one, return 0 sized circle around the point.
    else if (pts_num == 1) {
        return {relevant_pts[0], 0};
    }
        // If 2, the smallest circle will be the one with 2 points on the diameter.
    else if (pts_num == 2) {
        return circle_from(relevant_pts[0], relevant_pts[1]);
    }
    // If there are 3 points, the smallest circle could have 2 or 3 points
    // on the circle. Check each option for circle with 2 points on it.
    for (int j = 0; j < MAX_RELEVANT_POINTS_NUMBER; j++) {
        for (int i = j + 1; i < MAX_RELEVANT_POINTS_NUMBER; i++) {
            Circle c = circle_from(relevant_pts[i], relevant_pts[j]);
            if (is_valid_circle(c, relevant_pts))
                return c;
        }
    }
    // If not found good option, choose the worst (=biggest) one, 3 point on
    // the circle.
    return circle_from(relevant_pts[0], relevant_pts[1], relevant_pts[2]);
}

/**
 * Recursive method to return Minimum Enclosing Circle (MEC),
 * according to Welzl's algorithm.
 * @param pts_set       set of all points.
 * @param rlvnt_set     the relevant points in the circle (we need to compare
 *                      only to them).
 * @param n             number of points in point set that were not processed.
 * @return              MEC for the dataset pts_set.
 */
Circle min_circ_recursive_helper(vector<Point> &pts_set, vector<Point> rlvnt_set, int n) {
    // If all points are done, or there is no room for another relevant point,
    // so there is no way to keep going down the recursion, return min circle.
    if (n == 0 || rlvnt_set.size() == MAX_RELEVANT_POINTS_NUMBER) {
        return min_circle_simple(rlvnt_set);
    }
    // pick next point for process.
    Point p = pts_set[n - 1];
    // Go down the recursion, and see if the point is included
    // in the minimum circle without the current point.
    Circle c = min_circ_recursive_helper(pts_set, rlvnt_set, n - 1);
    // If the point is inside the circle, it has no effect on it,
    // return the circle without considering it.
    if (is_pt_in_circ(c, p)) {
        return c;
    }
    // Otherwise, we should consider the current point. It must be on the
    // edge of the circle. We will add it in to the "relevant" set, and check
    // what is the min circle with this point considered.
    rlvnt_set.push_back(p);
    return min_circ_recursive_helper(pts_set, rlvnt_set, n - 1);
}

/**
 * Returns the Minimum Enclosing Circle for given dataset, using Welzl's algo.
 * @param points    dataset of points.
 * @param size      size of dataset.
 * @return          The MEC for the dataset.
 */
Circle findMinCircle(Point **points, size_t size) {
    //Copy array to vector.
    vector<Point> v = vector<Point>();
    for (int i = 0; i < size; ++i) {
        Point p = *points[i];
        v.push_back(p);
    }
    // Copy the vector, avoid destroying it.
    auto pts_set_cpy = v;
    //activate recursive helper method.
    return min_circ_recursive_helper(pts_set_cpy, {}, pts_set_cpy.size());
}
