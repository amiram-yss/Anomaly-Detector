/*
 * HybridAnomalyDetector.cpp
 *
 * Author: 314970054 Ariel Barmats &
 *         314985474 Amiram Yasif
 */
 
 
#include "HybridAnomalyDetector.h"

HybridAnomalyDetector::HybridAnomalyDetector() {
	// TODO Auto-generated constructor stub

}

HybridAnomalyDetector::~HybridAnomalyDetector() {
	// TODO Auto-generated destructor stub
}


void HybridAnomalyDetector::learnHelper(const TimeSeries& ts,float p/*pearson*/,string f1, string f2,Point** ps){
	SimpleAnomalyDetector::learnHelper(ts,p,f1,f2,ps);
	if(p>0.5 && p<threshold){
		Circle cl = findMinCircle(ps,ts.getRowSize());
		correlatedFeatures c;
		c.feature1=f1;
		c.feature2=f2;
		c.corrlation=p;
		c.threshold=cl.radius*1.1; // 10% increase
		c.cx=cl.center.x;
		c.cy=cl.center.y;
		cf.push_back(c);
	}
}

bool HybridAnomalyDetector::isAnomalous(float x,float y,correlatedFeatures c){
	return (c.corrlation>=threshold && SimpleAnomalyDetector::isAnomalous(x,y,c)) ||
			(c.corrlation>0.5 && c.corrlation<threshold && dist(Point(c.cx,c.cy),Point(x,y))>c.threshold);
}
