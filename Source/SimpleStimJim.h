/*

Simple StimJim plugin by:
Marin Manuel <marin.manuel@neurobio.org>
University of Rhode Island

 Simple StimJim is a simplified version of the controller for the StimJim stimulation
 instead of being able to generate fully configurable trains, in Simple StimJim
 there is a direct triggering of a train with zero delay, and the user can only control
 the number and frequency of the pulses in the train.

*/
#pragma once

#define STIMJIM_NB_OUT_CHANNELS 2
#define STIMJIM_MAX_VOLTS_MV 14500  // I have noticed an integer overflow if trying to get all the way to 15V
#define STIMJIM_MAX_CURRENT_UA 3300
#define STIMJIM_MAX_PERIOD_US 4294967295
#define STIMJIM_MAX_DURATION_US 4294967295
#include <stdexcept>


enum class StimMode {
    Voltage = 0, Current = 1, Disconnected = 2, Grounded = 3
};

class OutputChannel {
private:
    int channelId;
    bool status;
    int pulseDuration; // in microseconds
    bool isBipolar;
    int numberOfPulses;
    float trainFrequency; // positive float
    StimMode stimMode;
    float amplitude; // in mV or uA
    bool isNormalized;
    float thresholdValue; // in mV or uA 

    void setDefaultParameters();

public:
    // Constructor
    explicit OutputChannel(int id);

    /***************************************************************
     * GETTERS
     ***************************************************************/
    int getChannelId() const;

    int getPulseDuration() const;

    bool getIsBipolar() const;

    int getNumberOfPulses() const;

    float getTrainFrequency() const;

    StimMode getStimMode() const;

    float getAmplitude() const;

    bool getIsNormalized() const;

    float getThresholdValue() const;

    /***************************************************************
     * SETTERS
     ***************************************************************/

    /**
     * Sets the pulse duration for the output channel.
     *
     * This function sets the pulse duration for the output channel to the specified value.
     * If the provided duration is outside the valid range (0 to STIMJIM_MAX_DURATION_US),
     * it will be constrained to the nearest valid boundary.
     *
     * @param duration The desired pulse duration in microseconds.
     */
    void setPulseDuration(unsigned long duration);

    /**
     * Sets whether the pulse will be bipolar or not.
     *
     * If the pulse is monopolar, then it will be the duration and amplitude set
     * by setAmplitude() and setDuration(). If the pulse is bipolar, then the pulse
     * will got to `amplitude` for half the duration, and then to `-amplitude` for the
     * remaining half duration.
     *
     * @param bipolar set to true for bipolar pulses.
     */
    void setIsBipolar(bool bipolar);

    /**
     * Sets the number of pulses in the train
     *
     * @param pulses number of pulses between 1 and XXXXXX #FIXME
     */
    void setNumberOfPulses(int pulses);

    /**
     * Set the train frequency (in Hz)
     *
     * @param frequency frequency in Hz
     */
    void setTrainFrequency(float frequency);

    /**
     * Set the output mode of the stimulator.
     *
     * Possible modes are Voltage, Current, Disconnected and Grounded
     *
     * @param mode the output mode
     */
    void setStimMode(StimMode mode);

    /**
     * Sets the amplitude of the stimulation
     *
     * Amplitude can be negative or positive. Note that in bipolar mode
     * the actual amplitude of the output pulse will be +amplitude followed by -amplitude
     * Units will depend on the output mode:
     *  - in VOLTAGE mode, amplitude is set in mV
     *  - in CURRENT mode, amplitude is set in uA
     *  - in DISCONNECTED and GROUNDED mode, this function has no effect
     * Note that if `isNormalized` is true, then `amplitude` represents the normalized amplitude,
     * and the actual output amplitude will be given by output = amplitude * threshold value
     *
     * @param amp the amplitude of the pulse. Value will be constrained to the maximum amplitude
     * attainable in the given output mode
     */
    void setAmplitude(float amp);

    /**
     * Sets whether the amplitude is normalized by the threshold value
     * This function will actually perform the normalization. After the function, the threshold value will be set
     * to `threshold` and `amplitude` will be set to 1.0 (xT)
     * When `setIsNormalized` is set to false, amplitude will be set to normalized amplitude * threshold
     *
     * @param normalized set to true to convert the amplitude to the normalized amplitude
     * @param threshold the threshold value (units depends on the output mode, see `setAmplitude`)
     */
    void setIsNormalized(bool normalized, float threshold);

    /**
     * Sets the threshold value that is used to normalize the amplitude in normalized mode
     * Note that this function only sets the threshold, it does not normalize the amplitude
     * @param threshold threshold value (units depends on the output mode, see `setAmplitude`)
     */
    void setThresholdValue(float threshold);
};

class SimpleStimJim {
private:
    OutputChannel channels[STIMJIM_NB_OUT_CHANNELS];

public:
    // Constructor
    SimpleStimJim();

    // Accessor to get OutputChannel by index
    OutputChannel& operator[](size_t index) {
        if (index >= STIMJIM_NB_OUT_CHANNELS) {
            // Handle out-of-range index appropriately, like throwing an exception or returning a default value
            throw std::out_of_range("Index out of range");
        }
        return channels[index];
    }
};