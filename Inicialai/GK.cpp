#include <math.h>
#include "BMP24.h"
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

#define PLOTIS  700
#define AUKSTIS  500
#define KRASTINE 3

struct Taskas {
	uint32_t x;
	uint32_t y;
	uint32_t r;
	uint32_t g;
	uint32_t b;
	int radius;
};

int pirmasNr, antrasNr, konturoTaskai = 0;
vector<Taskas> pirmasTaskas;
vector<Taskas> antrasTaskas;
vector<Taskas> konturoTaskuMasyvas;
Taskas pirmasStart;
Taskas pirmasEnd;
Taskas antrasStart;
Taskas antrasEnd;

void duomenys(string duomenuPavad);
void piestiIniciala(BMP24 &pav, vector<Taskas> Taskai, int taskuSkaicius);
void krastinesPiesimas(BMP24 &pav, Taskas start, Taskas end);
void linijinisGradientas(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint32_t xKur, uint32_t yKur, uint8_t r1, uint8_t g1, uint8_t b1, uint8_t r2, uint8_t g2, uint8_t b2, uint8_t& r, uint8_t& g, uint8_t& b);
bool ppoly(int vertices, vector<Taskas> Taskai, double x, double y);


int main(){
	BMP24 pav(PLOTIS, AUKSTIS);
	pav.valyk(255,255,255);
	duomenys("data.txt");

	for(uint32_t x = 0; x < PLOTIS; x++) {
		for(uint32_t y = 0; y < AUKSTIS; y++){
			uint8_t r,g,b;
			if( ppoly(pirmasNr, pirmasTaskas, x, y) ){
				linijinisGradientas(pirmasStart.x, pirmasStart.y,
									pirmasEnd.x, pirmasEnd.y,
									x, y,
									pirmasStart.r, pirmasStart.g, pirmasStart.b,
									pirmasEnd.r, pirmasEnd.g, pirmasEnd.b,
									r, g, b);
				pav.dekTaska(x,y,r,g,b);
			}
					if( ppoly(konturoTaskai, konturoTaskuMasyvas, x, y) ){
				pav.dekTaska(x,y,255,255,255);
			}
			if( ppoly(antrasNr, antrasTaskas, x, y) ){

				if( (((antrasStart.x - x) * (antrasStart.x - x)) +  ((antrasStart.y - y) * (antrasStart.y - y) )) < (antrasEnd.radius * antrasEnd.radius) ) {
					double a = (sqrt(((antrasStart.x - x) * (antrasStart.x - x)) +  ((antrasStart.y - y) * (antrasStart.y - y) )) / antrasEnd.radius);
					r = antrasStart.r *(1-a) + a*antrasEnd.r;
					g = antrasStart.g * (1-a) + a*antrasEnd.g;
					b = antrasStart.b * (1-a) + a*antrasEnd.b;
					pav.dekTaska(x,y,r,g,b);
				}


			}

		}
	}

	piestiIniciala(pav, pirmasTaskas, pirmasNr);
	piestiIniciala(pav, antrasTaskas, antrasNr);

	if(konturoTaskai > 0 ){

		piestiIniciala(pav, konturoTaskuMasyvas, konturoTaskai);
	}

	pav.rasykIByla("GK.bmp");
}

void duomenys(string duomenuPavad) {
	Taskas p;
	bool vidinis = false;
	ifstream file (duomenuPavad);

	if (file.is_open()) {
		ifstream fd;
		fd.open(duomenuPavad);

		fd >> pirmasNr;
		for(int i = 0; i < pirmasNr; ++i){
			fd >> p.x;
			if(p.x == -1 && !vidinis){
				vidinis = true;
				fd >> p.x;
				konturoTaskuMasyvas.push_back(p);
			}
			if(!vidinis){
				fd >> p.y;
				pirmasTaskas.push_back(p);
			} else {
				fd >> p.y;
				konturoTaskuMasyvas.push_back(p);
				konturoTaskai++;
			}
		}
		pirmasNr -= konturoTaskai;
		fd.ignore(80, '\n');

		fd >> antrasNr;
		for(int i = 0; i < antrasNr; ++i){
			fd >> p.x >> p.y;
			antrasTaskas.push_back(p);
		}
		fd.ignore(80, '\n');

		fd >> pirmasStart.x >> pirmasStart.y >> pirmasStart.r >> pirmasStart.g >> pirmasStart.b;
		fd >> pirmasEnd.x >> pirmasEnd.y >> pirmasEnd.r >> pirmasEnd.g >> pirmasEnd.b;

		fd.ignore(80, '\n');

		fd >> antrasStart.x >> antrasStart.y >> antrasStart.r >> antrasStart.g >> antrasStart.b;
		fd >> antrasEnd.radius >> antrasEnd.r >> antrasEnd.g >> antrasEnd.b;

		fd.close();
	}
	else{
		cout << "Unable to open file";
	}
}

