#include "BMP24.h"
#include <string>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <cmath>
#include <cstdint>


using namespace std;


     void BMP24::nuliai(){
         this->vaizdas = NULL;
     }

     BMP24::BMP24(uint32_t plotis, uint32_t aukstis){
           nuliai();
           this->eilutesIlgis =  plotis * 3;
           if ( this->eilutesIlgis % 4 != 0 )  this->eilutesIlgis +=  (4 - this->eilutesIlgis % 4);            
           this->plotis = plotis;
           this->aukstis = aukstis;  
           this->vaizdoDydis = aukstis *  this -> eilutesIlgis;  
           this->failoDydis = this->vaizdoDydis + this->vaizdoPradzia; 
           this->vaizdas = (uint8_t*) new uint8_t[this->vaizdoDydis];
           memset((void*)  vaizdas, '\0',  this->vaizdoDydis ); 

      }
//   ---------------------------------------------------------------------------
     BMP24::BMP24(string failas){
           //
     uint32_t sk;
     bylosPavadinimas = failas;
     nuliai();

     FILE* fin=fopen(this->bylosPavadinimas.c_str(),"rb");
     if (fin==NULL) {cout << "Blogas failo vardas"; return ; }

     // Signatura BM:
     uint8_t  signat[2]; 
     fread(&signat, 2,1,fin);
  
     fread( &sk,4,1,fin);   
     this->failoDydis=sk;
  
     // Praleidziame 4 baitus;
     fread( &sk,4,1,fin);
     //------------------------------
     fread( &sk,4,1,fin);
   //  this->vaizdoPradzia = sk;    
    
     fread( &sk,4,1,fin);
//     this->aprasoDydis = sk;    



     fread( &(this->plotis),4,1,fin);
     fread( &(this->aukstis),4,1,fin);
     //---------------------------------------------
     sk=0;
     fread( &sk, 2 ,1,fin); // 2 baitus
//     this->bituPlokstumuSkaicius = sk;

     sk=0;
     fread( &sk,2 ,1,fin); // 2 baitu laukas
//     this->bituSkaiciusTaskui=sk;

     fread( &sk,4,1,fin);
//     this->vaizdoSuspaudimoMetodas=sk;

     fread( &sk,4,1,fin);
     this->vaizdoDydis=sk;  

     fread( &sk,4,1,fin);
//     this->rezoliucijaHorizontaliai=sk; 

     fread( &sk,4,1,fin);
//     this->rezoliucijaVertikaliai=sk;  
     

     fread( &sk,4,1,fin);
//     this->spalvuSkaiciusRastre = sk;
    
     fread( &sk,4,1,fin);
//     this->svarbiuSpalvuSkaiciusRastre=sk; 

     this->vaizdas = (uint8_t*) new uint8_t[this->vaizdoDydis];
     memset((void*)  vaizdas, '\0',  this->vaizdoDydis );      

     fread( this->vaizdas,this->vaizdoDydis,1,fin);


     this->eilutesIlgis =  this->plotis * 3;
     if ( this->eilutesIlgis % 4 != 0 )  this->eilutesIlgis +=  (4 - this->eilutesIlgis % 4); 
  
     fclose(fin); 
  



      }

   //  Metodai:

   void BMP24::dekTaska(int32_t x, int32_t y, uint8_t r, uint8_t g, uint8_t b){
     if((x<0)||(x>plotis)||(y<0)||(y>aukstis)) {
                    cerr << "BMP24::dekTaska klaida : Blogai nurodytos tasko koordinates \n"; 
                    return;
          }
        

         int32_t  xX = x;// % plotis;
         int32_t  yY = y;// % aukstis;
         uint32_t vieta =  xX * 3 + this -> eilutesIlgis  * yY;  
         this->vaizdas[ vieta ] = b;
         this->vaizdas[ vieta + 1] = g;
         this->vaizdas[ vieta + 2] = r;
   }
  
  void BMP24::duokTaskoSpalva(int32_t x,int32_t y, uint8_t& r, uint8_t& g, uint8_t& b){
     if((x<0)||(x>plotis)||(y<0)||(y>aukstis)) {
                    cerr << "BMP24::duokTaskoSpalva klaida : Blogai nurodytos tasko koordinates \n"; 
                    
          }


        uint32_t xX = x;// % plotis;
        uint32_t yY = y;// % aukstis;
        uint32_t vieta =  xX * 3 + this -> eilutesIlgis  * yY;  
        
        b = this->vaizdas[ vieta ];
        g = this->vaizdas[ vieta + 1];
        r = this->vaizdas[ vieta + 2] ;
        
     }




   int  BMP24::rasyk(){
     FILE* fout=fopen(this->bylosPavadinimas.c_str(),"wb");
     // Signatura BM:
     fwrite("BM",2,1,fout);

     fwrite( &(this->failoDydis),4,1,fout);
     uint32_t sk=0;
     fwrite( &sk,4,1,fout);
     sk = this->vaizdoPradzia;    

     fwrite( &sk,4,1,fout);

     sk = this->aprasoDydis;    

     fwrite( &sk,4,1,fout);


     fwrite( &(this->plotis),4,1,fout);
     fwrite( &(this->aukstis),4,1,fout);
     sk = this->bituPlokstumuSkaicius;
     fwrite( &sk, 2 ,1,fout); // 2 baitu
     sk=this->bituSkaiciusTaskui;
     fwrite( &sk, 2 ,1,fout); // 2 baitu laukas
     sk=this->vaizdoSuspaudimoMetodas;
     fwrite( &sk,4,1,fout);
     sk=this->vaizdoDydis;  
     fwrite( &sk,4,1,fout);
     sk=this->rezoliucijaHorizontaliai; 
     fwrite( &sk,4,1,fout);
     sk=this->rezoliucijaVertikaliai;  
     fwrite( &sk,4,1,fout);
     sk=this->spalvuSkaiciusRastre;
     fwrite( &sk,4,1,fout);
     sk=this->svarbiuSpalvuSkaiciusRastre; 
     fwrite( &sk,4,1,fout);


     fwrite( this->vaizdas,this->vaizdoDydis,1,fout);

  
     fclose(fout); 
     return 1;

   }  

   int  BMP24::rasykIByla(string bylosPavadinimas){

         this->bylosPavadinimas =    bylosPavadinimas;

         this->rasyk();  
         return 1; 
  
   }

    
int zenklas(int32_t x){
   if(x<0) return -1;
      else if (x==0) return 0;
         else return 1;
  
}


// valymas:
void BMP24::valyk(uint8_t  r, uint8_t  g, uint8_t  b ) 
{

        
    for(uint32_t x=0; x <  this -> plotis; x++)
      for(uint32_t y=0; y < this -> aukstis; y++)
           this->dekTaska(x,y,r,g,b); 

        return;
}


//---------------------------


   //  Destruktorius:

     BMP24::~BMP24(){
       
        delete[] vaizdas;

      }
