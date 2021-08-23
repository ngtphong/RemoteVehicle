void listen_RC()
{
  thrVal = map(pulseIn(Throte, HIGH), 1945, 1103, -3, 300)*3/20;
  drvVal = map(pulseIn(Drive, HIGH), 1134, 1929, -100, 100);
  F_BVal = map(pulseIn(F_B, HIGH), 1000, 2000, 0, 1);
  ctrl_spd = thrVal;
  adj_val = abs(drvVal)*0.01;

  if(ActiveVal == 1){
    if(F_BVal == 0){  
      ML_start = true; ML_FB = true;
      MR_start = true; MR_FB = true;   
    }else{
      ML_start = true; ML_FB = false;
      MR_start = true; MR_FB = false;
    }

    if(drvVal > 0){
      L_adj_val = 0 + (adj_val*ctrl_spd);
      R_adj_val = 0 - (adj_val*ctrl_spd);
    }else if(drvVal < 0){
      L_adj_val = 0 - (adj_val*ctrl_spd);
      R_adj_val = 0 + (adj_val*ctrl_spd);
    }else{
      L_adj_val = 0;
      R_adj_val = 0; 
    }
    
  }else{
    ML_start = false; MR_start = false;
  }
}
