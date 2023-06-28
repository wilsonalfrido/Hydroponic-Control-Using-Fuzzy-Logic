#define STRUCT_FILE 0 

typedef struct Scn_parameter{
  float water_temp ;
  float ph ;
  float ec ;
  float water_level;
  int flagFLC;
} Scn_parameter;

typedef struct FLC_parameter{
    float offset_ec;
    float ec;
    float ph;
    float water_level;
    float water_temp;
    float ab_dur;
    float wp_dur;
    float set_water_level;
    float set_ec;
    int flc_mode;
} FLC_parameter;

typedef struct timeNow{
  String jam;
  String menit;
  String detik;
} TimeNow;








  
