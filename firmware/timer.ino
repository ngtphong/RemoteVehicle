ISR(TIMER3_OVF_vect) {
  TCNT3 = timer3_counter;


  
  //Collect data for PID calculation
  ML.para.pv_speed = L_simpleKalmanFilter.updateEstimate(ML.encoder_cnt); //ML.encoder_cnt; //60.0 * (ML.encoder_cnt / 90) / 0.1;  //calculate motor speed, unit is rpm
  MR.para.pv_speed = R_simpleKalmanFilter.updateEstimate(MR.encoder_cnt); //MR.encoder_cnt;//60.0 * (MR.encoder_cnt / 90) / 0.1;
  ML.encoder_cnt = 0;
  MR.encoder_cnt = 0;

  //Monitorring current speed
  if (Serial.available() <= 0) {
    Serial.print((60.0 * (ML.para.pv_speed / 90) / 0.1)*100,.0f); Serial.print("L");//L_showFilter.updateEstimate(60.0 * (ML.para.pv_speed / 90) / 0.1));
    Serial.print((60.0 * (MR.para.pv_speed / 90) / 0.1)*100,.0f); Serial.print("R");//R_showFilter.updateEstimate(60.0 * (MR.para.pv_speed / 90) / 0.1));
    Serial.println();
  }


  // calculating input speed
  if (ML_start == true){
    ML.para.set_speed = ctrl_spd + L_adj_val + Lcheck_speed;
    ML.para = PID_speed(ML.para, ML.kp, ML.ki, ML.kd);
  }else{
    ML.para ={ML.para.set_speed, 0, 0, 0, 0, 0};
  }
  if (MR_start == true){
    MR.para.set_speed = ctrl_spd + R_adj_val + Rcheck_speed; 
    MR.para = PID_speed(MR.para, MR.kp, MR.ki, MR.kd);
  }else{
    MR.para ={MR.para.set_speed, 0, 0, 0, 0, 0};
  }

  motor_drive(0,ML_start,!ML_FB);
  motor_drive(1,MR_start,MR_FB);

//  if(ML_start == true && MR_start == true){
//    if (ML_FB == true){
//      motor_drive(0,true,false);   //motor L (0), disable (0), FW (0);
//      motor_drive(1,true,true);   //motor R (1), disable (0), FW (1);
//    }else{
//      motor_drive(0,true,true);   //motor L (0), disable (0), FW (0);
//      motor_drive(1,true,false);   //motor R (1), disable (0), FW (1);
//   }
//  }else{
//    motor_drive(0,false,false);   //motor L (0), disable (0), FW (0);
//    motor_drive(1,false,false);   //motor R (1), disable (0), FW (1);
//    ctrl_spd = 0;
//  }

  //Stop turning action 
  if(Turning_Flag){
    if(ML.pulse_cnt > turn_taget & MR.pulse_cnt > turn_taget){
      motor_drive(0,false,false);
      motor_drive(1,false,false); 
      ML.pulse_cnt = 0;
      MR.pulse_cnt = 0;
      Turning_Flag = false;
    }
  }
}
