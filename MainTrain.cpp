//
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "minCircle.h"
#include <chrono>
#include "anomaly_detection_util.h"


using namespace std;
using namespace std::chrono;


Point** generate(Point center,int R, size_t size){
    Point** p =new Point*[size];
    for(size_t i=0;i<size;i++){
        float r=1+rand()%R;
        float a=3.14159*(rand()%360)/180;
        float x=center.x+r*cos(a);
        float y=center.y+r*sin(a);
        p[i]=new Point(x,y);
    }
    return p;
}


int main(){
    srand (time(NULL));
    const size_t N=250;
    float R=10+rand()%1000;
    float cx=-500+rand()%1001;
    float cy=-500+rand()%1001;
    Point** ps=generate(Point(cx,cy),R,N);

    // your working copy
    Point** ps_copy=new Point*[N];
    for(size_t i=0;i<N;i++)
        ps_copy[i]=new Point(ps[i]->x,ps[i]->y);

    auto start = high_resolution_clock::now();
    Circle c=findMinCircle(ps_copy,N);
    auto stop = high_resolution_clock::now();

    if((int)c.radius>(int)R)
        cout<<"you need to find a minimal radius (-40)"<<endl;

    bool covered=true;
    for(size_t i=0;i<N && covered;i++){
        float x2=(c.center.x-ps[i]->x)*(c.center.x-ps[i]->x);
        float y2=(c.center.y-ps[i]->y)*(c.center.y-ps[i]->y);
        float d=sqrt(x2+y2);
        if(d>c.radius+1)
            covered=false;
    }
    if(!covered)
        cout<<"all points should be covered (-45)"<<endl;

    auto duration = duration_cast<microseconds>(stop - start);
    int stime=duration.count();
    cout<<"your time: "<<stime<<" microseconds"<<endl;
    if(stime>3000){
        cout<<"over time limit ";
        if(stime<=3500)
            cout<<"(-5)"<<endl;
        else if(stime<=4000)
            cout<<"(-8)"<<endl;
        else if(stime<=6000)
            cout<<"(-10)"<<endl;
        else cout<<"(-15)"<<endl;
    }

    for(size_t i=0;i<N;i++){
        delete ps[i];
        delete ps_copy[i];
    }
    delete[] ps;
    delete[] ps_copy;

    cout<<"done"<<endl;
    return 0;
}

//################################################################################################

//
//#include <iostream>
//#include <vector>
//#include "AnomalyDetector.h"
//#include "SimpleAnomalyDetector.h"
//#include <fstream>
//#include <stdlib.h>     /* srand, rand */
//#include <time.h>       /* time */
//#include <math.h>
//
//using namespace std;
//
//// this is a simple test to put you on the right track
//void generateTrainCSV(float a1,float b1, float a2, float b2){
//    ofstream out("trainFile1.csv");
//    out<<"A,B,C,D"<<endl;
//    Line ac(a1,b1);
//    Line bd(a2,b2);
//    for(int i=1;i<=100;i++){
//        float a=i;
//        float b=rand()%40;
//        out<<a<<","<<b<<","<<ac.f(a)-0.02+(rand()%40)/100.0f<<","<<bd.f(b)-0.02+(rand()%40)/100.0f<<endl;
//    }
//    out.close();
//}
//
//void generateTestCSV(float a1,float b1, float a2, float b2, int anomaly){
//    ofstream out("testFile1.csv");
//    out<<"A,B,C,D"<<endl;
//    Line ac(a1,b1);
//    Line bd(a2,b2);
//    for(int i=1;i<=100;i++){
//        float a=i;
//        float b=rand()%40;
//        if(i!=anomaly)
//            out<<a<<","<<b<<","<<ac.f(a)-0.02+(rand()%40)/100.0f<<","<<bd.f(b)-0.02+(rand()%40)/100.0f<<endl;
//        else
//            out<<a<<","<<b<<","<<ac.f(a)+1<<","<<bd.f(b)-0.02+(rand()%40)/100.0f<<endl;
//    }
//    out.close();
//}
//
//void checkCorrelationTrain(correlatedFeatures c,string f1, string f2, float a, float b){
//    if(c.feature1==f1){
//        if(c.feature2!=f2)
//            cout<<"wrong correlated feature of "<<f1<<" (-20)"<<endl;
//        else{
//            if(c.corrlation<0.99)
//                cout<<f1<<"-"<<f2<<" wrong correlation detected (-5)"<<endl;
//            if(c.lin_reg.a<a-0.5f || c.lin_reg.a>a+0.5f)
//                cout<<f1<<"-"<<f2<<" wrong value of line_reg.a (-5)"<<endl;
//            if(c.lin_reg.b<b-0.5f || c.lin_reg.b>b+0.5f)
//                cout<<f1<<"-"<<f2<<" wrong value of line_reg.b (-5)"<<endl;
//            if(c.threshold>0.3)
//                cout<<f1<<"-"<<f2<<" wrong threshold detected (-5)"<<endl;
//        }
//    }
//
//}
//
//int main(){
//    srand (time(NULL));
//    float a1=1+rand()%10, b1=-50+rand()%100;
//    float a2=1+rand()%20 , b2=-50+rand()%100;
//
//
//    // test the learned model: (40 points)
//    // expected correlations:
//    //	A-C: y=a1*x+b1
//    //	B-D: y=a2*x+b2
//
//    generateTrainCSV(a1,b1,a2,b2);
//    TimeSeries ts("trainFile1.csv");
//    SimpleAnomalyDetector ad;
//    ad.learnNormal(ts);
//    vector<correlatedFeatures> cf=ad.getNormalModel();
//
//    if(cf.size()!=2)
//        cout<<"wrong size of correlated features (-40)"<<endl;
//    else
//        for_each(cf.begin(),cf.end(),[&a1,&b1,&a2,&b2](correlatedFeatures c){
//            checkCorrelationTrain(c,"A","C",a1,b1); // 20 points
//            checkCorrelationTrain(c,"B","D",a2,b2); // 20 points
//        });
//
//    // test the anomaly detector: (60 points)
//    // one simply anomaly is injected to the data
//    int anomaly=5+rand()%90; // one anomaly injected in a random time step
//    generateTestCSV(a1,b1,a2,b2,anomaly);
//    TimeSeries ts2("testFile1.csv");
//    vector<AnomalyReport> r = ad.detect(ts2);
//
//    bool anomlyDetected=false;
//    int falseAlarms=0;
//    for_each(r.begin(),r.end(),[&anomaly,&anomlyDetected,&falseAlarms](AnomalyReport ar){
//        if(ar.description=="A-C" && ar.timeStep == anomaly)
//            anomlyDetected=true;
//        else
//            falseAlarms++;
//    });
//
//    if(!anomlyDetected)
//        cout<<"the anomaly was not detected (-30)"<<endl;
//
//    if(falseAlarms>0)
//        cout<<"you have "<<falseAlarms<<" false alarms (-"<<min(30,falseAlarms*3)<<")"<<endl;
//
//    cout<<"done"<<endl;
//    return 0;
//}