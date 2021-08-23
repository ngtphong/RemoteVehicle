#include <SimpleKalmanFilter.h>
SimpleKalmanFilter L_simpleKalmanFilter(3, 3, 1);
SimpleKalmanFilter R_simpleKalmanFilter(3, 3, 1);
//SimpleKalmanFilter L_showFilter(3, 3, 1);
//SimpleKalmanFilter R_showFilter(3, 3, 1);

#define Throte 27   //Pin RC
#define Drive 26    //Pin RC
#define F_B 28      //Pin RC
#define Active 29   //Pin RC

//RC variable
int F_BVal = 0;
int ActiveVal = 0;
float thrVal = 0;
float drvVal = 0;

//Control variable
double ctrl_spd = 0;
double adj_val = 0;
double L_adj_val = 0;
double R_adj_val = 0;
double turn_deg = 0;
double turn_spd = 0;
double turn_taget = 0;

double Lcheck_speed = 0;
double Rcheck_speed = 0;

boolean ML_start = false;
boolean MR_start = false;
boolean ML_FB = true;
boolean MR_FB = true;
boolean Stopping_Flag = false;
boolean Turning_Flag = false;


struct PIDCal {
  double set_speed;    //6.
  double pv_speed;     //7.
  double e_speed_cur;  //8. Error of speed = set_speed - pv speed
  double e_speed_pre;  //9. Last error of speed
  double e_speed_sum;  //10. Sum error of speed
  double pwm_pulse;    //11.
};

struct Motor {
  const byte spd;      //1. Motor feedback speed
  const byte pmw;      //2. Motor PWM pin
  const byte dir;      //3. Motor direction
  const byte enb;      //4. Motor anable
  double encoder_cnt;  //5.
  struct PIDCal para;
  double kp;  //12.
  double ki;  //13.
  double kd;  //14.
  double pulse_cnt;
};

struct Motor ML = { 2, 11, 23, 22, 0, {0, 0, 0, 0, 0, 0 }, 3, 0.6, 0.075 ,0};
struct Motor MR = { 3, 12, 25, 24, 0, {0, 0, 0, 0, 0, 0 }, 3, 0.6, 0.075 ,0};
double wheel_diameter = 165.0;
double wheel_width = 330.0;


int timer3_counter;
int i = 0;

String mySt = "";
char myChar;
boolean stringComplete = false;


void setup() {
  pinMode(ML.spd, INPUT);   pinMode(MR.spd, INPUT);
  pinMode(ML.pmw, OUTPUT);  pinMode(MR.pmw, OUTPUT);
  pinMode(ML.dir, OUTPUT);  pinMode(MR.dir, OUTPUT);
  pinMode(ML.enb, OUTPUT);  pinMode(MR.enb, OUTPUT);
  pinMode(Throte, INPUT);   pinMode(F_B, INPUT);
  pinMode(Drive, INPUT);    pinMode(Active, INPUT);
  attachInterrupt(digitalPinToInterrupt(MR.spd), detect_MR, RISING);
  attachInterrupt(digitalPinToInterrupt(ML.spd), detect_ML, RISING);

  //--------------------------timer setup
  noInterrupts();
  TCCR3A = 0;
  TCCR3B = 0;
  timer3_counter = 59286;  // preload timer 65536-16MHz/256/10Hz (59286 for 0.1sec)
  TCNT3 = timer3_counter;  // preload timer
  TCCR3B |= (1 << CS12);   // 256 prescaler
  TIMSK3 |= (1 << TOIE1);  // enable timer overflow interrupt
  interrupts();            // enable all interrupts
  //--------------------------timer setup

  Serial.begin(9600);
  analogWrite(ML.pmw, 0);
  analogWrite(MR.pmw, 0);
  digitalWrite(ML.enb, LOW);
  digitalWrite(MR.enb, LOW);
  digitalWrite(ML.dir, LOW);
  digitalWrite(MR.dir, HIGH);
}

void loop() {
  ActiveVal = map(pulseIn(Active, HIGH), 1000, 2000, 0, 1);
  if(ActiveVal == 1){
    listen_RC();   
  }else{
    listen_serial();  
  }
  update_speed();
}
