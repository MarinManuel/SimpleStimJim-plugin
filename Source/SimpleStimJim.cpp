#include "SimpleStimJim.h"

template<typename T>
T constrain(const T &value, const T &min, const T &max) {
    return (value < min) ? min : (value > max) ? max : value;
}

void OutputChannel::setDefaultParameters() {
    setAmplitude(0.0);
    setStimMode(StimMode::Voltage);
    setIsBipolar(false);
    setNumberOfPulses(1);
    setTrainFrequency(200.0);
    setIsNormalized(false, 1.0);
    setThresholdValue(0.0);
}

OutputChannel::OutputChannel(int id) {
    this->channelId = id;
    this->setDefaultParameters();
}

int OutputChannel::getChannelId() const {
    return this->channelId;
}

int OutputChannel::getPulseDuration() const {
    return this->pulseDuration;
}

bool OutputChannel::getIsBipolar() const {
    return this->isBipolar;
}

int OutputChannel::getNumberOfPulses() const {
    return this->numberOfPulses;
}

float OutputChannel::getTrainFrequency() const {
    return this->trainFrequency;
}

StimMode OutputChannel::getStimMode() const {
    return this->stimMode;
}

float OutputChannel::getAmplitude() const {
    return this->amplitude;
}

bool OutputChannel::getIsNormalized() const {
    return this->isNormalized;
}

float OutputChannel::getThresholdValue() const {
    return this->thresholdValue;
}

void OutputChannel::setPulseDuration(unsigned long duration) {
    this->pulseDuration = constrain(duration, 0, STIMJIM_MAX_DURATION_US);
}

void OutputChannel::setIsBipolar(bool bipolar) {
    this->isBipolar = bipolar;

}

void OutputChannel::setNumberOfPulses(int pulses) {
    this->numberOfPulses = pulses;

}

void OutputChannel::setTrainFrequency(float frequency) {
    this->trainFrequency = frequency;

}

void OutputChannel::setStimMode(StimMode mode) {
    this->stimMode = mode;

}

void OutputChannel::setAmplitude(float amp) {
    if (!this->isNormalized) {
        if (this->stimMode == StimMode::Voltage) {
            this->amplitude = constrain(amp, (float) -1 * STIMJIM_MAX_VOLTS_MV, (float) STIMJIM_MAX_VOLTS_MV);
        } else if (this->stimMode == StimMode::Current) {
            this->amplitude = constrain(amp, (float) -1 * STIMJIM_MAX_CURRENT_UA, (float) STIMJIM_MAX_CURRENT_UA);
        } else {
            this->amplitude = 0.0;  // in the other modes, there is no output, so this value should not matter.
            // since in those cases, we cannot constrain the value, I think it's better to
            // set the value to zero, otherwise it could cause unintended consequences when the user
            // switches back to voltage or current
        }
    } else {
        // normalized mode
        this->amplitude = amp; // in normalized mode, the actual amplitude will be the normalized amplitude * threshold value
        // constrain will need to be performed when sending the command to StimJim.
    }
}

void OutputChannel::setThresholdValue(float threshold) {
    if (this->stimMode == StimMode::Voltage) {
        this->thresholdValue = constrain(threshold, (float) -1 * STIMJIM_MAX_VOLTS_MV, (float) STIMJIM_MAX_VOLTS_MV);
    } else if (this->stimMode == StimMode::Current) {
        this->thresholdValue = constrain(threshold, (float) -1 * STIMJIM_MAX_CURRENT_UA,
                                         (float) STIMJIM_MAX_CURRENT_UA);
    } else {
        this->thresholdValue = threshold; // cannot constrain the value in the other modes, but it could be usefull
        // to let the user set the threshold value in grounded mode, before switching
        // back to the actual stimulation mode
    }
}

void OutputChannel::setIsNormalized(bool normalized, float threshold) {
    if (this->isNormalized) {
        // output is currently normalized
        if (!normalized) { // we are turning OFF normalization
            this->isNormalized = normalized;
            this->setAmplitude(this->amplitude * this->thresholdValue);
            this->thresholdValue = threshold;
        } else { // normalization is left ON
            this->setThresholdValue(threshold);
        }
    } else {
        // output is currently not normalized
        if (normalized) {// we are turning normalization ON
            this->setThresholdValue(threshold);
            this->setAmplitude(1.0);
            this->isNormalized = normalized;
        } else { // we leave normalization off
            this->setThresholdValue(threshold);
        }
    }
}

