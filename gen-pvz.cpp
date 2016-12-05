#include <fstream>
#include <vector>
#include <string>
#include <math.h>

using namespace std;


int main(){

   vector<string> virsunes;
   vector<string> sienos;
   
   ofstream fout("kugis.obj");
   fout << "Generavo generatorius" << endl;
   

   double dfi = M_PI / 30; 
   virsunes.push_back("v 0 1 0");             // Kugio virsune
   virsunes.push_back("v 1 0 0");             // Kugio pagrindo "pirma" virsune
   int vNr = 2;                             // Virsunes numeris 
   for (double fi=0; fi<=M_PI*2;fi+=dfi){
       double x = cos(fi);
       double z = -sin(fi);
       vNr++;
       virsunes.push_back("v "+to_string(x)+" 0 " + to_string(z));             
                                            // Kugio pagrindo eiline virsune
       sienos.push_back("f 1 "+to_string(vNr-1)+" "+to_string(vNr));                                            
   }
   
   for (int i=0;i<virsunes.size();i++)
   	   fout << virsunes[ i ] << endl; 

   for (int i=0;i<sienos.size();i++)
   	   fout << sienos[ i ] << endl; 
  
   fout.close();
   
   return 0;

}


