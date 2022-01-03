
/*
 * MainTrain.cpp
 *
 *  Created on: 11 áàå÷× 2020
 *      Author: Eli
 */

#include <iostream>
#include <vector>
#include "HybridAnomalyDetector.h"
#include <fstream>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <math.h>

using namespace std;

void generateTrainCSV(float a1,float b1, float a2, float b2, float a3, float b3){
    ofstream out("trainFile.csv");
    out<<"A,B,C,D,E,F"<<endl;
    Line ab(a1,b1);
    Line cd(a2,b2);
    Line ef(a3,b3);
    for(int i=1;i<=200;i++){
        float a=rand()%40;
        float b=ab.f(a)-0.03+(rand()%60)/100.0f;
        float c=rand()%40;
        float d=cd.f(c)-0.03+(rand()%60)/100.0f;
        float r=-100+rand()%200;
        float an=45-40+rand()%70;
        float e=r*cosf(3.14159f*(an)/180);
        float f=r*sinf(3.14159f*(an)/180);
        out<<a<<","<<b<<","<<c<<","<<d<<","<<e<<","<<f<<endl;
    }
    out.close();
}

vector<AnomalyReport> anomalies;
void generateTestCSV(float a1,float b1, float a2, float b2, float a3, float b3){
    ofstream out("testFile.csv");
    out<<"A,B,C,D,E,F"<<endl;
    Line ab(a1,b1);
    Line cd(a2,b2);
    Line ef(a3,b3);
    for(int i=1;i<=200;i++){
        float a=rand()%40;
        float b=ab.f(a)-0.03+(rand()%60)/100.0f;
        float c=rand()%40;
        float d=cd.f(c)-0.03+(rand()%60)/100.0f;
        float r=-100+rand()%200;

        for(auto it=anomalies.begin();it!=anomalies.end();it++){
            if((*it).timeStep==i){
                if((*it).description=="A-B")
                    b++;
                if((*it).description=="C-D")
                    d++;
                if((*it).description=="E-F")
                    r=111;
            }
        }
        float an=45-40+rand()%70;
        float e=r*cosf(3.14159f*(an)/180);
        float f=r*sinf(3.14159f*(an)/180);

        out<<a<<","<<b<<","<<c<<","<<d<<","<<e<<","<<f<<endl;

    }
    out.close();
}

void checkCorrelation(correlatedFeatures c,string f1, string f2, float a, float b){
    if(c.feature1==f1){
        if(c.feature2!=f2)
            cout<<"wrong correlated feature of "<<f1<<" (-10)"<<endl;
        else{
            if(c.corrlation>=0.9){
                if(c.corrlation<0.99)
                    cout<<f1<<"-"<<f2<<" wrong correlation detected (-2)"<<endl;
                if(c.lin_reg.a<a-0.5f || c.lin_reg.a>a+0.5f)
                    cout<<f1<<"-"<<f2<<" wrong value of line_reg.a (-4)"<<endl;
                if(c.lin_reg.b<b-0.5f || c.lin_reg.b>b+0.5f)
                    cout<<f1<<"-"<<f2<<" wrong value of line_reg.b (-4)"<<endl;
            }else{
                if(c.corrlation<=0.5)
                    cout<<f1<<"-"<<f2<<" wrong correlation detected (-2)"<<endl;
                if(c.threshold>111)
                    cout<<f1<<"-"<<f2<<" wrong value of the radius (-18)"<<endl;
            }
        }
    }

}

