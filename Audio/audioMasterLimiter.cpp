#include "audioMasterLimiter.h"
#include <algorithm>
#include <cmath>
#include <cstdio>

MasterLimiter::MasterLimiter(float lookAheadMs, float sampleRate) {
	bufferSize = std::max(1, static_cast<int>(lookAheadMs * sampleRate / 1000.0f));
	delayL.assign(bufferSize, 0.0f);
	delayR.assign(bufferSize, 0.0f);
}

void MasterLimiter::setParameters(float thresholdDb, float ceilingDb, float releaseMs, float sampleRate) {
	targetThreshold = std::pow(10.0f, thresholdDb / 20.0f);
	targetCeiling = std::pow(10.0f, ceilingDb / 20.0f);
	targetRelease = std::exp(-1.0f / (releaseMs * sampleRate / 1000.0f));
}

float MasterLimiter::calculateTruePeak(float input, float* fifo) {
	static const float c[4][4] = {
		{ 0.000f, 1.000f, 0.000f, 0.000f }, 
		{ -0.125f, 0.875f, 0.375f, -0.125f },
		{ -0.250f, 0.750f, 0.750f, -0.250f },
		{ -0.125f, 0.375f, 0.875f, -0.125f }
	};
	fifo[0] = fifo[1]; fifo[1] = fifo[2]; fifo[2] = fifo[3]; fifo[3] = input;
	float maxP = 0.0f;
	for (int p = 0; p < 4; ++p) {
		float s = (fifo[0]*c[p][0]) + (fifo[1]*c[p][1]) + (fifo[2]*c[p][2]) + (fifo[3]*c[p][3]);
		maxP = std::max(maxP, std::fabs(s));
	}
	return maxP;
}

void MasterLimiter::processBlock(float* bufferL, float* bufferR, int numSamples) {
	// 1. Setup Ramping Steps
	float threshStep = (targetThreshold - activeThreshold) / numSamples;
	float ceilingStep = (targetCeiling - activeCeiling) / numSamples;
	float releaseStep = (targetRelease - activeRelease) / numSamples;
    
	float targetMakeup = useAutoGain.load() ? (activeCeiling / targetThreshold) : activeCeiling;
	float makeupStep = (targetMakeup - activeAutoMakeup) / numSamples;
    
	float blockMaxGR = 0.0f;
	
	float peak = 0.0f;
	float filtered = 0.0f;

	for (int i = 0; i < numSamples; ++i) {
		activeThreshold += threshStep;
		activeCeiling += ceilingStep;
		activeRelease += releaseStep;
		activeAutoMakeup += makeupStep;

		// 2. Detection (Always run for metering/sidechain continuity)
		float tpL = (bufferL) ? calculateTruePeak(bufferL[i], fifoL) : 0.0f;
		float tpR = (bufferR) ? calculateTruePeak(bufferR[i], fifoR) : 0.0f;
		peak = std::max(tpL, tpR);

		// 3. Sidechain Filter
		filtered = peak - scHPF_State;
		scHPF_State = (filtered * scAlpha) + scHPF_State;

		// 4. Gain Calculation
		float instant = (filtered > activeThreshold) ? (activeThreshold / filtered) : 1.0f;

		// Debug
		static float maxFiltered = 0.0f;
		maxFiltered = std::max(maxFiltered, filtered);

		//static int loopDebug = 0;
		//if (++loopDebug % 4800 == 0)
		//{ // Every ~100ms at 48kHz
		//	fprintf(stderr, "Peak: %.4f | scHPF_State: %.4f | Filtered: %.4f | Max Filtered: %.4f\n",
		//			peak, scHPF_State, filtered, maxFiltered);
		//	fprintf(stderr, "Threshold: %.4f | Instant: %.4f\n", activeThreshold, instant);
		//	maxFiltered = 0.0f;
		//}

		if (isBypassed.load()) {
			instant = 1.0f; peakHoldGain = 1.0f; holdCounter = 0;
		}
		else {
			if (instant < peakHoldGain) { peakHoldGain = instant; holdCounter = bufferSize; }
			else {
				if (holdCounter > 0) holdCounter--; 
				else peakHoldGain = (activeRelease * peakHoldGain) + (1.0f - activeRelease);
			}
		}

		currentGain = (smoothCoeff * currentGain) + ((1.0f - smoothCoeff) * peakHoldGain);
        
		// 5. Apply Output (Makeup * GR)
		float finalG = currentGain * (isBypassed.load() ? 1.0f : activeAutoMakeup);

		if (bufferL) { float out = delayL[writeIdx] * finalG; delayL[writeIdx] = bufferL[i]; bufferL[i] = out; }
		if (bufferR) { float out = delayR[writeIdx] * finalG; delayR[writeIdx] = bufferR[i]; bufferR[i] = out; }

		writeIdx = (writeIdx + 1) % bufferSize;

		// 6. Metering
		float gr = -20.0f * std::log10(currentGain + 1e-9f);
		blockMaxGR = std::max(blockMaxGR, gr);
	}
    
	float m = visualGR.load();
	visualGR.store(blockMaxGR > m ? blockMaxGR : m * meterRelease);

	// Live monitoring 
	//static int debugCounter = 0;
	//if (++debugCounter % 100 == 0)
	//{ // Log every 100 blocks
	//	fprintf(stderr, "GR: %.2f dB | Peak: %.2f dB | Gain: %.4f\n",
	//			visualGR.load(), blockMaxGR, currentGain);
	//}

	// Enhanced live monitoring
	//static int debugCounter = 0;
	//if (++debugCounter % 100 == 0)
	//{
	//	fprintf(stderr, "=== LIMITER DEBUG ===\n");
	//	fprintf(stderr, "Bypassed: %s\n", isBypassed.load() ? "YES" : "NO");
	//	fprintf(stderr, "Threshold: %.4f (%.2f dB)\n", activeThreshold,
	//			20.0f * std::log10(activeThreshold + 1e-9f));
	//	fprintf(stderr, "Ceiling: %.4f (%.2f dB)\n", activeCeiling,
	//			20.0f * std::log10(activeCeiling + 1e-9f));
	//	fprintf(stderr, "Peak: %.4f | Filtered: %.4f\n", peak, filtered);
	//	fprintf(stderr, "Current Gain: %.4f | GR: %.2f dB\n",
	//			currentGain, visualGR.load());
	//	fprintf(stderr, "Auto Makeup: %.4f\n", activeAutoMakeup);
	//	fprintf(stderr, "====================\n");
	//}
}

float MasterLimiter::getGainReductiondB() const { return visualGR.load(); }
