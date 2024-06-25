#include "StimJim.h"

#include <algorithm>

PulseTrain::PulseTrain(int id, StimMode *modes, float period, float duration, int nb_stages, PulseStage *stages) {
    this->pulse_train_id = id;
    for (int i = 0; i < STIMJIM_NB_OUTPUT_CHANNELS; i++) {
        this->output_modes[i] = modes[i];
    }
    this->period = period;
    this->train_duration = duration;
    this->nb_stages = nb_stages;
    for (int i = 0; i < std::min(STIMJIM_MAX_NB_STAGES, this->nb_stages); i++) {
        this->stages[i] = stages[i];
    }
}

int PulseTrain::getPulseTrainId() const {
    return this->pulse_train_id;
}

const StimMode *PulseTrain::getOutputModes() const {
    return this->output_modes;
}

float PulseTrain::getPeriod() const {
    return this->period;
}

float PulseTrain::getTrainDuration() const {
    return this->train_duration;
}

int PulseTrain::getNbStages() const {
    return this->nb_stages;
}

PulseStage PulseTrain::getStage(int stage_id) const {
    return this->stages[stage_id];
}

const PulseStage *PulseTrain::getStages() const {
    return this->stages;
}

void PulseTrain::setStimModes(StimMode *modes) {
    for (int i = 0; i < STIMJIM_NB_OUTPUT_CHANNELS; i++) {
        this->output_modes[i] = modes[i];
    }
}

void PulseTrain::setTrainDuration(float trainDuration) {
    this->train_duration = trainDuration;
}

void PulseTrain::setPeriod(float period) {
    this->period = period;
}

void PulseTrain::setFrequency(float frequency) {
    this->period = 1 / frequency;
}

void PulseTrain::setDuration(float duration) {
    this->train_duration = duration;
}



