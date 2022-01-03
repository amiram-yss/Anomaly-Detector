/*
 * animaly_detection_util.cpp
 *
 * Author: 314970054 Ariel Barmats &
 *         314985474 Amiram Yasif
 */

#include <math.h>
#include <stdlib.h>
#include "anomaly_detection_util.h"
#include "minCircle.h"
#include "SimpleAnomalyDetector.h"


/***********************************************************
 *
 * @param x
 * @param size
 * @return the average of the numbers in the array
 *************************************************************/
float avg(float *x, int size) {
    float avg = 0;
    for (int i = 0; i < size; ++i) {
        avg += x[i];
    };
    avg /= size;
    return avg;
}


/********************************************
 *
 * @param x
 * @param size
 * @return the variance of X and Y
 *******************************************/
float var(float *x, int size) {

    float mue = avg(x, size);

    float var = 0;

    for (int i = 0; i < size; ++i) {
        var += pow(x[i] - mue, 2);
    }

    var /= size;
    return var;
};


/**************************************
 *
 * @param x
 * @param y
 * @param size
 * @return the covariance of X and Y
 *************************************/
float cov(float *x, float *y, int size) {

    //find meanX and meanY
    float meanX = avg(x, size);
    float meanY = avg(y, size);

    float cov = 0;

    //find covariance
    for (int i = 0; i < size; ++i) {
        cov += (x[i] - meanX) * (y[i] - meanY);
    }
    cov /= size;

    return cov;
};

/************************************************************
 *
 * @param x
 * @param y
 * @param size
 * @return the Pearson correlation coefficient of X and Y
 ***********************************************************/
float pearson(float *x, float *y, int size) {

    float covXY = cov(x, y, size);

    float varX = var(x, size);
    float varY = var(y, size);

    float pearson = covXY / (sqrt(varX) * sqrt(varY));
    return pearson;
};


/*********************************
 *
 * @param points
 * @param size
 * @return the line equation
 * performs a linear regression
 ***********************************/
Line linear_reg(Point **points, int size) {
    float a, b, covXY, varX, meanX, meanY;

    float *arrayX = new float[size];
    float *arrayY = new float[size];

    for (int i = 0; i < size; ++i) {

        //fill the X array
        arrayX[i] = points[i]->x;
        //fill the Y array
        arrayY[i] = points[i]->y;
    }

    //get means of X and Y
    meanX = avg(arrayX, size);
    meanY = avg(arrayY, size);

    covXY = cov(arrayX, arrayY, size);
    varX = var(arrayX, size);
    a = covXY / varX;
    b = meanY - a * meanX;

    Line linear_reg = Line(a, b);

    return linear_reg;

};

/*********************************************************************************
 *
 * @param p
 * @param points
 * @param size
 * @return the deviation between point p and the line equation of the points
 ********************************************************************************/
float dev(Point p, Point **points, int size) {
    return distance(p, linear_reg(points, size));
};

/************************************************************
 *
 * @param p - point
 * @param l - line
 * @return the deviation between point p and the line
 ***********************************************************/
float distance(Point p, Line l) {
    float dev = fabsf(l.f(p.x) - p.y);
    return dev;
}


/*****************************************************************
 *
 * @param points - the array of points
 * @param numOfPoints
 * @param l - line
 * @return The maximum distance between one of the
 *         collection of points and the given line.
 *****************************************************************/
float maxDev(Point **points, int numOfPoints, Line l) {

    float theDev = 0;
    float maxDev = 0;

    for (int i = 0; i < numOfPoints; ++i) {
        theDev = distance(*points[i], l);
        if (theDev > maxDev) {
            maxDev = theDev;
        }
    }
    return maxDev;
}

/****************************************************************
 *
 * Gets 2 points and calculates the distance between them.
 * @param p1    point
 * @param p2    point
 * @return      distance [square root of |x1-x2|^2 to |y1-y2|^2]
 *****************************************************************/
float distance(Point p1, Point p2) {
    float dev = sqrt( pow(p1.x - p2.x,2) + pow(p1.y - p2.y,2) );
    return dev;
}


