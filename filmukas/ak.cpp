//Anastasija Kiseliova, matematinė informatika, 3 kursas
#include <math.h>
#include <vector>
#include <fstream>
#include <algorithm>
#include <iostream>
#include "BMP24.h"

#define PLOTIS 640
#define AUKSTIS 480

using namespace std;

struct Taskas {
    int32_t x;
    int32_t y;
    Taskas(double x, double y){
        this->x = (int32_t)x;
        this->y = (int32_t)y;
    }

};


struct Spalva {
    int8_t r;
    int8_t g;
    int8_t b;
    Spalva(double r, double g, double b){
        this->r = (int8_t)r * 255;
        this->g = (int8_t)g * 255;
        this->b = (int8_t)b * 255;

    }

};
////////////////////////////////////////////////////////////////////////
void linijinisGradientas(uint32_t x1, uint32_t y1,
    uint32_t x2, uint32_t y2,
    uint32_t xKur, uint32_t yKur,

    uint8_t r1, uint8_t g1, uint8_t b1,
    uint8_t r2, uint8_t g2, uint8_t b2,
    uint8_t& r, uint8_t& g, uint8_t& b)
{

    double P2P1x = x2*1.0 - x1;
    double P2P1y = y2*1.0 - y1;

    double PkurP1x = xKur*1.0 - x1;
    double PkurP1y = yKur*1.0 - y1;

    double distP2P1 = sqrt(P2P1x*P2P1x + P2P1y*P2P1y);
    double distPkurP1 = sqrt(PkurP1x*PkurP1x + PkurP1y*PkurP1y);
    double cos_fi = (P2P1x*PkurP1x + P2P1y*PkurP1y) / (distP2P1*distPkurP1);
    if (cos_fi <= 0) {
        r = r1; g = g1; b = b1;
    }
    else {
        double t = cos_fi * distPkurP1 / distP2P1;

        if (t > 1) {
            r = r2; g = g2; b = b2;
        }
        else {

            r = (uint8_t)(r2 * t + (1 - t) * r1);
            g = (uint8_t)(g2 * t + (1 - t) * g1);
            b = (uint8_t)(b2 * t + (1 - t) * b1);

        }
    }

}
////////////////////////////////////////////////////////////////////////
void radialinisGradientas(uint32_t x1, uint32_t y1, double distP2P1,
    uint32_t xKur, uint32_t yKur,

    uint8_t r1, uint8_t g1, uint8_t b1,
    uint8_t r2, uint8_t g2, uint8_t b2,
    uint8_t& r, uint8_t& g, uint8_t& b)
{

    //  double P2P1x = x2*1.0 - x1;
    //  double P2P1y = y2*1.0 - y1;

    double PkurP1x = xKur*1.0 - x1;
    double PkurP1y = yKur*1.0 - y1;

    //  double distP2P1 = sqrt(P2P1x*P2P1x + P2P1y*P2P1y);  //center<->min(w,h) of bitman dimensions
    double distPkurP1 = sqrt(PkurP1x*PkurP1x + PkurP1y*PkurP1y);    //center<->pixel


    r = distPkurP1 / distP2P1  * (r2 - r1);
    g = distPkurP1 / distP2P1  * (g2 - g1);
    b = distPkurP1 / distP2P1 * (b2 - b1);

}
////////////////////////////////////////////////////////////////////////

void atkarpa(BMP24& pav, Taskas p1, Taskas p2, Spalva s){

    double dt = 0.25 / max(fabs((double)p1.x - (double)p2.x), fabs((double)p1.y - (double)p2.y));
    for (double t = 0; t <= 1.0; t += dt){
        pav.dekTaska((uint32_t)(p1.x*(1 - t) + p2.x*t), (uint32_t)(p1.y*(1 - t) + p2.y*t),
            s.r, s.g, s.b);
    }
}

////////////////////////////////////////////////////////////////////////

