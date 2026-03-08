#ifndef PMU_MANAGER_H
#define PMU_MANAGER_H

#include <Arduino.h>
#include <Wire.h>
#include <XPowersLib.h>

class PMUManager {
public:
    static bool begin();
    static void enableMicrophonePower();
};

#endif // PMU_MANAGER_H
