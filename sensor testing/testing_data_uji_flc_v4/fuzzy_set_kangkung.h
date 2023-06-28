#define FUZZY_SET 0
#include <Fuzzy.h>

typedef struct Kangkung{
//FUZZY SET
  //INPUT
    //Offset EC
    // Kangkung -> ec = 2 - 2.4 mS
    //0.1 minimum
    //2.5 maksimum 

    //Durasi nyala pompa
    //dari 0.1 ke 2.5 berapa lama durasi -> Tmaks = ... sekon
    //dari 
    
    FuzzySet *ec_L = new FuzzySet(-0.6,-0.5,-0.4,-0.2);
    FuzzySet *ec_M = new FuzzySet(-0.4,-0.2,-0.2,0);
    FuzzySet *ec_E = new FuzzySet(-0.2,0,0,0.4);
    FuzzySet *ec_H = new FuzzySet(0,0.4,0.5,0.6);  
    
    //Water Level
    FuzzySet *w_L = new FuzzySet(-2,0,10.9,14.4);
    FuzzySet *w_M = new FuzzySet(10.9,14.4,14.4,20);
    FuzzySet *w_E = new FuzzySet(14.4,20,20,23.6);
    FuzzySet *w_H = new FuzzySet(20,22,24,26);
    
  
  //OUTPUT  
    //AB MIX
    FuzzySet *ab_Z = new FuzzySet(-0.835 ,-0.0003125 ,-0.0003125 ,0.8344);
    FuzzySet *ab_VVS = new FuzzySet(0.8344 ,1.669 ,1.669 ,2.504);
    FuzzySet *ab_VS = new FuzzySet(2.504 ,3.338 ,3.338 ,4.173);
    FuzzySet *ab_S = new FuzzySet(4.173 ,5.008 ,5.008 ,5.843);
    FuzzySet *ab_M = new FuzzySet(5.843 ,6.677 ,6.677 ,7.512);
    FuzzySet *ab_LG = new FuzzySet(7.512 ,8.347 ,8.347 ,9.181);
    FuzzySet *ab_VLG = new FuzzySet(9.181 ,10.02, 10.02, 10.85);
    FuzzySet *ab_VVLG = new FuzzySet(10.85 ,11.69 ,11.69 ,12.52);

    //Water pump
    FuzzySet *wp_Z = new FuzzySet(-23,1.438,1.438,25.88);
    FuzzySet *wp_S = new FuzzySet(25.88,50.32,50.32,74.75);
    FuzzySet *wp_M = new FuzzySet(74.75,99.19,99.19,123.7);
    FuzzySet *wp_LG = new FuzzySet(123.7,148,148,172.5);
    
} Fuzzy_set_kangkung;

typedef struct OutputFuzzy{
  float output_water_pump;
  float output_ab_pump;
} Output_fuzzy;
