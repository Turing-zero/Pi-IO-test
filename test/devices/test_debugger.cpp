#include "debugger.h"
#include "ini.h"
#include <map>

struct Config{
    std::map<std::string,std::map<std::string,std::string>> sections;
};

// 处理程序函数，每当解析到一个键值对时调用
static int iniHandler(void* user, const char* section, const char* name, const char* value) {
    Config* config = static_cast<Config*>(user);
    config->sections[section][name] = value;
    return 1;
}

int main(){
    Debugger debug;
    debug.open_device(ROCKER);
    // debug.open_device(BUTTON);

    // Config config;
    // if (ini_parse("../config.ini", iniHandler, &config) < 0){
    //     std::cerr << "Can't load 'config.ini'\n";
    //     return 1;
    // }

    // for (const auto& section : config.sections) {
    //     if(section.first=="rs485") std::cout << "yes" <<std::endl;
    //     std::cout << "[" << section.first << "]" << std::endl;
    //     for (const auto& key_value : section.second) {
    //         auto six = key_value.second;
    //         int x = atoi(six.c_str());
    //         std::cout << key_value.first << "=" << /*key_value.second*/x << std::endl;
    //     }
    // }
    while(true){
        std::cout << debug.get_value().first <<" "<< debug.get_value().second << std::endl;
        // std::cout << debug.get_status(sw2) <<std::endl;
    }
}