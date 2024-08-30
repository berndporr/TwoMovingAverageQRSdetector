#include "two_avg_qrs_det.h"

#include <stdio.h>
#include "Iir.h"

struct MyCallback : TwoMovingAverageQRSdetector::QRSDetectionCallback {
    MyCallback(float samplingrate) {
	fs = samplingrate;
	f = fopen("hr.dat","wt");
	if (!f) {
	    fprintf(stderr,"Could not open hr.dat\n");
	    exit(1);
	}
    }
    ~MyCallback() {
	fclose(f);
    }
    virtual void twoMovingAverageQRSdetected(long int sampleNo) {
	if (previousQRSsampleNo != sampleNo) {
	    float hr = fs/(float)(sampleNo - previousQRSsampleNo)*60.0f;
	    printf("HR = %f\n",hr);
	    if (f) fprintf(f,"%f\n",hr);
	}
	previousQRSsampleNo = sampleNo;
    }
    FILE* f = NULL;
    float fs = 0;
    long int previousQRSsampleNo = 0;
};

int main (int,char**)
{
    const float fs = 250;
    const float mains = 50;
    const int filterorder = 2;
    
    Iir::Butterworth::BandStop<filterorder> iirnotch;
    iirnotch.setup(fs,mains,2);

    MyCallback callback(fs);

    TwoMovingAverageQRSdetector twoavg(fs);
    twoavg.registerCallback(&callback);

    FILE *finput = fopen("example_data/ECG.tsv","rt");
    for(;;) 
    {
	float a1,a2,a3,a4,a5,a6;
	if (fscanf(finput,"%f %f %f %f %f %f\n",&a1,&a2,&a3,&a4,&a5,&a6)<1) break;
	const float a = iirnotch.filter(a1);
	twoavg.detect(a);
    }
    fclose(finput);
}
