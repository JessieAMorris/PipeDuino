//Direct pin read/write procedures for high speed I/O

inline void digitalWriteDirect(int pin, boolean val) {
  if (val) g_APinDescription[pin].pPort->PIO_SODR = g_APinDescription[pin].ulPin;
  else    g_APinDescription[pin].pPort->PIO_CODR = g_APinDescription[pin].ulPin;
}

inline int digitalReadDirect(int pin) {
  return !!(g_APinDescription[pin].pPort->PIO_PDSR & g_APinDescription[pin].ulPin);
}
