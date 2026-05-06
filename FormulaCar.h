#include<bits/stdc++.h>
using namespace std;

class FormulaCar{
friend class PitStop;
    protected:
        string teamName, driverName;
        float currentSpeed,fuelLevel,totalRaceTime,gapToLeader;  
        int tireAge,carHealth;

    public:
        FormulaCar(string team,string driver){
            teamName=team;
            driverName=driver;
            currentSpeed=0;
            fuelLevel=100.0;
            totalRaceTime=0;
            gapToLeader=0;
            tireAge=0;
            carHealth=100;
            engine = InternalCombustionEngine(enginePower);
        }
        virtual ~FormulaCar() {}

        void setGap(float gap){
            gapToLeader=gap;
        }
        string getDriverName()const{return driverName;}
        string getTeam()const {return teamName;}
        int getTireAge()const {return tireAge;}
        int getHealth()const{return carHealth;}
        float getTotalTime()const{return totalRaceTime;}

        virtual float calculateAerodynamics()=0;
        virtual float enginePowerOutput()=0;
        virtual void applyPerformanceTrait()=0;

        void updateRaceStatus(float lapTime){
            totalRaceTime+=lapTime;
            fuelLevel-=2.2;
            tireAge+=1;
            carHealth-=(rand()%3);   //random value
            engine.applyWear(2.5);
            ers.applyWear(1.8);
            gearbox.applyWear(1.2);
            bool engineBlown = engine.checkComponentFailure();
            bool ersFailed   = ers.checkComponentFailure();
            bool gearFailed  = gearbox.checkComponentFailure();
            if (engineBlown) {
                currentSpeed = 0;
            } else {
                currentSpeed = currentSpeed * gearbox.getSpeedMultiplier();
            }
            ers.recharge(0.3);
        }
};
class RedBull: public FormulaCar{
    private:
    float aeroCoefficient=1.25;

    public:
    RedBull(string driver):FormulaCar("RedBull",driver){}

    float calculateAerodynamics() {return aeroCoefficient *105.0;}

    float enginePowerOutput() {return 950.0;}

    void applyPerformanceTrait() {currentSpeed=310+(calculateAerodynamics()*0.1);}   
};

class Ferrari: public FormulaCar{
    private:
        float reliablilityMultiplier=0.2;

    public:
    Ferrari(string driver):FormulaCar("Ferrari",driver){}

    float calculateAerodynamics() {return  112.0;}

    float enginePowerOutput() {return 1300*reliablilityMultiplier;}

    void applyPerformanceTrait() {currentSpeed=(enginePowerOutput())/3.0+10.0;}
};
class Mercedes: public FormulaCar{
    public:
        Mercedes(string driver):FormulaCar("Mercedes",driver){}

        float calculateAerodynamics() {return 118.0;}

        float enginePowerOutput() {return 985.0;}

        void applyPerformanceTrait() {
            currentSpeed=318.0;
            fuelLevel+=0.4;
        }
};

class McLaren : public FormulaCar {
private:
    float downforceBoost = 1.15;
    
public:
    McLaren(string driver) : FormulaCar("McLaren", driver) {}

    float calculateAerodynamics() { return 120.0*downforceBoost;}

    float enginePowerOutput() { return 975.0;}

    void applyPerformanceTrait() {currentSpeed=312.0+(calculateAerodynamics()* 0.08);}
};

class Audi : public FormulaCar {
public:
    Audi(string driver) : FormulaCar("Audi", driver) {}

    float calculateAerodynamics() { return 114.0;}

    float enginePowerOutput() {return 1005.0;}

    void applyPerformanceTrait() {
        currentSpeed = 316.0;
        if (carHealth < 95) carHealth += 1; 
    }
};
