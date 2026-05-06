#include <bits/stdc++.h>
using namespace std;

//  Forward declaration
class FormulaCar;

//  WeatherSystem — Static global environment
class WeatherSystem
{
public:
    static float trackTemperature;   // °C
    static float precipitationLevel; // 0.0 (dry) → 1.0 (heavy rain)

    void updateWeather(){
        float tempDelta = ((rand() % 41) - 20) / 10.0f;
        trackTemperature = max(15.0f, min(55.0f, trackTemperature + tempDelta));

        float rainDelta = ((rand() % 21) - 10) / 100.0f;
        precipitationLevel = max(0.0f, min(1.0f, precipitationLevel + rainDelta));

        cout << "[WeatherSystem] Temp: " << trackTemperature
             << " °C  |  Rain: " << precipitationLevel << "\n";
    }

    void printConditions(){
        cout << "  Track Temp    : " << trackTemperature << " °C\n"
             << "  Precipitation : " << precipitationLevel << "\n";
    }
};

float WeatherSystem::trackTemperature   = 30.0f;
float WeatherSystem::precipitationLevel = 0.0f;

//  SafetyCar — Simple state object
class SafetyCar{
public:
    static bool isActive;

    void deploy(){
        isActive = true;
        cout << "[SafetyCar] ⚠  Safety Car DEPLOYED — overtaking suspended.\n";
    }

    void recall(){
        isActive = false;
        cout << "[SafetyCar] Safety Car recalled — racing resumes.\n";
    }
};

bool SafetyCar::isActive = false;

//  FormulaCar — Concrete race car
class FormulaCar{
private:
    string driverName;
    int    position;
    float  speed;
    float  baseSpeed;
    static constexpr float SAFETY_CAR_SPEED_LIMIT = 120.0f;

public:
    FormulaCar(const string &name, int startPos, float topSpeed)
        : driverName(name), position(startPos),
          speed(topSpeed), baseSpeed(topSpeed) {}

    void limitSpeed(){
        speed = SAFETY_CAR_SPEED_LIMIT;
        cout << "  [" << driverName << "] P" << position
             << " — speed limited to " << speed << " km/h (Safety Car)\n";
    }

    void updatePosition(){
        float grip        = 1.0f - (WeatherSystem::precipitationLevel * 0.4f);
        float heatPenalty = (WeatherSystem::trackTemperature > 45.0f) ? 0.95f : 1.0f;
        speed             = baseSpeed * grip * heatPenalty;

        int delta = (rand() % 3) - 1;
        position  = max(1, position + delta);

        cout << "  [" << driverName << "] P" << position
             << " — speed: " << speed << " km/h\n";
    }

    int    getPosition()   const { return position;   }
    string getDriverName() const { return driverName; }
    float  getSpeed()      const { return speed;      }

    static bool comparePositions(const FormulaCar *a, const FormulaCar *b){ return a->getPosition() < b->getPosition();}
};

//  RaceOfficial — Issues decisions / penalties
class RaceOfficial
{
private:
    string    name;
    SafetyCar safetyCar; 

public:
    explicit RaceOfficial(const string &officialName) : name(officialName) {}

    void issuePenalty(FormulaCar &car, const string &reason) const{
        cout << "[Official: " << name << "] ⚑  Penalty issued to "
             << car.getDriverName() << " — Reason: " << reason << "\n";
    }

    void deploySafetyCar(){
        cout << "[Official: " << name << "] Ordering Safety Car deployment.\n";
        safetyCar.deploy();  
    }

    void recallSafetyCar(){
        cout << "[Official: " << name << "] Ordering Safety Car recall.\n";
        safetyCar.recall();  
    }
};

//  RaceTrack — Lap Logic controller
class RaceTrack{
    private:
    string trackName;
    int totalLaps;
    int currentLap;
    WeatherSystem weather;   

public:
    RaceTrack(const string &name, int laps)
        : trackName(name), totalLaps(laps), currentLap(0) {}

    void lapLogic(vector<FormulaCar *> &grid){
        ++currentLap;
        cout << "\n══════════════════════════════════════\n";
        cout << "  " << trackName
             << "  |  Lap " << currentLap << " / " << totalLaps << "\n";
        cout << "══════════════════════════════════════\n";

        weather.updateWeather();   
        weather.printConditions(); 
        cout << "--------------------------------------\n";

        for (auto &car : grid){
            if (SafetyCar::isActive)
                car->limitSpeed();
            else
                car->updatePosition();
        }

        if (!SafetyCar::isActive)
            sort(grid.begin(), grid.end(), FormulaCar::comparePositions);
    }

    void printGrid(const vector<FormulaCar *> &grid) const{
        cout << "\n--- Current Grid ---\n";
        for (size_t i = 0; i < grid.size(); ++i){
            cout << "  P" << (i + 1) << "  "
                 << grid[i]->getDriverName()
                 << "  (" << grid[i]->getSpeed() << " km/h)\n";
        }
    }

    bool isRaceOver() const { return currentLap >= totalLaps; }
};