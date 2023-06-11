#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "FIRFilter.h"

static double FIR_FILTER_IMPULSE_RESPONSE[FIR_FILTER_LENGTH] = {0.0116, 0.0269, 0.0690, 0.1248, 0.1723, 0.1909, 0.1723, 0.1248, 0.0690, 0.0269};

void FIRFilter_init(FIRFilter *fir) {
    for (int i = 0; i < FIR_FILTER_LENGTH; i++) {
        fir->buff[i] = 1.0f;
    }
    fir->buffIndex = 0;
    fir->out = 0.0f;
}

double FIRFilter_calc(FIRFilter *fir, double inputVal) { 
    fir->buff[fir->buffIndex] = inputVal;

    fir->buffIndex++;
    if (fir->buffIndex == FIR_FILTER_LENGTH) {
        fir->buffIndex = 0;
    }
		uint8_t sumIndex = fir->buffIndex;

    fir->out = 0.0f;
    for (int i = 0; i < FIR_FILTER_LENGTH; i++) {
        if (sumIndex > 0) {
            sumIndex--;
        } else {
            sumIndex = FIR_FILTER_LENGTH - 1;
        }
        fir->out += fir->buff[sumIndex] * FIR_FILTER_IMPULSE_RESPONSE[i];
    }

    return fir->out;
}

BOOL checkStep(FIRFilter *fir, double magnitude) {
		static double output_buf[5] = { 1.0, 1.0, 1.0, 1.0, 1.0};
    FIRFilter_calc(fir, magnitude);
		for (int i = 0; i <= 3; i++) {
				output_buf[i] = output_buf[i+1];
		}
		output_buf[4] = fir->out;
    if (output_buf[0] < output_buf[1] && output_buf[1] < output_buf[2] && output_buf[2] > output_buf[3] && output_buf[3] > output_buf[4] && fir->out > 1.1) {       	
            return TRUE;
		}
    return FALSE;
}
