#include "env_file_handler.hpp"

std::unordered_map<std::string, std::string> ReadEnvFile(const std::string& fileName) {
    std::ifstream env_file(fileName);
    std::unordered_map<std::string, std::string> envVars;
    
    if(!env_file.is_open()) {
        std::cerr << "Could not open Env file" << std::endl;
        return {};
    } else {
        std::string key_value;
        while(std::getline(env_file, key_value)) {
            int equal_pos = key_value.find('=');
            if(equal_pos != std::string::npos) {
                std::string key = key_value.substr(0, equal_pos);
                std::string value = key_value.substr(equal_pos + 1);
                envVars[key] = value;
            } 
        }
    }
    env_file.close();
    return envVars;
}