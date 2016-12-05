#include <math.h>
#include "BMP24.h"
#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>

//***********Greta Kerpiðkytë*************
//*********MAT INFO 3 KURSAS**************

using namespace std;

string to_string(double i) {
	stringstream ss;
	ss << i;
	return ss.str();
}

int main(){
	BMP24 pav("a.bmp");
	ofstream fout("ak.obj");

	vector<string> v;
	vector<string> f;

	double dx = 0.01;
	double dy = 0.01;
	int vNr = 0;
	double storis = 0.3;
	double arBalta = 765; // 765 = 255 + 255 + 255 (balta spalva)
	bool arRaidesViduj;
	for (int i = 0; i<pav.plotis; i++){
		for (int j = 0; j<pav.aukstis; j++){
			uint8_t r, g, b;
			double s1, s2, s3, s4; // y asyje virsunes iskylumas
			bool vidusNuoKairesVirsaus = false;
			bool vidusNuoApacios = false;
			bool vidusNuoKaires = false;
			bool vidusNuoDesines = false;
			bool vidusNuoVirsaus = false;
			if (i >= 2 && j >= 2 && i <= pav.plotis - 2 && j <= pav.aukstis - 2) { // tikrinam per 2 taskus nuo esancio
				// Virsuniu iskilimas y asyje pagal pilkos atspalvi (s1 - pirmo tasko, s2 - antro, ...)
				pav.duokTaskoSpalva(i - 1, j - 1, r, g, b);
				s1 = (r + g + b) / 3.0;
				pav.duokTaskoSpalva(i - 1, j, r, g, b);
				s2 = (r + g + b) / 3.0;
				pav.duokTaskoSpalva(i, j, r, g, b);
				s3 = (r + g + b) / 3.0;
				pav.duokTaskoSpalva(i, j - 1, r, g, b);
				s4 = (r + g + b) / 3.0;

				//Tikrinam aplink taska ar per 2 tasku atstuma randam balta spalva
				pav.duokTaskoSpalva(i, j + 2, r, g, b);
				if ((r + g + b) == arBalta){
					vidusNuoApacios = true;
				}
				pav.duokTaskoSpalva(i - 2, j, r, g, b);
				if ((r + g + b) == arBalta){
					vidusNuoKaires = true;
				}
				pav.duokTaskoSpalva(i + 2, j, r, g, b);
				if ((r + g + b) == arBalta){
					vidusNuoDesines = true;
				}
				pav.duokTaskoSpalva(i, j - 2, r, g, b);
				if ((r + g + b) == arBalta){
					vidusNuoVirsaus = true;
				}

				pav.duokTaskoSpalva(i - 2, j - 2, r, g, b);
				if ((r + g + b) > arBalta)
					vidusNuoKairesVirsaus = true; //

			}
			pav.duokTaskoSpalva(i, j, r, g, b);

			if ((r + g + b)<arBalta && (vidusNuoApacios == true || vidusNuoKaires == true ||
				vidusNuoDesines == true || vidusNuoVirsaus == true || vidusNuoKairesVirsaus == true)) {
				; //nuimam aplinkine siena, kur gali buti perejimu nuo baltos iki juodos spalvos (sudarytu bereikalingus kalnus)
			}
			else if ((r + g + b) < arBalta){
				int daliklis = 5;
				//Priekine siena
				vNr += 4; // prisidedam 4 virsunes priekiniai sienai sukurti

				//susidedam 4 virsunes is kuriu po to darysim sienas
				v.push_back("v " + to_string(i * dx) + " " + to_string(storis + i / pav.plotis) + " " + to_string(j * dy));
				v.push_back("v " + to_string((i + 1) * dx) + " " + to_string(storis + i / pav.plotis) + " " + to_string(j * dy));
				v.push_back("v " + to_string(i * dx) + " " + to_string(storis + i / pav.plotis) + " " + to_string((j + 1) * dy));
				v.push_back("v " + to_string((i + 1) * dx) + " " + to_string(storis + i / pav.plotis) + " " + to_string((j + 1) * dy));

				//gaminam 2 trikampius is katik pridetu tasku
				f.push_back("f " + to_string(vNr - 3) + " "
					+ to_string(vNr - 1) + " "
					+ to_string(vNr - 2));

				f.push_back("f " + to_string(vNr) + " "
					+ to_string(vNr - 2) + " "
					+ to_string(vNr - 1));
				//Galine siena

				vNr += 4; // dar 4 virsunes galinei sienai sukurti
				v.push_back("v " + to_string(i * dx) + " " + to_string(0 - storis + s1 / 255) + " " + to_string(j * dy));
				v.push_back("v " + to_string((i + 1) * dx) + " " + to_string(0 - storis + s1 / 255) + " " + to_string(j * dy));
				v.push_back("v " + to_string(i * dx) + " " + to_string(0 - storis + s1 / 255) + " " + to_string((j + 1) * dy));
				v.push_back("v " + to_string((i + 1) * dx) + " " + to_string(0 - storis + s1 / 255) + " " + to_string((j + 1) * dy));

				f.push_back("f " + to_string(vNr - 3) + " "
					+ to_string(vNr - 1) + " "
					+ to_string(vNr - 2));

				f.push_back("f " + to_string(vNr) + " "
					+ to_string(vNr - 2) + " "
					+ to_string(vNr - 1));

				//Soniniu sienu kurimas (eiliskumas nesvarbus)
				//Kaire siena
				f.push_back("f " + to_string(vNr - 7) + " "
					+ to_string(vNr - 3) + " "
					+ to_string(vNr - 5));
				f.push_back("f " + to_string(vNr - 5) + " "
					+ to_string(vNr - 3) + " "
					+ to_string(vNr - 1));
				//Desine siena
				f.push_back("f " + to_string(vNr - 6) + " "
					+ to_string(vNr - 2) + " "
					+ to_string(vNr - 4));
				f.push_back("f " + to_string(vNr - 2) + " "
					+ to_string(vNr - 4) + " "
					+ to_string(vNr));
				//Apatine siena
				f.push_back("f " + to_string(vNr - 5) + " "
					+ to_string(vNr - 1) + " "
					+ to_string(vNr - 4));
				f.push_back("f " + to_string(vNr - 1) + " "
					+ to_string(vNr - 4) + " "
					+ to_string(vNr));
				//Virsutine siena
				f.push_back("f " + to_string(vNr - 7) + " "
					+ to_string(vNr - 6) + " "
					+ to_string(vNr - 3));
				f.push_back("f " + to_string(vNr - 6) + " "
					+ to_string(vNr - 3) + " "
					+ to_string(vNr - 2));
			}
		}
	}

	for (int i = 0; i<v.size(); i++)
		fout << v[i] << endl;

	for (int i = 0; i<f.size(); i++)
		fout << f[i] << endl;

	fout.close();
}


