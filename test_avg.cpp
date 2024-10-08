// This is the only include you need
#include "two_avg_qrs_det.h"
#include "math.h"

#include <stdio.h>
#include <vector>
#include <assert.h>

void assertEqual(float a, float b) {
	if (fabs(a-b) < 1E-10) return;
	printf("BUG: %f != %f\n",a,b);
	exit(1);
}

int main (int,char**) {
    TwoMovingAverageQRSdetector::DelayLine fir;
	const int N = 8;
	fir.init(N);
	float acc = 0;
	for(int i = 0;i < 16;i++) {
		float v = 0;
		if ((i == 1) || (i > 10)) {
			v = 1;
		}
		// let's filter a delta pulse at time step 1 and
		// a unit step at time step 10
		v = fir.average(v);
		// now let's check if the filter has done the right ops
		// the delta pulse should reproduce the impulse respnse
		if ( (i < 9) && (i > 0) ) {
		    assertEqual(1.0f/(float)N,v);
		}
		// and the step response should accumulate it
		if (i > 10) {
		    acc = acc + 1.0f/(float)N;
			assertEqual(v,acc);
		}
	}
}
