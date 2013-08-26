#include "WaveTableNode.h"

/*
    ================
    GPNODE OVERRIDES
    ================
*/

void WaveTableNode::setRenderInfo(float sr, unsigned blockSize, unsigned maxFrameNumber, float maxTime) {
	sampleRate = sr;
	osc = new WaveTableOsc();
	makeAddAllWaveTables((double) sr, 2, 99999, 20.0f, (double) 20.0f * 2.0 / sampleRate);
	//makeAddAllWaveTables((double) sr, 2, 99999, 20.0f, (double) sr/2);
	GPNode::setRenderInfo(sr, blockSize, maxFrameNumber, maxTime);
}

void WaveTableNode::doneRendering() {
	if (preparedToRender) {
		delete osc;
	}
	GPNode::doneRendering();
}

void WaveTableNode::evaluateBlockPerformance(unsigned firstFrameNumber, unsigned numSamples, float* sampleTimes, unsigned numConstantVariables, float* constantVariables, float* buffer) {
	// use unused variables
	sampleTimes;
	numConstantVariables;
	constantVariables;
	
	// check if first frame number
	if (firstFrameNumber == 0) {
		// reset oscillator
	}
	
	// fill the audio buffer
	float freqVal = (constantVariables[variableNum] * partial) / sampleRate;
    for (unsigned i = 0; i < numSamples; i++) {
        osc->setFrequency(freqVal);
        buffer[i] = osc->getOutput();
        osc->updatePhase(); 
    }
}

void WaveTableNode::updateMutatedParams() {
    GPNode::updateMutatedParams();
}

/*
    =================
    WAVETABLE HELPERS
    =================
*/

void WaveTableNode::fft(int N, double *ar, double *ai)
{    
    int i, j, k, L;            /* indexes */
    int M, TEMP, LE, LE1, ip;  /* M = log N */
    int NV2, NM1;
    double t;               /* temp */
    double Ur, Ui, Wr, Wi, Tr, Ti;
    double Ur_old;
    
    // if ((N > 1) && !(N & (N - 1)))   // make sure we have a power of 2
    
    NV2 = N >> 1;
    NM1 = N - 1;
    TEMP = N; /* get M = log N */
    M = 0;
    while (TEMP >>= 1) ++M;
    
    /* shuffle */
    j = 1;
    for (i = 1; i <= NM1; i++) {
        if(i<j) {             /* swap a[i] and a[j] */
            t = ar[j-1];     
            ar[j-1] = ar[i-1];
            ar[i-1] = t;
            t = ai[j-1];
            ai[j-1] = ai[i-1];
            ai[i-1] = t;
        }
        
        k = NV2;             /* bit-reversed counter */
        while(k < j) {
            j -= k;
            k /= 2;
        }
        
        j += k;
    }
    
    LE = 1.;
    for (L = 1; L <= M; L++) {            // stage L
        LE1 = LE;                         // (LE1 = LE/2) 
        LE *= 2;                          // (LE = 2^L)
        Ur = 1.0;
        Ui = 0.; 
        Wr = cos(M_PI/(float)LE1);
        Wi = -sin(M_PI/(float)LE1); // Cooley, Lewis, and Welch have "+" here
        for (j = 1; j <= LE1; j++) {
            for (i = j; i <= N; i += LE) { // butterfly
                ip = i+LE1;
                Tr = ar[ip-1] * Ur - ai[ip-1] * Ui;
                Ti = ar[ip-1] * Ui + ai[ip-1] * Ur;
                ar[ip-1] = ar[i-1] - Tr;
                ai[ip-1] = ai[i-1] - Ti;
                ar[i-1]  = ar[i-1] + Tr;
                ai[i-1]  = ai[i-1] + Ti;
            }
            Ur_old = Ur;
            Ur = Ur_old * Wr - Ui * Wi;
            Ui = Ur_old * Wi + Ui * Wr;
        }
    }
}

float WaveTableNode::makeAddWaveTable(int len, double* ar, double* ai, double scale, double topFreq) {
    fft(len, ar, ai);
    
    if (scale == 0.0) {
        // calc normal
        double max = 0;
        for (int idx = 0; idx < len; idx++) {
            double temp = fabs(ai[idx]);
            if (max < temp)
                max = temp;
        }
        scale = 1.0 / max * .999;        
    }
    
    // normalize
    float* wave = (float*) malloc(sizeof(float) * len);
    for (int idx = 0; idx < len; idx++)
        wave[idx] = ai[idx] * scale;
        
    if (osc->addWaveTable(len, wave, topFreq))
        scale = 0.0;

    free(wave);

    return scale;
}
