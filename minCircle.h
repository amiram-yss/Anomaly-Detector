/*
 * miniCircle.h
 *
 * Author: 314970054 Ariel Barmats &
 *         314985474 Amiram Yassif
 */

#ifndef MINCIRCLE_H_
#define MINCIRCLE_H_

#define     MAX_RELEVANT_POINTS_NUMBER  3

#include <iostream>
//#include <vector>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include "anomaly_detection_util.h"

using namespace std;


// ------------ DO NOT CHANGE -----------

class Circle{
public:
    Point center;
    float radius;
    Circle(Point c,float r):center(c),radius(r){}
};
// --------------------------------------

Circle findMinCircle(Point** points,size_t size);

// you can add here additional methods

#endif /* MINCIRCLE_H_ */
