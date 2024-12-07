/* Initialize the WDT peripheral */
void initWDT() {
  // Remember to use a 100% refresh window, unlike the prelab!
  R_WDT->WDTCR = 0b0011001110000010;

  // Enable WDT when debugger is connected
  R_DEBUG->DBGSTOPCR_b.DBGSTOP_WDT = 0;
  R_WDT->WDTSR = 0; // clear watchdog status;

  // Make the watchdog trigger an interrupt
  // and use the ICU to connect it to the CPU
  R_WDT->WDTRCR = 0b00000000; // if bit 7 is 1 then resets the system 
  R_ICU->IELSR[WDT_INT] = 0x025;

  NVIC_SetVector((IRQn_Type) WDT_INT, (uint32_t) &wdtISR); // set vector entry to our handler
  NVIC_SetPriority((IRQn_Type) WDT_INT, 13); // Priority lower than Serial (12)
  NVIC_EnableIRQ((IRQn_Type) WDT_INT);
}

/* pet the watchdog */
void petWDT() {
  R_WDT->WDTRR = 0;
  R_WDT->WDTRR = 0xff;
}

/* ISR when WDT triggers */
void wdtISR() {
  Serial.println("WATCHDOG"); // python code reads this and exits the game
}