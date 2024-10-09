#pragma once
#include <algorithm>
#include <cstdio>
#include <vector>
#include <cstring>
#include <unordered_map>
#include <fstream>
#include <filesystem>
#include <utility>
#include "../imgui/imgui.h"

enum class HitBoxes {
    HEAD = 1 << 0,
    NECK = 1 << 1,
    CHEST = 1 << 2,
    STOMACH = 1 << 3,
    PELVIS = 1 << 4,
};

#define CONFIGINT(name) Config::config.at(name).intValue
#define CONFIGBOOL(name) Config::config.at(name).boolValue
#define CONFIGSTR(name) Config::config.at(name).strValue
#define CONFIGCOL(name) Config::config.at(name).colValue

#define CONFIGITEM(name, value) {name, ConfigItem(value)}

enum ConfigItemType {
    INT,
    FOOL,
    STR,
    COLOR
};

namespace Config {
    inline std::vector<std::string> cfgFiles;
    inline char configFileName[128] = "gmod_re.cfg";
    inline char cfgDir[256];

    class ConfigItem {
        public:
        ConfigItem(int value) {
            type = INT;
            intValue = value;
        }
        ConfigItem(bool value) {
            type = FOOL;
            boolValue = value;
        }
        ConfigItem(char* value) {
            type = STR;
            strValue = value;
        }
        ConfigItem(ImColor value) {
            type = COLOR;
            colValue = value;
        }
        ConfigItemType type;
        int intValue = -1;
        bool boolValue = false;
        char* strValue;
        ImColor colValue = ImColor(255, 255, 255, 255);
    };

    inline std::unordered_map<std::string_view, ConfigItem> config {
        // Legit {
            // LegitBot {
                // Default {
                    CONFIGITEM("Legit>LegitBot>Default>Key", 0),
            // }
        // }
        // RageBot {
            // Default {
                CONFIGITEM("Rage>Enabled", false),
            //}
        // }
        // Visuals {
            // ESP {
                CONFIGITEM("Visuals>Enabled", true),
                
            // }
        // }
        // Misc {
            CONFIGITEM("Misc>Misc>Movement>Auto Hop", false),
            

        // }
    };

    inline void reloadCfgList()
    {
        Config::cfgFiles.clear();
        char path[128];
        strcpy(path, getenv("HOME"));
        strcat(path, "/.gmod_re");
        if (!std::filesystem::is_directory(path)) {
            std::filesystem::create_directory(path);
        }
        strcat(path, "/configs");
        if (!std::filesystem::is_directory(path)) {
            std::filesystem::create_directory(path);
            return;
        }
        for (const auto &entry : std::filesystem::directory_iterator(cfgDir)) {
            Config::cfgFiles.push_back(entry.path().string().substr(strlen(path)+1));
        }
        std::sort(Config::cfgFiles.begin(), Config::cfgFiles.end());
    }

    inline void save() {
        std::ofstream configFile;
        char path[128];
        if (configFileName[0] == '/') {
            strcpy(path, configFileName);
        } else {
            strcpy(path, cfgDir);
            strcat(path, configFileName);
        }
        configFile.open(path);
        for (auto i : config) {
            switch (i.second.type) {
                case INT:
                    configFile << i.second.type << "\t"<< i.first << "\t" << i.second.intValue << "\n";
                break;
                case FOOL:
                    configFile << i.second.type << "\t" << i.first << "\t" << i.second.boolValue << "\n";
                break;
                case STR:
                    configFile << i.second.type << "\t" << i.first << "\t" << i.second.strValue << "\n";
                break;
                case COLOR:
                    configFile << i.second.type << "\t" << i.first << "\t" << i.second.colValue.Value.x << "|" << i.second.colValue.Value.y << "|" << i.second.colValue.Value.z << "|" << i.second.colValue.Value.w << "\n";
                break;
            }
        }
        configFile.close();
        reloadCfgList();
    }

    inline void load() {
        std::string line;
        std::ifstream configFile;
        char path[128];
        if (configFileName[0] == '/') {
            strcpy(path, configFileName);
        } else {
            strcpy(path, cfgDir);
            strcat(path, configFileName);
        }
        configFile.open(path);
        while(std::getline(configFile, line)) {
            ConfigItemType type;
            char name[64];
            char value[64];
            sscanf(line.c_str(), "%d\t%[^\t]\t%[^\t]", (int*)&type, name, value);
            try {
                switch (type) {
                    case INT:
                        CONFIGINT(name) = atoi(value);
                    break;
                    case FOOL:
                        CONFIGBOOL(name) = atoi(value);
                    break;
                    case STR:
                        CONFIGSTR(name) = value;
                    break;
                    case COLOR:
                        float r, g, b, a;
                        sscanf(value, "%f|%f|%f|%f", (float*)&r, (float*)&g, (float*)&b, (float*)&a);
                        CONFIGCOL(name) = ImColor(r, g, b, a);
                    break;
                }
            } catch (std::out_of_range& e) {
                ;
            }
        }
    }

    inline void remove() {
        std::vector<std::string>::iterator itr = std::find(cfgFiles.begin(), cfgFiles.end(), configFileName);
        if (itr != cfgFiles.cend()) {
            char path[128];
            strcpy(path, cfgDir);
            strcat(path, configFileName);
            std::remove(path);
            cfgFiles.erase(itr);
        }
    }

    inline void init() {
        strcpy(cfgDir, getenv("HOME"));
        strcat(cfgDir, "/.gmod_re/configs/");
        reloadCfgList();
        if (cfgFiles.size() == 1) {
            strcpy(configFileName, cfgFiles[0].c_str());
        }
    }
}
