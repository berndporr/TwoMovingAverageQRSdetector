#ifndef TWO_AVG_QRS_DET_H
#define TWO_AVG_QRS_DET_H

#include <stdio.h>
#include <vector>
#include <deque>
#include <algorithm>
#include <numeric>
#include <Iir.h>

class DelayLine {
public:
    
    void init(const int length) {
        buffer = std::deque<float>();
        buffer.resize(length);
    }

    inline float average(const float input) {
        buffer.push_front(input);
        buffer.pop_back();
        float a = 0.0;
        for(auto &v:buffer) {
            a += v;
        }
        return a / (float)(buffer.size());
    }

private:
    std::deque<float> buffer;

};

/**
 * @brief Heartrate detector callback interface
 * This interface establishes the communication between
 * the main program and the detector. The main program
 * needs to create an instance of this interface
 * and register it with the Engzee class.
 */
struct HRCallback {
    virtual void hasQRS(long int) = 0;
};

/**
 * @brief The EngZee heartrate detector class
 * This class performs continous heartrate detection on a stream of
 * ECG data.
 */
class TwoMovingAverageQRSdetector {
public:
    
/**
 * @brief Construct a new Engzee object
 * 
 * @param samplingrate The sampling rate in Hz in the region of 250Hz..300Hz.
 * @param callback An instancce of the HRCallback class
 */
    TwoMovingAverageQRSdetector(float samplingrate) {
        fs = samplingrate;
	lowpass.setup(fs, f_lowpass);
	highpass.setup(fs, f_highpass);
        window1.init((int)(0.12*fs));
	window2.init((int)(0.6*fs));
	thres_within_roi = (int)(0.08*fs);
	ignore_duration = (int)(0.3*fs);
	ignore_ctr = (int)fs;
    }

    void registerCallback(HRCallback* cb) {
	hrCallbacks.push_back(cb);
    }
    
    void detect(float unfiltered_ecg) {
	sample_ctr++;
	float filtered_ecg = lowpass.filter(highpass.filter(unfiltered_ecg));
	float mwa_qrs = window1.average(abs(filtered_ecg));
        float mwa_beat = window2.average(abs(filtered_ecg));
	if (ignore_ctr > 0) {
	    ignore_ctr--;
	    within_roi_ctr = 0;
	    return;
	}
	if (mwa_qrs > mwa_beat) {
	    within_roi_ctr++;
	    if (within_roi_ctr > thres_within_roi) {
		for(auto& cb: hrCallbacks) {
		    cb->hasQRS(sample_ctr);
		    ignore_ctr = ignore_duration;
		}
	    }
	} else {
	    within_roi_ctr = 0;
	}
    }
    
private:
    
    static constexpr double f_highpass = 8;
    static constexpr double f_lowpass = 20;
    static constexpr int order = 4;
    
    Iir::Butterworth::LowPass<order> lowpass;
    Iir::Butterworth::HighPass<order> highpass;
    
    DelayLine window1;
    DelayLine window2;

    int within_roi_ctr = 0;
    int thres_within_roi = 0;

    int ignore_ctr = 0;
    int ignore_duration = 0;

    long int sample_ctr = -1;

    std::vector<HRCallback*> hrCallbacks;

    float fs;
};

#endif