void lauzteViena(BMP24& pav, vector<Taskas>& lauzte, Spalva s){

    for (int i = 0; i < lauzte.size() - 1; i++)
        atkarpa(pav, lauzte[i], lauzte[i + 1], s);


    atkarpa(pav, lauzte[0], lauzte[lauzte.size() - 1], s);


}
////////////////////////////////////////////////////////////////////////

//method to check if a Coordinate is located in a polygon
bool checkIsInPolygon(int size, vector<Taskas> poly, float x, float y){
    //this method uses the ray tracing algorithm to determine if the point is in the polygon

    int j = -999;
    int i = -999;
    bool locatedInPolygon = false;

    for (i = 0; i < size; i++){
        //repeat loop for all sets of points
        if (i == (size - 1)){
            //if i is the last vertex, let j be the first vertex
            j = 0;
        }
        else{
            //for all-else, let j=(i+1)th vertex
            j = i + 1;
        }

        float vertY_i = (float)poly[i].y;
        float vertX_i = (float)poly[i].x;
        float vertY_j = (float)poly[j].y;
        float vertX_j = (float)poly[j].x;

        // following statement checks if testPoint.Y is below Y-coord of i-th vertex
        bool belowLowY = vertY_i > y;
        // following statement checks if testPoint.Y is below Y-coord of i+1-th vertex
        bool belowHighY = vertY_j > y;

        /* following statement is true if testPoint.Y satisfies either (only one is possible)
        -->(i).Y < testPoint.Y < (i+1).Y        OR
        -->(i).Y > testPoint.Y > (i+1).Y

        (Note)
        Both of the conditions indicate that a point is located within the edges of the Y-th coordinate
        of the (i)-th and the (i+1)- th vertices of the polygon. If neither of the above
        conditions is satisfied, then it is assured that a semi-infinite horizontal line draw
        to the right from the testpoint will NOT cross the line that connects vertices i and i+1
        of the polygon
        */
        bool withinYsEdges = belowLowY != belowHighY;

        if (withinYsEdges){
            // this is the slope of the line that connects vertices i and i+1 of the polygon
            float slopeOfLine = (vertX_j - vertX_i) / (vertY_j - vertY_i);

            // this looks up the x-coord of a point lying on the above line, given its y-coord
            float pointOnLine = (slopeOfLine* (y - vertY_i)) + vertX_i;

            //checks to see if x-coord of testPoint is smaller than the point on the line with the same y-coord
            bool isLeftToLine = x < pointOnLine;

            if (isLeftToLine){
                //this statement changes true to false (and vice-versa)
                locatedInPolygon = !locatedInPolygon;
            }//end if (isLeftToLine)
        }//end if (withinYsEdges
    }

    return locatedInPolygon;
}
////////////////////////////////////////////////////////////////////////
void spalvinkA(int n, int n1, uint32_t x1, uint32_t y1,
    uint32_t x2, uint32_t y2,

    uint8_t r1, uint8_t g1, uint8_t b1,
    uint8_t r2, uint8_t g2, uint8_t b2,
    vector<Taskas> a, vector<Taskas> vidus, BMP24 &pav){


    for (uint32_t x = 0; x < x2; x++)
        for (uint32_t y = 0; y < y2; y++)
        {

        uint8_t r, g, b;
        linijinisGradientas(x1, y1, y1, y2, x, y, r1, g1, b1, r2, g2, b2, r, g, b);
        if ((checkIsInPolygon(n, a, x, y) == 1) && (checkIsInPolygon(n1, vidus, x, y) == 0))
            pav.dekTaska(x, y, r, g, b);

        }

}
void spalvinkK(int n2, int n3, uint32_t x1, uint32_t y1,
    double distP2P1,

    uint8_t r1, uint8_t g1, uint8_t b1,
    uint8_t r2, uint8_t g2, uint8_t b2,
    vector<Taskas> k, vector<Taskas> k2, BMP24 &pav){


    for (uint32_t x = 0; x < PLOTIS; x++)
        for (uint32_t y = 0; y < AUKSTIS; y++)
        {

        uint8_t r, g, b;
        radialinisGradientas(x1, y1, distP2P1, x, y, r1, g1, b1, r2, g2, b2, r, g, b);
        if ((checkIsInPolygon(n2, k, x, y) == 1) && (checkIsInPolygon(n3, k2, x, y) == 0))
            pav.dekTaska(x, y, r, g, b);

        }

}
////////////////////////////////////////////////////////////////////////

