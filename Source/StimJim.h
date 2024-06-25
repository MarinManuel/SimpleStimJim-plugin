#pragma once

#define STIMJIM_NB_OUTPUT_CHANNELS 2
#define STIMJIM_NB_TRIGGER_CHANNELS 2
#define STIMJIM_MAX_NB_STAGES 10
#define STIMJIM_MAX_NB_TRAINS 100

enum class StimMode {
    Voltage = 0, Current = 1, Disconnected = 2, Grounded = 3
};

enum class TriggerDirection {
    Rising = 0, Falling = 1
};

struct PulseStage {
    float amplitudes[STIMJIM_NB_OUTPUT_CHANNELS] = {}; // either in mV or uA depending on output mode
    float duration_us = 0; // in us
};

class PulseTrain {
private:
    int pulse_train_id;
    StimMode output_modes[STIMJIM_NB_OUTPUT_CHANNELS];
    float period;
    float train_duration;
    int nb_stages;
    PulseStage stages[STIMJIM_MAX_NB_STAGES];

public:
    PulseTrain(int id, StimMode modes[], float period, float duration, int nb_stages, PulseStage stages[]);

    int getPulseTrainId() const;
    const StimMode *getOutputModes() const;
    float getPeriod() const;
    float getTrainDuration() const;
    int getNbStages() const;
    PulseStage getStage(int stage_id) const;
    const PulseStage *getStages() const;

    void setStimModes(StimMode modes[]);
    void setTrainDuration(float trainDuration);
    void setPeriod(float period);
    void setFrequency(float frequency);
    void setDuration(float duration);
    void setNbStages(int nbStages);
    void setStages(PulseStage stages[]);
    void setStage(int stage_id, PulseStage stage);
};

struct Trigger {
    bool enabled;
    int source;
    int target;
    TriggerDirection direction;
};

class StimJim {
private:
    PulseTrain pulse_trains[STIMJIM_MAX_NB_TRAINS];
    Trigger triggers[STIMJIM_NB_TRIGGER_CHANNELS];
public:
    StimJim();
    void create_pulsetrain(PulseTrain pulseTrain);
    void create_pulsetrain(int id, StimMode modes[], float period, float duration, PulseStage stages[]);
    void enable_trigger(int trigger_id, int source, int target, TriggerDirection direction=TriggerDirection::Rising);
    void disable_trigger(int trigger_id);
    void trigger(int trigger_id);
};