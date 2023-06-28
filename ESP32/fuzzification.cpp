#define FUZZIFICATION 0
#include "fuzzification.h"

#ifndef FUZZY_LIB
#include <Fuzzy.h>
#endif 

Fuzzy *fuzzy = new Fuzzy();

Fuzzy_set_kangkung fuzzy_kangkung;

Output_fuzzy output_fuzzy_kangkung(float input_ec, float input_water_level){

  Output_fuzzy fuzzy_kangkung_output;


  fuzzy->setInput(1,input_ec);
  fuzzy->setInput(2,input_water_level);
  fuzzy->fuzzify();

  fuzzy_kangkung_output.output_ab_pump = fuzzy->defuzzify(1);
  fuzzy_kangkung_output.output_water_pump = fuzzy->defuzzify(2);

  return fuzzy_kangkung_output;
}



void fuzzification(){
  //INPUT
    //EC
    FuzzyInput *ec = new FuzzyInput(1);
    
    ec->addFuzzySet(fuzzy_kangkung.ec_L);
    ec->addFuzzySet(fuzzy_kangkung.ec_M);
    ec->addFuzzySet(fuzzy_kangkung.ec_E);
    ec->addFuzzySet(fuzzy_kangkung.ec_H);
    fuzzy->addFuzzyInput(ec);

    //Water level
    FuzzyInput *w = new FuzzyInput(2);
    
    w->addFuzzySet(fuzzy_kangkung.w_L);
    w->addFuzzySet(fuzzy_kangkung.w_M);
    w->addFuzzySet(fuzzy_kangkung.w_E);
    w->addFuzzySet(fuzzy_kangkung.w_H);
    fuzzy->addFuzzyInput(w);   


  //OUTPUT

    //AB MIX
    FuzzyOutput *ab = new FuzzyOutput(1);
    
    ab->addFuzzySet(fuzzy_kangkung.ab_Z);
    ab->addFuzzySet(fuzzy_kangkung.ab_VVS);
    ab->addFuzzySet(fuzzy_kangkung.ab_VS);
    ab->addFuzzySet(fuzzy_kangkung.ab_S);
    ab->addFuzzySet(fuzzy_kangkung.ab_M);
    ab->addFuzzySet(fuzzy_kangkung.ab_LG);
    ab->addFuzzySet(fuzzy_kangkung.ab_VLG);
    ab->addFuzzySet(fuzzy_kangkung.ab_VVLG);
    fuzzy->addFuzzyOutput(ab);

    //water pump
    FuzzyOutput *wp = new FuzzyOutput(2);

    wp->addFuzzySet(fuzzy_kangkung.wp_Z);
    wp->addFuzzySet(fuzzy_kangkung.wp_S);
    wp->addFuzzySet(fuzzy_kangkung.wp_M);
    wp->addFuzzySet(fuzzy_kangkung.wp_LG);
    fuzzy->addFuzzyOutput(wp);
    
    
  
}

