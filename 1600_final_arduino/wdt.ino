/* Initialize the WDT peripheral */
void initWDT() {
  // Remember to use a 100% refresh window, unlike the prelab!
  R_WDT->WDTCR = 0b0011001110000010;

  // Enable WDT when debugger is connected
  R_DEBUG->DBGSTOPCR_b.DBGSTOP_WDT = 0;
  R_WDT->WDTSR = 0; // clear watchdog status;

  // Make the watchdog reset hardware
  R_WDT->WDTRCR = 0b10000000; // if bit 7 is 1 then resets the system, if 0 then runs wdtISR()
}

/* pet the watchdog */
void petWDT() {
  R_WDT->WDTRR = 0;
  R_WDT->WDTRR = 0xff;
}