# Two Moving Average QRS detector

C++ implementation similar to: 
        Elgendi, Mohamed & Jonkman, 
        Mirjam & De Boer, Friso. (2010).
        Frequency Bands Effects on QRS Detection.
        The 3rd International Conference on Bio-inspired Systems 
        and Signal Processing (BIOSIGNALS2010). 428-431.

In contrast to the orig Elgendi it looks for the moment
when the fast moving average filter becomes larger than
the slow moving average filter. The originnal Elgendi
detector then searches for the peak of the QRS complex
but for heartrate detection only timestamp differences
are relevant. Looking for true peaks is prone to errors
while just looking for essentially zero crossing is very
robust against noise.

## Usage

The detector is header-only:
```
include "two_avg_qrs_det.h"
```

Create a QRS callback:

```
struct MyCallback : TwoMovingAverageQRSdetector::QRSDetectionCallback {
        virtual void twoMovingAverageQRSdetected(long int sampleNo) {
		// do something with the detection
        }
};

```

Create an instance of the detector:

```
TwoMovingAverageQRSdetector twoavg(fs);
```
with `fs` being the sampling rate.


Register the callback:

```
MyCallback callback(fs);
twoavg.registerCallback(&callback);
```

And then in your realtime application feed the ECG sample by sample into the detector:

```
twoavg.detect(ecg_sample);
```

The callback is then called whenever there is a QRS detected.


## Demo

Install the IIR filter library (https://github.com/berndporr/iir1)
which is used to remove the mains interference.

Run:
```
cmake .
make
./demo
```
and it will print the heartrate from an example ECG and
save it to `hr.dat` which can be plotted for example
gnuplot or the script `plot_hr.py`.

If you have the python detector installed then you can
compare its output with that from this detector:
`compare_cpp_and_python_detector.py`

![alt tag](compare_cpp_py.png)

# Credit

Bernd Porr
