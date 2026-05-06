#include <iostream>

using namespace std;

//ALL TIRES INFO

class Tire {
protected:
    float gripLevel;

public:
    Tire(float initialGrip) : gripLevel(initialGrip) {}
    virtual ~Tire() {}

    // Polymorphism
    virtual float getGripLevel() {
        return gripLevel;
    }

    // Method to handle per lap tire grip degradation
    virtual void degradePerLap() = 0; 
    virtual string getCompoundName() = 0;
};

class SoftCompound : public Tire {
public:
    // SoftCompound returns high grip
    SoftCompound() : Tire(100.0f) {} 

    float getGripLevel() {
        return gripLevel;
    }
   string getCompoundName() override { 
        return "SOFT"; 
    }

    void degradePerLap() {
        // loses 5% every lap
        gripLevel -= 5.0f; 
        if (gripLevel < 0.0f) gripLevel = 0.0f;
    }
};

class MediumCompound : public Tire {
public:
    MediumCompound() : Tire(85.0f) {}

    float getGripLevel() {
        return gripLevel;
    }
    string getCompoundName() override { 
        return "MEDIUM"; 
    }

    void degradePerLap() {
        gripLevel -= 3.0f; 
        if (gripLevel < 0.0f) gripLevel = 0.0f;
    }
};

class HardCompound : public Tire {
public:
    // HardCompound returns medium grip
    HardCompound() : Tire(70.0f) {} 

    float getGripLevel() {
        return gripLevel;
    }
   string getCompoundName() override { 
        return "HARD"; 
    }

    void degradePerLap() {
        // only loses 1% every lap
        gripLevel -= 1.0f; 
        if (gripLevel < 0.0f) gripLevel = 0.0f;
    }
};

class WetTire : public Tire {
public:
    WetTire() : Tire(75.0f) {}

    float getGripLevel() {
        return gripLevel;
    }
   string getCompoundName() override { 
        return "WET"; 
    }

    void degradePerLap() {
        gripLevel -= 2.0f;
        if (gripLevel < 0.0f) gripLevel = 0.0f;
    }
};

class PitStop {
public:
    // Swap Tire Object
    void performPitStop(FormulaCar& car, Tire* newTire) {
        if (car.currentTire != nullptr) {
            delete car.currentTire; // Free the old tire memory
        }
        
        car.currentTire = newTire; // Swap to the new tire
        
        // adding 25 seconds to the car's total time taken during Pitstop
        car.totalRaceTime += 25.0f; 
    }
};

