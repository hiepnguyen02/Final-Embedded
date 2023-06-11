#include <stm32f10x.h>

#define FIR_FILTER_LENGTH 10
#define BOOL int8_t
#define TRUE 1
#define FALSE 0

typedef struct {
    double buff[FIR_FILTER_LENGTH]; // for circular buffer array
    uint8_t buffIndex; 							// Tracking the index of the circular buffer
    double out; 										// the output value of the circular buffer
} FIRFilter;

// function prototypes
void FIRFilter_init(FIRFilter *fir);
double FIRFilter_calc(FIRFilter *fir, double inputVal);
BOOL checkStep(FIRFilter *fir, double magnitude);
