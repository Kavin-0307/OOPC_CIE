#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#pragma once
using namespace std;

//Internal Combustion Engine Class
class InternalCombustionEngine {
private:
    //wear level and base power
    double wearLevel;
    double basePower;

public:
    InternalCombustionEngine(double power = 1000.0)
        : wearLevel(0.0), basePower(power) {}

    // To add wear
    void applyWear(double amount) {
        wearLevel = min(100.0, wearLevel + amount);
    }

    // To get current wear level
    double getWearLevel() const { return wearLevel; }

    // To get power output
    double getPowerOutput() const {
        return basePower * (1.0 - wearLevel / 100.0);
    }

    // To check working of component
    bool checkComponentFailure() const {
        double failureProb = (wearLevel / 100.0) * 0.6;
        double roll = (double)rand() / RAND_MAX;
        return roll < failureProb;
    }

    // Display current status of object
    void display() const {
        cout << "  [ICE] Power: " << getPowerOutput() << " HP"
             << " | Wear: " << wearLevel << "%" << endl;
    }
};

//Energy Recovery System
class ERS {
private:
    double wearLevel;
    double storedEnergy;
    const double maxEnergy = 4.0;

public:
    ERS() : wearLevel(0.0), storedEnergy(4.0) {}

    // To apply wear
    void applyWear(double amount) {
        wearLevel = min(100.0, wearLevel + amount);
    }

    // To get current wear level
    double getWearLevel() const { return wearLevel; }

    //Deploying boost
    double deployBoost() {
        if (storedEnergy <= 0) return 0.0;
        double boost = 160.0 * (storedEnergy / maxEnergy);
        storedEnergy = max(0.0, storedEnergy - 0.5);
        return boost;
    }

    // Recharging boost
    void recharge(double amount) {
        storedEnergy = min(maxEnergy, storedEnergy + amount);
    }

    // Checking if component failed
    bool checkComponentFailure() const {
        double failureProb = (wearLevel / 100.0) * 0.4;
        double roll = (double)rand() / RAND_MAX;
        return roll < failureProb;
    }

    //Displaying current level
    void display() const {
        cout << "  [ERS] Energy: " << storedEnergy << " MJ"
             << " | Wear: " << wearLevel << "%" << endl;
    }
};

//Transmission class
class Transmission {
private:
    double wearLevel;
    bool failed;

public:
    Transmission() : wearLevel(0.0), failed(false) {}

    // Applying wear
    void applyWear(double amount) {
        wearLevel = min(100.0, wearLevel + amount);
    }

    // To get current wear level
    double getWearLevel() const { return wearLevel; }

    //To check if transmission has failed
    bool hasFailed() const { return failed; }

    // Speed is capped at 50% is transmission fails
    double getSpeedMultiplier() const {
        return failed ? 0.5 : 1.0;
    }

    //Check if components failed
    bool checkComponentFailure() {
        if (failed) return true;
        double failureProb = (wearLevel / 100.0) * 0.5;
        double roll = (double)rand() / RAND_MAX;
        if (roll < failureProb) {
            failed = true;
            cout << "  *** TRANSMISSION FAILURE! Speed capped at 50%! ***" << endl;
        }
        return failed;
    }

    //Display data
    void display() const {
        cout << "  [Gearbox] Wear: " << wearLevel << "%"
             << " | Status: " << (failed ? "FAILED" : "OK") << endl;
    }
};