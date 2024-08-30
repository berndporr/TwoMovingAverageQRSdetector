#include "two_avg_qrs_det.h"

#include <stdio.h>
#include "Iir.h"

struct MyCallback : HRCallback {
    FILE* f;
    long int n = 0;
    float fs = 0;
    long int previousQRSsampleNo = 0;
    MyCallback(FILE* hrFile, float samplingrate) {
	f = hrFile;
	fs = samplingrate;
    }
    virtual void hasQRS(long int sampleNo) {
	print("Pulse!\n");
	if (previousQRSsampleNo != sampleNo) {
	    float hr = fs/(sampleNo - previousQRSsampleNo)*60;
	    printf("HR = %f\n",hr);
	    fprintf(f,"%f\n",hr);
	}
	previousQRSsampleNo = sampleNo;
    }
};

int main (int,char**)
{
    const float fs = 250;
    const float mains = 50;
    Iir::Butterworth::BandStop<2> iirnotch;
    iirnotch.setup(fs,mains,2);

    FILE* f = fopen("hr.dat","wt");
    if (!f) {
	fprintf(stderr,"Could not open hr.dat\n");
	exit(1);
    }
    
    MyCallback callback(f,fs);

    TwoMovingAverageQRSdetector twoavg(fs);
    twoavg.registerCallback(callback);

    FILE *finput = fopen("example_data/ECG.tsv","rt");
    for(;;) 
    {
	float a1,a2,a3,a4,a5,a6;
	if (fscanf(finput,"%f %f %f %f %f %f\n",&a1,&a2,&a3,&a4,&a5,&a6)<1) break;
	const float a = iirnotch.filter(a1);
	twoavg.detect(a);
    }
    fclose(finput);
    
    fclose(f);
}