void krastinesPiesimas(BMP24 &pav, Taskas start, Taskas end){
	int x0 = (int)start.x, y0 = (int)start.y, x1 = (int)end.x, y1 = (int)end.y;
	int dx = fabs(x1 - x0);
	int	dy = fabs(y1 - y0);
	int sx = (x0 < x1) ? 1 : -1, sy = (y0 < y1) ? 1 : -1;
	int err = dx - dy;

	while (true)
	{


		for (double t = 0; t < 2 * 3.14; t += 0.01){
			int32_t xt =  (int32_t)(x0 + KRASTINE/2 * sin(t));
			int32_t yt = (int32_t)(y0 + KRASTINE/2 * cos(t));
			pav.dekTaska(xt, yt,0,0,0);
		}
		if (x0 == x1 && y0 == y1) return;
		int e2 = (err << 1);
		if (e2 > -dy)
		{
			err -= dy;
			x0 += sx;
		}
		if (e2 < dx)
		{
			err += dx;
			y0 += sy;
		}
	}
}

bool ppoly(int TaskaiNumber, vector<Taskas> Taskai, double x, double y){

    int i = -999, j = -999;
    bool insidePolygon = false;
    for(int i = 0; i < TaskaiNumber; i++){
        if(i == (TaskaiNumber - 1)){
            j = 0;
        }
        else{
            j = i+1;
        }
        double iVertX = Taskai[i].x;
        double iVertY = Taskai[i].y;
        double jVertX = Taskai[j].x;
        double jVertY = Taskai[j].y;
        bool iBelowY = iVertY > y;
        bool iPlusBelowY = jVertY > y;

        bool withinYEdges = iBelowY != iPlusBelowY;

        if(withinYEdges){
            double lineSlope = (jVertX - iVertX)/(jVertY - iVertY);
            double TaskasOnLine = (lineSlope *(y - iVertY)) + iVertX;

            bool leftToLine = x < TaskasOnLine;

            if(leftToLine){
                insidePolygon = !insidePolygon;
            }
        }
    }
    return insidePolygon;
}

void linijinisGradientas(uint32_t x1, uint32_t y1,
                         uint32_t x2, uint32_t y2,
                         uint32_t xKur, uint32_t yKur,

                         uint8_t r1, uint8_t g1, uint8_t b1,
                         uint8_t r2, uint8_t g2, uint8_t b2,
                         uint8_t& r, uint8_t& g, uint8_t& b){

   double P2P1x = x2*1.0-x1;
   double P2P1y = y2*1.0-y1;

   double PkurP1x = xKur*1.0-x1;
   double PkurP1y = yKur*1.0-y1;

   double distP2P1 = sqrt(P2P1x*P2P1x+P2P1y*P2P1y);
   double distPkurP1 = sqrt(PkurP1x*PkurP1x+PkurP1y*PkurP1y);
   double cos_fi = (P2P1x*PkurP1x + P2P1y*PkurP1y)/(distP2P1*distPkurP1);
   if (cos_fi <= 0) {
     r = r1; g = g1; b = b1;
   }
   else {
     double t = cos_fi * distPkurP1 / distP2P1;

     if(t>1) {
       r = r2; g = g2; b = b2;
     }
     else {

       r = (uint8_t) (r2 * t + (1-t) * r1);
       g = (uint8_t) (g2 * t + (1-t) * g1);
       b = (uint8_t) (b2 * t + (1-t) * b1);

     }
  }

}


void piestiIniciala(BMP24 &pav, vector<Taskas> Taskai, int taskuSkaicius){
	if(taskuSkaicius > 0){
		for(int i = 0; i < taskuSkaicius - 1; ++i){
			krastinesPiesimas(pav, Taskai[i], Taskai[i+1]);
		}
		krastinesPiesimas(pav, Taskai[taskuSkaicius-1], Taskai[0]);
	}
}
