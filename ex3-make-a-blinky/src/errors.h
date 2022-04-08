#ifndef MES_ERRORS_H__

typedef enum {
   errOk,
   errButtonNotReady,
   errButtonNotConfigured,
   errButtonInterruptNotConfigured,
   errLEDNotReady,
   errLEDNotConfigured 
} eBlinkyError_t;

#define MES_ERRORS_H__
#endif
