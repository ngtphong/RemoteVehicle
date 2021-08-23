void detect_ML() {
  ML.encoder_cnt += 1;  //increase encoder at new pulse
  if(Stopping_Flag || Turning_Flag) ML.pulse_cnt +=1;
  //Serial.print("ML encoder:");
  //Serial.println(ML.pulse_cnt);
}

void detect_MR() {
  MR.encoder_cnt += 1;  //increase encoder at new pulse
  if(Stopping_Flag || Turning_Flag) MR.pulse_cnt +=1;
  //Serial.print("MR encoder:");
  //Serial.println(MR.pulse_cnt);
}

PIDCal PID_speed(PIDCal input, double Kp, double Ki, double Kd) {
  struct PIDCal result = { 0 };   // Khoi tao struct result return du lieu tinh toan
  result.set_speed = input.set_speed;
  result.pv_speed = input.pv_speed;
  result.e_speed_cur = input.e_speed_cur;
  result.e_speed_pre = input.e_speed_pre;
  result.e_speed_sum = input.e_speed_sum;

  result.e_speed_cur = result.set_speed - result.pv_speed;
  result.pwm_pulse = result.e_speed_cur * Kp + result.e_speed_sum * Ki + result.e_speed_pre * Kd;
  result.e_speed_pre = result.e_speed_cur;
  result.e_speed_sum += result.e_speed_cur;
  return result;
}

void motor_drive (byte Motor_no, boolean Run, boolean Dir)
{
  switch(Motor_no)
  {
    case 0:
      if (Dir) digitalWrite(ML.dir, HIGH); else digitalWrite(ML.dir, LOW);
      if (Run) {
        digitalWrite(ML.enb, HIGH);
        ML_start = true; 
      }else{
        digitalWrite(ML.enb, LOW);
        ML_start = false;
      }
    break;
    
    case 1:
      if (Dir) digitalWrite(MR.dir, HIGH); else digitalWrite(MR.dir, LOW);
      if (Run) {
        digitalWrite(MR.enb, HIGH);
        MR_start = true;  
      }else{
        digitalWrite(MR.enb, LOW);
        MR_start = false; 
      }
    break;
  }
}

double moving_lenght_cal(double leng, double wheeldia)
{
  double pulse = 0;
  return pulse = (leng*90)/(3.14*wheeldia);
}

double moving_angle_cal(double agl, double width, double wheeldia)
{
  double pulse = 0;
  pulse = (agl*width)/(4*wheeldia);
  return pulse;
}

void Action_turning (double agl, boolean dir, double spd)
{
  turn_taget = moving_angle_cal(agl,wheel_width,wheel_diameter);
  ctrl_spd = spd;
  L_adj_val = 0;
  R_adj_val = 0;
  ML.pulse_cnt = 0;
  MR.pulse_cnt = 0;
  if (dir) motor_drive(0,true,false); else motor_drive(0,true,true);
  if (dir) motor_drive(1,true,false); else motor_drive(1,true,true);
  Turning_Flag = true;
}  

void update_speed() {
  //update new speed
  if(ML_start == true)  {
    if (ML.para.pwm_pulse < 255 & ML.para.pwm_pulse > 0) {
      analogWrite(ML.pmw,ML.para.pwm_pulse );
    }
    else if (ML.para.pwm_pulse > 255) {
      analogWrite(ML.pmw, 255);
    }
    else {
      analogWrite(ML.pmw,0);
    }
  }else{
      analogWrite(ML.pmw, 0);
  }
  
  if(MR_start == true)  {
    if (MR.para.pwm_pulse < 255 & MR.para.pwm_pulse > 0) {
      analogWrite(MR.pmw, MR.para.pwm_pulse);
    }
    else if (MR.para.pwm_pulse > 255) {
      analogWrite(MR.pmw, 255);
    }else {
      analogWrite(MR.pmw, 0);
    }
  }else{
    analogWrite(MR.pmw, 0);
  }
}



  
 