int main(){


    BMP24 pav(PLOTIS, AUKSTIS);

    pav.valyk(128, 128, 128);

    uint32_t r, g, b;

    for (int x = 0; x < PLOTIS; x++)
        for (int y = 0; y < AUKSTIS; y++){
        r = 400 / sqrt((x*1.0 - 320)* (x*1.0 - 320) + (y*1.0 - 240)* (y*1.0 - 240))  * (255 - 186);
        g = 400 / sqrt((x*1.0 - 320)* (x*1.0 - 320) + (y*1.0 - 240)* (y*1.0 - 240)) * (99 - 85);
        b = 400 / sqrt((x*1.0 - 320)* (x*1.0 - 320) + (y*1.0 - 240)* (y*1.0 - 240)) * (71 - 211);
        pav.dekTaska(x, y, r, g, b);
        }

    vector<Taskas> a;
    vector<Taskas> vidus;
    vector<Taskas> k;
    vector<Taskas> k2;

    uint32_t x1, y1, x2, y2, r1, g1, b1, r2, g2, b2;
    double distP2P1;

    ifstream myfile("test.txt");

    int32_t x, y;
    int n, n1, n2, n3;
    char ch;
    string line;
    if (myfile.is_open())

    {
        myfile >> n;
        getline(myfile, line);
        for (int i = 0; i < n; i++){
            myfile >> x >> ch >> y;
            a.push_back(Taskas(x, y));
        }
        getline(myfile, line);
        myfile >> n1;
        getline(myfile, line);
        for (int i = 0; i < n1; i++){
            myfile >> x >> ch >> y;
            vidus.push_back(Taskas(x, y));
        }
        getline(myfile, line);
        myfile >> n2;
        getline(myfile, line);
        for (int i = 0; i < n2; i++){
            myfile >> x >> ch >> y;
            k.push_back(Taskas(x, y));
        }

        getline(myfile, line);
        myfile >> n3;
        getline(myfile, line);
        for (int i = 0; i < n3; i++){
            myfile >> x >> ch >> y;
            k2.push_back(Taskas(x, y));
        }

        getline(myfile, line);
        myfile >> x1 >> ch >> y1 >> ch >> r1 >> ch >> g1 >> ch >> b1;
        getline(myfile, line);
        myfile >> x2 >> ch >> y2 >> ch >> r2 >> ch >> g2 >> ch >> b2;

        cout << x1 << y1 << r1 << g1 << b1 << "\n";
        cout << x2 << y2 << r2 << g2 << b2 << "\n";
        spalvinkA(n, n1, x1, y1, x2, y2, r1, g1, b1, r2, g2, b2, a, vidus, pav);

        getline(myfile, line);
        myfile >> x1 >> ch >> y1 >> ch >> r1 >> ch >> g1 >> ch >> b1;
        getline(myfile, line);
        myfile >> distP2P1 >> ch >> r2 >> ch >> g2 >> ch >> b2;

        cout << x1 << y1 << r1 << g1 << b1 << "\n";
        cout << x2 << y2 << r2 << g2 << b2 << "\n";
        spalvinkK(n2, n3, x1, y1, distP2P1, r1, g1, b1, r2, g2, b2, k, k2, pav);
    }
    myfile.close();


    lauzteViena(pav, a, Spalva(0, 0, 0));
    lauzteViena(pav, vidus, Spalva(0, 0, 0));
    lauzteViena(pav, k, Spalva(0, 0, 0));

    pav.rasykIByla("a.bmp");


}