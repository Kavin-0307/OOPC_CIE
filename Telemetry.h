#ifndef TELEMETRY_H
#define TELEMETRY_H

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include "FormulaCar.h"


class FormulaCar;

// ANSI Color Codes
#define RESET   "\033[0m"
#define RED     "\033[31m"      // Ferrari
#define BLUE    "\033[34m"      // Red Bull
#define CYAN    "\033[36m"      // Mercedes
#define YELLOW  "\033[33m"      // Warnings/UI
#define GREEN   "\033[32m"      // Health/Success
#define MAGENTA "\033[35m"      // McLaren
#define WHITE   "\033[37m"      // Audi

class Telemetry {
private:
    std::vector<std::string> commentaryLog;
    const int maxLogEntries = 3;

public:
    void addCommentary(std::string message) {
        if (commentaryLog.size() >= maxLogEntries) {
            commentaryLog.erase(commentaryLog.begin());
        }
        commentaryLog.push_back(message);
    }

    std::string getTeamColor(std::string team) {
        if (team == "RedBull") return BLUE;
        if (team == "Ferrari") return RED;
        if (team == "Mercedes") return CYAN;
        if (team == "McLaren") return MAGENTA;
        if (team == "Audi") return WHITE;
        return RESET;
    }

    
    void renderDashboard(int currentLap, int totalLaps, const std::vector<FormulaCar*>& cars) {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif

        std::cout << YELLOW << "===============================================================" << RESET << std::endl;
        std::cout << " F1 LIVE TELEMETRY - LAP " << currentLap << " / " << totalLaps << std::endl;
        std::cout << YELLOW << "===============================================================" << RESET << std::endl;

        
        std::cout << std::left << std::setw(5) << "POS" 
                  << std::setw(15) << "DRIVER" 
                  << std::setw(12) << "TEAM" 
                  << std::setw(12) << "GAP" 
                  << std::setw(10) << "TIRE" 
                  << "HEALTH" << std::endl;
        std::cout << "---------------------------------------------------------------" << std::endl;

        for (size_t i = 0; i < cars.size(); ++i) {
            // Using getters from Member 1
            std::string color = getTeamColor(cars[i]->getTeam());
            
            std::cout << color << std::left << std::setw(5) << i + 1 
                      << std::setw(15) << cars[i]->getDriverName()
                      << std::setw(12) << cars[i]->getTeam();
            
            // F1 Style Gap formatting: +1.234s
            if (i == 0) {
                std::cout << std::setw(12) << "LEADER";
            } else {
                std::cout << "+" << std::fixed << std::setprecision(3) 
                          << std::setw(11) << cars[i]->getGap();
            }

            std::cout << std::left << std::setw(10) << std::to_string(cars[i]->getTireAge()) + "L"
                      << cars[i]->getHealth() << "%" << RESET << std::endl;
        }

        std::cout << "\n" << YELLOW << "[LIVE COMMENTARY]" << RESET << std::endl;
        for (const auto& log : commentaryLog) {
            std::cout << "> " << log << std::endl;
        }
        std::cout << YELLOW << "===============================================================" << RESET << std::endl;
    }

    
    void archiveResults(const std::vector<FormulaCar*>& finalStandings) {
        std::ofstream file("race_results.csv");
        file << "Position,Driver,Team,FinalTime\n";
        for (size_t i = 0; i < finalStandings.size(); ++i) {
            file << i + 1 << "," 
                 << finalStandings[i]->getDriverName() << "," 
                 << finalStandings[i]->getTeam() << "," 
                 << std::fixed << std::setprecision(3) << finalStandings[i]->getTotalTime() << "\n";
        }
        file.close();
        std::cout << GREEN << "\n[System] Race results successfully archived to race_results.csv" << RESET << std::endl;
    }
};

#endif
