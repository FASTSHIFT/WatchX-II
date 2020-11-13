* Test Wakeup */
static void  Loop_Test_Wakeup(void)
{
  LIS2DS12_ACC_WU_IA_DUP2_t WakeupStatus;

  /* Clear WU_IA event*/
  LIS2DS12_ACC_R_WU_IA_DUP2(0, &WakeupStatus);

  /* configure wakeup */
#ifdef TEST_WITH_FTH_INTERRUPT
  /* register callback */
  RegisterInterrupt(LIS2DS12_ACC_Wakeup_Callback);
#endif

  /* Set ACC ODR  HR_14bit 100Hz*/
  response = LIS2DS12_ACC_W_ODR(0, LIS2DS12_ACC_ODR_HR_14bit_100Hz);
  if(response==MEMS_ERROR) while(1); //manage here comunication error

  /* Set Wakeup Event */
  SetupWakeUpEvent(2, 0, 1);

#ifdef TEST_WITH_FTH_INTERRUPT
  while(1) {
    /* Event will be handled in driver callback */
  }
#else
  /*
   * Handle the event using polling mode
   */
  while(1) {
    LIS2DS12_ACC_R_WU_IA_DUP2(&WakeupStatus);
    if (WakeupStatus == LIS2DS12_ACC_WU_IA_DUP2_EV_ON) {
      /* handle event */
      WakeupStatus = LIS2DS12_ACC_WU_IA_DUP2_EV_OFF;
    }
  }
#endif
}

/*
 * Callback to handle the Wakeup event.
 * It must be registered to be called at interrupt time.
 */
static void LIS2DS12_ACC_Wakeup_Callback(u8_t intID)
{
  LIS2DS12_ACC_WU_IA_DUP2_t WakeupStatus;

  LIS2DS12_ACC_R_WU_IA_DUP2(0, &WakeupStatus);
  if (WakeupStatus == LIS2DS12_ACC_WU_IA_DUP2_EV_ON) {
    LIS2DS12_ACC_wakeup_ev_num++;

    /* handle event */
    WakeupStatus = LIS2DS12_ACC_WU_IA_DUP2_EV_OFF;
  }
}