void fuzzy_rule(){

  //rule 1
  FuzzyRuleAntecedent *ec_L_w_L = new FuzzyRuleAntecedent();
  ec_L_w_L->joinWithAND(fuzzy_kangkung.ec_L,fuzzy_kangkung.w_L);

  FuzzyRuleConsequent *ab_VVLG_wp_LG = new FuzzyRuleConsequent();
  ab_VVLG_wp_LG->addOutput(fuzzy_kangkung.ab_VVLG);
  ab_VVLG_wp_LG->addOutput(fuzzy_kangkung.wp_LG);

  FuzzyRule *fuzzyRule1 = new FuzzyRule(1,ec_L_w_L,ab_VVLG_wp_LG);
  fuzzy->addFuzzyRule(fuzzyRule1);
  
  //rule 2
  FuzzyRuleAntecedent *ec_L_w_M = new FuzzyRuleAntecedent();
  ec_L_w_M->joinWithAND(fuzzy_kangkung.ec_L,fuzzy_kangkung.w_M);

  FuzzyRuleConsequent *ab_VLG_wp_M = new FuzzyRuleConsequent();
  ab_VLG_wp_M->addOutput(fuzzy_kangkung.ab_VLG);
  ab_VLG_wp_M->addOutput(fuzzy_kangkung.wp_M);

  FuzzyRule *fuzzyRule2 = new FuzzyRule(2,ec_L_w_M,ab_VLG_wp_M);
  fuzzy->addFuzzyRule(fuzzyRule2);

  //rule 3
  FuzzyRuleAntecedent *ec_L_w_E = new FuzzyRuleAntecedent();
  ec_L_w_E->joinWithAND(fuzzy_kangkung.ec_L,fuzzy_kangkung.w_E);

  FuzzyRuleConsequent *ab_VS_wp_Z = new FuzzyRuleConsequent();
  ab_VS_wp_Z->addOutput(fuzzy_kangkung.ab_VS);
  ab_VS_wp_Z->addOutput(fuzzy_kangkung.wp_Z);

  FuzzyRule *fuzzyRule3 = new FuzzyRule(3,ec_L_w_E,ab_VS_wp_Z);
  fuzzy->addFuzzyRule(fuzzyRule3);

  //rule 4
  FuzzyRuleAntecedent *ec_L_w_H = new FuzzyRuleAntecedent();
  ec_L_w_H->joinWithAND(fuzzy_kangkung.ec_L,fuzzy_kangkung.w_H);

//  FuzzyRuleConsequent *ab_VS_wp_Z = new FuzzyRuleConsequent();
  ab_VS_wp_Z->addOutput(fuzzy_kangkung.ab_VS);
  ab_VS_wp_Z->addOutput(fuzzy_kangkung.wp_Z);

  FuzzyRule *fuzzyRule4 = new FuzzyRule(4,ec_L_w_H,ab_VS_wp_Z);
  fuzzy->addFuzzyRule(fuzzyRule4);
  
  //rule 5
  FuzzyRuleAntecedent *ec_M_w_L = new FuzzyRuleAntecedent();
  ec_M_w_L->joinWithAND(fuzzy_kangkung.ec_M,fuzzy_kangkung.w_L);

//  FuzzyRuleConsequent *ab_VVLG_wp_LG = new FuzzyRuleConsequent();
  ab_VVLG_wp_LG->addOutput(fuzzy_kangkung.ab_VVLG);
  ab_VVLG_wp_LG->addOutput(fuzzy_kangkung.wp_LG);

  FuzzyRule *fuzzyRule5 = new FuzzyRule(5,ec_M_w_L,ab_VVLG_wp_LG);
  fuzzy->addFuzzyRule(fuzzyRule5);
  
  //rule 6
  FuzzyRuleAntecedent *ec_M_w_M = new FuzzyRuleAntecedent();
  ec_M_w_M->joinWithAND(fuzzy_kangkung.ec_M,fuzzy_kangkung.w_M);

  FuzzyRuleConsequent *ab_LG_wp_M = new FuzzyRuleConsequent();
  ab_LG_wp_M->addOutput(fuzzy_kangkung.ab_LG);
  ab_LG_wp_M->addOutput(fuzzy_kangkung.wp_M);

  FuzzyRule *fuzzyRule6 = new FuzzyRule(6,ec_M_w_M,ab_LG_wp_M);
  fuzzy->addFuzzyRule(fuzzyRule6);
  
  //rule 7
  FuzzyRuleAntecedent *ec_M_w_E = new FuzzyRuleAntecedent();
  ec_M_w_E->joinWithAND(fuzzy_kangkung.ec_M,fuzzy_kangkung.w_E);

  FuzzyRuleConsequent *ab_VVS_wp_Z = new FuzzyRuleConsequent();
  ab_VVS_wp_Z->addOutput(fuzzy_kangkung.ab_VVS);
  ab_VVS_wp_Z->addOutput(fuzzy_kangkung.wp_Z);

  FuzzyRule *fuzzyRule7 = new FuzzyRule(7,ec_M_w_E,ab_VVS_wp_Z);
  fuzzy->addFuzzyRule(fuzzyRule7);
  
  //rule 8
  FuzzyRuleAntecedent *ec_M_w_H = new FuzzyRuleAntecedent();
  ec_M_w_H->joinWithAND(fuzzy_kangkung.ec_M,fuzzy_kangkung.w_H);

//  FuzzyRuleConsequent *ab_VVS_wp_Z = new FuzzyRuleConsequent();
  ab_VVS_wp_Z->addOutput(fuzzy_kangkung.ab_VVS);
  ab_VVS_wp_Z->addOutput(fuzzy_kangkung.wp_Z);

  FuzzyRule *fuzzyRule8 = new FuzzyRule(8,ec_M_w_H,ab_VVS_wp_Z);
  fuzzy->addFuzzyRule(fuzzyRule8);
  
  //rule 9
  FuzzyRuleAntecedent *ec_E_w_L = new FuzzyRuleAntecedent();
  ec_E_w_L->joinWithAND(fuzzy_kangkung.ec_E,fuzzy_kangkung.w_L);

  FuzzyRuleConsequent *ab_VLG_wp_LG = new FuzzyRuleConsequent();
  ab_VLG_wp_LG->addOutput(fuzzy_kangkung.ab_VLG);
  ab_VLG_wp_LG->addOutput(fuzzy_kangkung.wp_LG);

  FuzzyRule *fuzzyRule9 = new FuzzyRule(9,ec_E_w_L,ab_VLG_wp_LG);
  fuzzy->addFuzzyRule(fuzzyRule9);
    
  //rule 10
  FuzzyRuleAntecedent *ec_E_w_M = new FuzzyRuleAntecedent();
  ec_E_w_M->joinWithAND(fuzzy_kangkung.ec_E,fuzzy_kangkung.w_M);

  FuzzyRuleConsequent *ab_M_wp_M = new FuzzyRuleConsequent();
  ab_M_wp_M->addOutput(fuzzy_kangkung.ab_M);
  ab_M_wp_M->addOutput(fuzzy_kangkung.wp_M);

  FuzzyRule *fuzzyRule10 = new FuzzyRule(10,ec_E_w_M,ab_M_wp_M);
  fuzzy->addFuzzyRule(fuzzyRule10);
  
  //rule 11
  FuzzyRuleAntecedent *ec_E_w_E = new FuzzyRuleAntecedent();
  ec_E_w_E->joinWithAND(fuzzy_kangkung.ec_E,fuzzy_kangkung.w_E);

  FuzzyRuleConsequent *ab_Z_wp_Z = new FuzzyRuleConsequent();
  ab_Z_wp_Z->addOutput(fuzzy_kangkung.ab_Z);
  ab_Z_wp_Z->addOutput(fuzzy_kangkung.wp_Z);

  FuzzyRule *fuzzyRule11 = new FuzzyRule(11,ec_E_w_E,ab_Z_wp_Z);
  fuzzy->addFuzzyRule(fuzzyRule11);
    
  //rule 12
  FuzzyRuleAntecedent *ec_E_w_H = new FuzzyRuleAntecedent();
  ec_E_w_H->joinWithAND(fuzzy_kangkung.ec_E,fuzzy_kangkung.w_H);

//  FuzzyRuleConsequent *ab_Z_wp_Z = new FuzzyRuleConsequent();
  ab_Z_wp_Z->addOutput(fuzzy_kangkung.ab_Z);
  ab_Z_wp_Z->addOutput(fuzzy_kangkung.wp_Z);

  FuzzyRule *fuzzyRule12 = new FuzzyRule(12,ec_E_w_H,ab_Z_wp_Z);
  fuzzy->addFuzzyRule(fuzzyRule12);
    
  //rule 13
  FuzzyRuleAntecedent *ec_H_w_L = new FuzzyRuleAntecedent();
  ec_H_w_L->joinWithAND(fuzzy_kangkung.ec_H,fuzzy_kangkung.w_L);

  FuzzyRuleConsequent *ab_LG_wp_LG = new FuzzyRuleConsequent();
  ab_LG_wp_LG->addOutput(fuzzy_kangkung.ab_LG);
  ab_LG_wp_LG->addOutput(fuzzy_kangkung.wp_LG);

  FuzzyRule *fuzzyRule13 = new FuzzyRule(13,ec_H_w_L,ab_LG_wp_LG);
  fuzzy->addFuzzyRule(fuzzyRule13);
    
  //rule 14
  FuzzyRuleAntecedent *ec_H_w_M = new FuzzyRuleAntecedent();
  ec_H_w_M->joinWithAND(fuzzy_kangkung.ec_H,fuzzy_kangkung.w_M);

  FuzzyRuleConsequent *ab_S_wp_M = new FuzzyRuleConsequent();
  ab_S_wp_M->addOutput(fuzzy_kangkung.ab_S);
  ab_S_wp_M->addOutput(fuzzy_kangkung.wp_M);

  FuzzyRule *fuzzyRule14 = new FuzzyRule(14,ec_H_w_M,ab_S_wp_M);
  fuzzy->addFuzzyRule(fuzzyRule14);

  //rule 15
  FuzzyRuleAntecedent *ec_H_w_E = new FuzzyRuleAntecedent();
  ec_H_w_E->joinWithAND(fuzzy_kangkung.ec_H,fuzzy_kangkung.w_E);

  FuzzyRuleConsequent *ab_Z_wp_S = new FuzzyRuleConsequent();
  ab_Z_wp_S->addOutput(fuzzy_kangkung.ab_Z);
  ab_Z_wp_S->addOutput(fuzzy_kangkung.wp_S);

  FuzzyRule *fuzzyRule15 = new FuzzyRule(15,ec_H_w_E,ab_Z_wp_S);
  fuzzy->addFuzzyRule(fuzzyRule15);

  //rule 16
  FuzzyRuleAntecedent *ec_H_w_H = new FuzzyRuleAntecedent();
  ec_H_w_H->joinWithAND(fuzzy_kangkung.ec_H,fuzzy_kangkung.w_H);

//  FuzzyRuleConsequent *ab_Z_wp_Z = new FuzzyRuleConsequent();
  ab_Z_wp_Z->addOutput(fuzzy_kangkung.ab_Z);
  ab_Z_wp_Z->addOutput(fuzzy_kangkung.wp_Z);

  FuzzyRule *fuzzyRule16 = new FuzzyRule(16,ec_H_w_H,ab_Z_wp_Z);
  fuzzy->addFuzzyRule(fuzzyRule16);
}
