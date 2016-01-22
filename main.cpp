#include <iostream>
#include "structures.hpp"
#include "core/threads.hpp"
#include <classes/environment.hpp>

using namespace tesis;

int main(int argc, char **argv) {
    
    EnvironmentConfiguration config("/home/diego/dev/c++/tesis-ardrone/config/environment.json");
    
    EnvironmentConfig c = config.get();
    
    std::cout << c.safe_spot[0].Hue.min;
    
    
    return 0;
}
