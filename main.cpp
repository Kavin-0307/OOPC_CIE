#include <bits/stdc++.h>
using namespace std;

//  Forward declaration
class FormulaCar;

//  WeatherSystem — Static global environment
class WeatherSystem{
public:
    static float trackTemperature;   // °C
    static float precipitationLevel; // 0.0 (dry) → 1.0 (heavy rain)

    // Randomise small weather shifts each call
    static void updateWeather(){
        // Temperature drifts ±2 °C
        float tempDelta = ((std::rand() % 41) - 20) / 10.0f; // –2.0 to +2.0
        trackTemperature = std::max(15.0f, std::min(55.0f, trackTemperature + tempDelta));

        // Precipitation drifts ±0.1
        float rainDelta = ((std::rand() % 21) - 10) / 100.0f; // –0.10 to +0.10
        precipitationLevel = std::max(0.0f, std::min(1.0f, precipitationLevel + rainDelta));

        std::cout << "[WeatherSystem] Temp: " << trackTemperature
                  << " °C  |  Rain: " << precipitationLevel << "\n";
    }

    static void printConditions(){
        std::cout << "  Track Temp    : " << trackTemperature << " °C\n"
                  << "  Precipitation : " << precipitationLevel << "\n";
    }
};

// Static member definitions
float WeatherSystem::trackTemperature = 30.0f;
float WeatherSystem::precipitationLevel = 0.0f;

//  SafetyCar — Simple state object
class SafetyCar{
public:
    static bool isActive;

    static void deploy(){
        isActive = true;
        std::cout << "[SafetyCar] ⚠  Safety Car DEPLOYED — overtaking suspended.\n";
    }

    static void recall(){
        isActive = false;
        std::cout << "[SafetyCar] ✅ Safety Car recalled — racing resumes.\n";
    }
};

bool SafetyCar::isActive = false;

//  FormulaCar — Concrete race car
class FormulaCar{
private:
    std::string driverName;
    int position;    // Current grid position (1-based)
    float speed;     // km/h
    float baseSpeed; // Normal racing speed
    static constexpr float SAFETY_CAR_SPEED_LIMIT = 120.0f; // km/h

public:
    FormulaCar(const std::string &name, int startPos, float topSpeed)
        : driverName(name), position(startPos),
          speed(topSpeed), baseSpeed(topSpeed) {}

    // Called under Safety Car — clamp speed, no position change
    void limitSpeed(){
        speed = SAFETY_CAR_SPEED_LIMIT;
        std::cout << "  [" << driverName << "] P" << position
                  << " — speed limited to " << speed << " km/h (Safety Car)\n";
    }

    // Called during normal racing — adjust speed for weather, attempt overtake
    void updatePosition(){
        // Weather degrades speed: rain reduces grip → lower top speed
        float grip = 1.0f - (WeatherSystem::precipitationLevel * 0.4f);
        float heatPenalty = (WeatherSystem::trackTemperature > 45.0f) ? 0.95f : 1.0f;
        speed = baseSpeed * grip * heatPenalty;

        // Small random position swap to simulate overtaking (simplified)
        int delta = (std::rand() % 3) - 1; // –1, 0, or +1
        position = std::max(1, position + delta);

        std::cout << "  [" << driverName << "] P" << position
                  << " — speed: " << speed << " km/h\n";
    }

    // Getters
    int getPosition() const { return position; }
    std::string getDriverName() const { return driverName; }
    float getSpeed() const { return speed; }

    // Allow RaceTrack to sort the grid
    static bool comparePositions(const FormulaCar *a, const FormulaCar *b){
        return a->getPosition() < b->getPosition();
    }
};

//  RaceOfficial — Issues decisions / penalties
class RaceOfficial{
private:
    std::string name;

public:
    explicit RaceOfficial(const std::string &officialName) : name(officialName) {}

    void issuePenalty(FormulaCar &car, const std::string &reason) const {
        std::cout << "[Official: " << name << "] ⚑  Penalty issued to "
                  << car.getDriverName() << " — Reason: " << reason << "\n";
    }

    void deploySafetyCar() const {
        std::cout << "[Official: " << name << "] Ordering Safety Car deployment.\n";
        SafetyCar::deploy();
    }

    void recallSafetyCar() const {
        std::cout << "[Official: " << name << "] Ordering Safety Car recall.\n";
        SafetyCar::recall();
    }
};

//  RaceTrack — Lap Logic controller
class RaceTrack{
private:
    std::string trackName;
    int totalLaps;
    int currentLap;

public:
    RaceTrack(const std::string &name, int laps)
        : trackName(name), totalLaps(laps), currentLap(0) {}

    // Core loop — iterates grid each lap
    void lapLogic(std::vector<FormulaCar *> &grid){
        ++currentLap;
        std::cout << "\n══════════════════════════════════════\n";
        std::cout << "  " << trackName
                  << "  |  Lap " << currentLap << " / " << totalLaps << "\n";
        std::cout << "══════════════════════════════════════\n";

        // Update global weather each lap
        WeatherSystem::updateWeather();
        WeatherSystem::printConditions();
        std::cout << "--------------------------------------\n";

        for (auto &car : grid){
            if (SafetyCar::isActive){
                car->limitSpeed(); // No overtaking allowed
            }
            else{
                car->updatePosition(); // Normal racing
            }
        }

        // Re-sort grid by position after normal racing
        if (!SafetyCar::isActive){
            std::sort(grid.begin(), grid.end(), FormulaCar::comparePositions);
        }
    }

    void printGrid(const std::vector<FormulaCar *> &grid) const{
        std::cout << "\n--- Current Grid ---\n";
        for (size_t i = 0; i < grid.size(); ++i){
            std::cout << "  P" << (i + 1) << "  "
                      << grid[i]->getDriverName()
                      << "  (" << grid[i]->getSpeed() << " km/h)\n";
        }
    }

    bool isRaceOver() const { return currentLap >= totalLaps; }
};