int main(){
    srand (time(NULL));
    float a1=1+rand()%10, b1=-50+rand()%100;
    float a2=1+rand()%20 , b2=-50+rand()%100;
    float a3=1+rand()%40 , b3=-50+rand()%100;


    // test the learned model: (40 points)
    // expected correlations:
    //	A-B: y=a1*x+b1
    //	C-D: y=a2*x+b2
    //	E-F: y=a3*x+b3

    generateTrainCSV(a1,b1,a2,b2,a3,b3);
    TimeSeries ts("trainFile.csv");
    HybridAnomalyDetector ad;
    ad.learnNormal(ts);
    vector<correlatedFeatures> cf=ad.getNormalModel();

    if(cf.size()!=3)
        cout<<"wrong size of correlated features (-40)"<<endl;
    else
        for_each(cf.begin(),cf.end(),[&a1,&b1,&a2,&b2,&a3,&b3](correlatedFeatures c){
            checkCorrelation(c,"A","B",a1,b1); // 10 points
            checkCorrelation(c,"C","D",a2,b2); // 10 points
            checkCorrelation(c,"E","F",a3,b3); // 20 points
        });

    // test the anomaly detector: (60 points)
    // one simply anomaly is injected to the data
    int anomaly1=5+rand()%90; // anomaly injected in a random time step
    int anomaly2=5+rand()%90; // anomaly injected in a random time step
    int anomaly3=5+rand()%90; // anomaly injected in a random time step

    anomalies.push_back(AnomalyReport("A-B",anomaly1));
    anomalies.push_back(AnomalyReport("C-D",anomaly2));
    anomalies.push_back(AnomalyReport("E-F",anomaly3));

    generateTestCSV(a1,b1,a2,b2,a3,b3);
    TimeSeries ts2("testFile.csv");
    vector<AnomalyReport> r = ad.detect(ts2);
    bool detected[]={false,false,false};

    for_each(r.begin(),r.end(),[&detected](AnomalyReport ar){
        if(ar.description==anomalies[0].description && ar.timeStep==anomalies[0].timeStep)
            detected[0]=true;
        if(ar.description==anomalies[1].description && ar.timeStep==anomalies[1].timeStep)
            detected[1]=true;
        if(ar.description==anomalies[2].description && ar.timeStep==anomalies[2].timeStep)
            detected[2]=true;
    });

    int falseAlarms=r.size();
    for(int i=0;i<3;i++)
        if(!detected[i])
            cout<<"an anomaly was not detected (-10)"<<endl;
        else
            falseAlarms--;

    if(falseAlarms>0)
        cout<<"you have "<<falseAlarms<<" false alarms (-"<<min(30,falseAlarms*3)<<")"<<endl;

    cout<<"done"<<endl;
    return 0;
}


//#############################################
//
//#include <iostream>
//#include <vector>
//#include <stdlib.h>
//#include <time.h>
//#include <math.h>
//#include "minCircle.h"
//#include <chrono>
//#include "anomaly_detection_util.h"
//
//
//using namespace std;
//using namespace std::chrono;
//
//
//Point** generate(Point center,int R, size_t size){
//    Point** p =new Point*[size];
//    for(size_t i=0;i<size;i++){
//        float r=1+rand()%R;
//        float a=3.14159*(rand()%360)/180;
//        float x=center.x+r*cos(a);
//        float y=center.y+r*sin(a);
//        p[i]=new Point(x,y);
//    }
//    return p;
//}
//
//
//int main(){
//    srand (time(NULL));
//    const size_t N=250;
//    float R=10+rand()%1000;
//    float cx=-500+rand()%1001;
//    float cy=-500+rand()%1001;
//    Point** ps=generate(Point(cx,cy),R,N);
//
//    // your working copy
//    Point** ps_copy=new Point*[N];
//    for(size_t i=0;i<N;i++)
//        ps_copy[i]=new Point(ps[i]->x,ps[i]->y);
//
//    auto start = high_resolution_clock::now();
//    Circle c=findMinCircle(ps_copy,N);
//    auto stop = high_resolution_clock::now();
//
//    if((int)c.radius>(int)R)
//        cout<<"you need to find a minimal radius (-40)"<<endl;
//
//    bool covered=true;
//    for(size_t i=0;i<N && covered;i++){
//        float x2=(c.center.x-ps[i]->x)*(c.center.x-ps[i]->x);
//        float y2=(c.center.y-ps[i]->y)*(c.center.y-ps[i]->y);
//        float d=sqrt(x2+y2);
//        if(d>c.radius+1)
//            covered=false;
//    }
//    if(!covered)
//        cout<<"all points should be covered (-45)"<<endl;
//
//    auto duration = duration_cast<microseconds>(stop - start);
//    int stime=duration.count();
//    cout<<"your time: "<<stime<<" microseconds"<<endl;
//    if(stime>3000){
//        cout<<"over time limit ";
//        if(stime<=3500)
//            cout<<"(-5)"<<endl;
//        else if(stime<=4000)
//            cout<<"(-8)"<<endl;
//        else if(stime<=6000)
//            cout<<"(-10)"<<endl;
//        else cout<<"(-15)"<<endl;
//    }
//
//    for(size_t i=0;i<N;i++){
//        delete ps[i];
//        delete ps_copy[i];
//    }
//    delete[] ps;
//    delete[] ps_copy;
//
//    cout<<"done"<<endl;
//    return 0;
//}
