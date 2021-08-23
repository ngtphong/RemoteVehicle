
void listen_serial()  {
  if (mySt.substring(0, 8) == "LF_start") {
    motor_drive(0,true,false);   //motor L (0), disable (0), FW (0);
    ML_start = true;
    //Serial.println("LF_started");
  }
  if (mySt.substring(0, 8) == "LB_start") {
    motor_drive(0,true,true);   //motor L (0), disable (0), FW (0);
    ML_start = true;
    //Serial.println("LB_started");
  }
  if (mySt.substring(0, 7) == "ML_stop") {
    motor_drive(0,false,false);   //motor L (0), disable (0), FW (0);
    ML_start = false;
    //Serial.println("MR_stopped");
  }
  if (mySt.substring(0, 12) == "ML_set_speed") {
    Lcheck_speed = (mySt.substring(12, mySt.length()).toFloat())*3/20;
    //Serial.println("L_stp: "); Serial.println(ML.para.set_speed);
  }
  if (mySt.substring(0, 5) == "ML_kp") {
    ML.kp = mySt.substring(5, mySt.length()).toFloat();
    //Serial.print("L_sKp: "); Serial.println(ML.kp);
  }
  if (mySt.substring(0, 5) == "ML_ki") {
    ML.ki = mySt.substring(5, mySt.length()).toFloat();
    //Serial.print("L_sKi: "); Serial.println(ML.ki);
  }
  if (mySt.substring(0, 5) == "ML_kd") {
    ML.kd = mySt.substring(5, mySt.length()).toFloat();
    //Serial.print("L_sKd: "); Serial.println(ML.kd);
  }
  if (mySt.substring(0, 8) == "RF_start") {
    motor_drive(1,true,true);   //motor L (0), disable (0), FW (0);
    MR_start = true;
    //Serial.println("RF_started");
  }
  if (mySt.substring(0, 8) == "RB_start") {
    motor_drive(1,true,false);   //motor L (0), disable (0), FW (0);
    MR_start = true;
    //Serial.println("RB_started");
  }
  if (mySt.substring(0, 7) == "MR_stop") {
    motor_drive(1,false,false);   //motor L (0), disable (0), FW (0);
    MR_start = false;
    //Serial.println("MR_stopped");
  }
  if (mySt.substring(0, 12) == "MR_set_speed") {
    Rcheck_speed = (mySt.substring(12, mySt.length()).toFloat())*3/20;
    //Serial.println("R_stp: "); Serial.println(MR.para.set_speed);
  }
  if (mySt.substring(0, 5) == "MR_kp") {
    MR.kp = mySt.substring(5, mySt.length()).toFloat();
    //Serial.print("R_sKp: "); Serial.println(MR.kp);
  }
  if (mySt.substring(0, 5) == "MR_ki") {
    MR.ki = mySt.substring(5, mySt.length()).toFloat();
    //Serial.print("R_sKi: "); Serial.println(MR.ki);
  }
  if (mySt.substring(0, 5) == "MR_kd") {
    MR.kd = mySt.substring(5, mySt.length()).toFloat();
    //Serial.print("R_sKd: "); Serial.println(MR.kd);
  }

  if (mySt.substring(0, 2) == "CS") {                             //STOP
    ML_start = false;
    MR_start = false;
    ctrl_spd = 0;
    //Serial.println("Stopped");
  }

  if (mySt.substring(0, 2) == "CF") {                             //FORWARD
    ML_start = true; ML_FB = true;
    MR_start = true; MR_FB = true;
    ctrl_spd = (mySt.substring(2,5).toFloat())*3/20;
    //Serial.println("RunF");
  }

  if (mySt.substring(0, 2) == "CB") {                              //BACKWARD
    ML_start = true; ML_FB = false;
    MR_start = true; MR_FB = false;
    ctrl_spd = (mySt.substring(2,5).toFloat())*3/20;
    //Serial.println("RunB");
  }

  if (mySt.substring(0, 2) == "CR") {                              //CW_TURN
    turn_spd = (mySt.substring(2,5).toFloat())*3/20;
    turn_deg = mySt.substring(6,9).toFloat();
    Action_turning(turn_deg,false,turn_spd);
    //Serial.println("RunR");
  }

  if (mySt.substring(0, 2) == "CL") {                              //CCW_TURN
    turn_spd = (mySt.substring(2,5).toFloat())*3/20;
    turn_deg = mySt.substring(6,9).toFloat();
    Action_turning(turn_deg,true,turn_spd);
    //Serial.println("RunL");
  }
  
  if (mySt.substring(5, 6) == "L") {                              // L/R ADJ
    adj_val = (mySt.substring(6,9).toFloat())*0.01;
    L_adj_val = 0 - (adj_val*ctrl_spd);
    R_adj_val = 0 + (adj_val*ctrl_spd);
  }
  
  if (mySt.substring(5, 6) == "R") {                              // L/R ADJ
    adj_val = mySt.substring(6,9).toFloat()*0.01;
    L_adj_val = 0 + (adj_val*ctrl_spd);
    R_adj_val = 0 - (adj_val*ctrl_spd);
  }
  
  if (mySt.substring(5, 6) == "N") {                              // L/R ADJ
    L_adj_val = 0;
    R_adj_val = 0;
    turn_deg = 0;
  }
  if (stringComplete) {
    mySt = "";
    stringComplete = false;
  }
}

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    if (inChar != '\n') {
      mySt += inChar;
    }
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}
