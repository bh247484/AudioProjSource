/*
  ==============================================================================

    ADSR.h
    Created: 7 Dec 2020 6:10:56pm
    Author:  bh247

  ==============================================================================
*/

#pragma once

class ADSR {
public:
    ADSR();
    ~ADSR();
    double process();
    double getOutput();
    int getState();
    void gate(bool on);
    void setAttackRate(double rate);
    void setDecayRate(double rate);
    void setReleaseRate(double rate);
    void setSustainLevel(double level);
    void setTargetRatioA(double targetRatio);
    void setTargetRatioDR(double targetRatio);
    void reset();

    enum envState {
        envIdle = 0,
        envAttack,
        envDecay,
        envSustain,
        envRelease
        
    };

protected:
    int state;
    double output;
    double attackRate;
    double decayRate;
    double releaseRate;
    double attackCoef;
    double decayCoef;
    double releaseCoef;
    double sustainLevel;
    double targetRatioA;
    double targetRatioDR;
    double attackBase;
    double decayBase;
    double releaseBase;

    double calcCoef(double rate, double targetRatio);
};

inline double ADSR::process() {
    switch (state) {
    case envIdle:
        break;
    case envAttack:
        output = attackBase + output * attackCoef;
        if (output >= 1.0) {
            output = 1.0;
            state = envDecay;
        }
        break;
    case envDecay:
        output = decayBase + output * decayCoef;
        if (output <= sustainLevel) {
            output = sustainLevel;
            state = envSustain;
        }
        break;
    case envSustain:
        break;
    case envRelease:
        output = releaseBase + output * releaseCoef;
        if (output <= 0.0) {
            output = 0;
            state = envIdle;
        }
    }
    return output;
}

inline void ADSR::gate(bool gate) {
    if (gate)
        state = envAttack;
    else if (state != envIdle)
        state = envRelease;
}

inline int ADSR::getState() {
    return state;
}

inline void ADSR::reset() {
    state = envIdle;
    output = 0.0;
}

inline double ADSR::getOutput() {
    return output;
}