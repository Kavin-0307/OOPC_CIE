#include <iostream>
#include <vector>
#include <algorithm>
#include <thread>
#include <chrono>

#include "Components.h"
#include "Tires.h"
#include "FormulaCar.h"
#include "Telemetry.h"

using namespace std;

class PitStop {
public:
    void performPitStop(FormulaCar& car, Tire* newTire) {
        if (car.currentTire != nullptr) {
            delete car.currentTire; 
        }
        car.currentTire = newTire; 
        car.totalRaceTime += 25.0f; 
    }
};

class WeatherSystem {
public:
    static float trackTemperature;
    static float precipitationLevel;

    static void updateWeather() {
        float tempDelta = ((rand() % 41) - 20) / 10.0f;
        trackTemperature = max(15.0f, min(55.0f, trackTemperature + tempDelta));

        float rainDelta = ((rand() % 21) - 10) / 100.0f;
        precipitationLevel = max(0.0f, min(1.0f, precipitationLevel + rainDelta));
    }
};

float WeatherSystem::trackTemperature = 30.0f;
float WeatherSystem::precipitationLevel = 0.0f;

class SafetyCar {
public:
    static bool isActive;
    static void deploy() { isActive = true; }
    static void recall() { isActive = false; }
};
bool SafetyCar::isActive = false;

// --- HELPER FUNCTION TO SORT CARS ---
bool compareTotalTime(const FormulaCar* a, const FormulaCar* b) {
    return a->getTotalTime() < b->getTotalTime();
}


int main() {
    srand(time(0));

    Telemetry ui;

    //Initialize the Grid (Polymorphism)
    vector<FormulaCar*> grid;
    grid.push_back(new RedBull("Max Verstappen"));
    grid.push_back(new Ferrari("Charles Leclerc"));
    grid.push_back(new Mercedes("Lewis Hamilton"));
    grid.push_back(new McLaren("Lando Norris"));
    grid.push_back(new Audi("Carlos Sainz"));

    // Equip starting tires
    for (auto car : grid) {
        car->currentTire = new SoftCompound();
    }

    int totalLaps = 5;
    PitStop pitCrew;

    // The Race Loop
    for (int lap = 1; lap <= totalLaps; lap++) {
        WeatherSystem::updateWeather();
        
        // Random chance for safety car
        if (rand() % 10 == 0) SafetyCar::deploy();
        else SafetyCar::recall();

        if (SafetyCar::isActive) {
            ui.addCommentary("SAFETY CAR DEPLOYED! Speed limited.");
        } else if (WeatherSystem::precipitationLevel > 0.5) {
            ui.addCommentary("Heavy rain reported on track!");
        }

        // Simulate Lap for each car
        for (auto car : grid) {
            // Apply car specific traits
            car->applyPerformanceTrait();
            
            // Calculate base lap time 
            float baseLapTime = 90.0f;
            float speedBonus = car->enginePowerOutput() / 100.0f;
            float aeroBonus = car->calculateAerodynamics() / 50.0f;
            
            float lapTime = baseLapTime - speedBonus - aeroBonus;

            // Apply weather and safety car penalties
            if (SafetyCar::isActive) lapTime += 20.0f; 
            lapTime += (WeatherSystem::precipitationLevel * 15.0f); // Rain slows them down

            // Update car status (Fuel, wear, health)
            car->updateRaceStatus(lapTime);
            
            // Tire wear logic
            if (car->currentTire) {
                car->currentTire->degradePerLap();
                // Pitstop trigger if grip is too low
                if (car->currentTire->getGripLevel() < 30.0f) {
                    ui.addCommentary(car->getDriverName() + " is BOXING for new Hard tires!");
                    pitCrew.performPitStop(*car, new HardCompound());
                }
            }
        }

        // Sort grid by who has the lowest total race time 
        sort(grid.begin(), grid.end(), compareTotalTime);

        // Calculate Gaps
        float leaderTime = grid[0]->getTotalTime();
        for (auto car : grid) {
            car->setGap(car->getTotalTime() - leaderTime);
        }

        // Render the Dashboard
        ui.renderDashboard(lap, totalLaps, grid);
        this_thread::sleep_for(chrono::milliseconds(1500));
    }

    //Archive Results & Cleanup
    ui.addCommentary("CHEQUERED FLAG! " + grid[0]->getDriverName() + " WINS!");
    ui.renderDashboard(totalLaps, totalLaps, grid); // Final render
    
    ui.archiveResults(grid);

    // Free memory
    for (auto car : grid) {
        delete car;
    }

    return 0;
}