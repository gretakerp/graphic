#ifndef __BMP24_H_
#define __BMP24_H_

#include <string>
#include <stdio.h>
#include <cstdint>

using namespace std;

class BMP24 {
  public:
  uint32_t failoDydis;
  uint32_t nenaudojamas;
  static const uint32_t vaizdoPradzia = 54;
  static const uint32_t aprasoDydis = 40;
  
  uint32_t plotis;
  uint32_t aukstis;

  static const uint32_t bituPlokstumuSkaicius = 1;
  static const uint32_t bituSkaiciusTaskui = 24;

  static const uint32_t vaizdoSuspaudimoMetodas = 0;

  uint32_t vaizdoDydis;
   
  static const uint32_t rezoliucijaHorizontaliai = 0;
  static const uint32_t rezoliucijaVertikaliai = 0;

  static const uint32_t spalvuSkaiciusRastre = 0;
  static const uint32_t svarbiuSpalvuSkaiciusRastre = 0;

  //---------------------------------
  string bylosPavadinimas;   

  //   Konstruktoriai:
  
  // BMP24();
  BMP24(uint32_t plotis, uint32_t aukstis);
  BMP24(string failas);

  // Destruktoriai:
  
  ~BMP24();
  //   Funkcijos:

  void nuliai();
  void dekTaska(int32_t x, int32_t y, uint8_t r, uint8_t g, uint8_t b);
  void duokTaskoSpalva(int32_t x, int32_t y, uint8_t& r, uint8_t& g, uint8_t& b);  
  int rasykIByla(string bylosPavadinimas);
  int rasyk();
  void valyk(uint8_t  r, uint8_t  g ,uint8_t  b ); 

  uint8_t*  vaizdas;
  uint32_t  eilutesIlgis;
  

};


#endif

