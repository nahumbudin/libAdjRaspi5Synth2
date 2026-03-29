/*******************************************************************************
 * File: MasterLimiter.h
 * Description: Professional Look-Ahead Limiter with True Peak, Auto-Gain, 
 *              and Latency-Safe Bypass.
 *              
 * Mono Mode: You can use a single instance to process a mono track by passing (monoBuffer, nullptr, 512).
 * Dual Mono Setup: As mentioned previously, you can create two instances and pass (leftBuffer, nullptr) 
 * to the first and (nullptr, rightBuffer) to the second. This prevents the channels from interacting at all.
 ******************************************************************************/

#pragma once
#include <vector>
#include <atomic>

class MasterLimiter {
public:
	MasterLimiter(float lookAheadMs, float sampleRate);
	~MasterLimiter() = default;

	// Updates parameters from UI
	void setParameters(float thresholdDb, float ceilingDb, float releaseMs, float sampleRate);
    
	// Toggles
	void setBypass(bool shouldBypass) { isBypassed.store(shouldBypass); }
	void setAutoGain(bool enabled)    { useAutoGain.store(enabled); }

	// Main Process (Supports nullptr for mono processing)
	void processBlock(float* bufferL, float* bufferR, int numSamples);

	// Getters
	float getGainReductiondB() const;
	int getLatencySamples() const { return bufferSize; }

private:
	// Buffers
	std::vector<float> delayL, delayR;
	int bufferSize;
	int writeIdx = 0;

	// State
	float currentGain = 1.0f;
	float peakHoldGain = 1.0f;
	int holdCounter = 0;
	float scHPF_State = 0.0f;
	std::atomic<float> visualGR { 0.0f }
	;
	std::atomic<bool> isBypassed { false }
	;
	std::atomic<bool> useAutoGain { false }
	;

	// True Peak FIR State
	float fifoL[4] = { 0.0f };
	float fifoR[4] = { 0.0f };

	// Ramping Parameters
	float targetThreshold = 0.25f;
	float activeThreshold = 0.25f;
	float targetCeiling = 0.95f;
	float activeCeiling = 0.95f;
	float targetRelease = 0.999f;
	float activeRelease = 0.999f;
	float activeAutoMakeup = 1.0f;

	// Constants
	const float scAlpha = 0.98f;    
	const float smoothCoeff = 0.9f; 
	const float meterRelease = 0.995f;

	float calculateTruePeak(float inputSample, float* fifo);
};
