//        Elgendi, Mohamed & Jonkman, 
//        Mirjam & De Boer, Friso. (2010).
//        Frequency Bands Effects on QRS Detection.
//        The 3rd International Conference on Bio-inspired Systems 
//        and Signal Processing (BIOSIGNALS2010). 428-431.

class TwoAverageQRSdetector {

    const double f_highpass = 8;
    const double f_lowpass = 20;
    const double order = 4;
    
    Iir::Butterworth::LowPass<order> lowpass;
    Iir::Butterworth::HighPass<order> highpass;

// Fast implementation of moving window average with numpy's cumsum function 
    class MWA_cumulative(window_size) {
    
    ret = np.cumsum(input_array, dtype=float)
    ret[window_size:] = ret[window_size:] - ret[:-window_size]
    
    for i in range(1,window_size):
        ret[i-1] = ret[i-1] / i
    ret[window_size - 1:]  = ret[window_size - 1:] / window_size
    
    return ret

    void init(double fs) {
	lowpass.setup(fs, f_lowpass);
	highpass.setup(fs, f_highpass);	
        int window1 = int(0.12*self.fs)
        int window2 = int(0.6*self.fs)
    }

    void two_average_detector(double unfiltered_ecg) {
        
        double filtered_ecg = lowpass.filter(highpass(unfiltered_ecg));

        mwa_qrs = MWA_from_name(MWA_name)(abs(filtered_ecg), window1)

        mwa_beat = MWA_from_name(MWA_name)(abs(filtered_ecg), window2)

        blocks = np.zeros(len(unfiltered_ecg))
        block_height = np.max(filtered_ecg)

        for i in range(len(mwa_qrs)):
            if mwa_qrs[i] > mwa_beat[i]:
                blocks[i] = block_height
            else:
                blocks[i] = 0

        QRS = []

        for i in range(1, len(blocks)):
            if blocks[i-1] == 0 and blocks[i] == block_height:
                start = i
            
            elif blocks[i-1] == block_height and blocks[i] == 0:
                end = i-1

                if end-start>int(0.08*self.fs):
                    detection = np.argmax(filtered_ecg[start:end+1])+start
                    if QRS:
                        if detection-QRS[-1]>int(0.3*self.fs):
                            QRS.append(detection)
                    else:
                        QRS.append(detection)

        return QRS

