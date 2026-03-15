#include <iostream>

// One pressurizer node
struct Node {
    double temperature;  // Celsius
    double pressure;     // Bar
    double mass;         // kg of fluid in this node
    bool isSteam;        // liquid or steam
};

// Calculate new temperature after heat input
void updateNode(Node& node, double heatInput, double specificHeat, double deltaTime) {
    // Q = m * c * dT  →  dT = Q / (m * c)
    double deltaTemp = (heatInput * deltaTime) / (node.mass * specificHeat);
    node.temperature += deltaTemp;

    // Simplified pressure update — pressure rises with temperature
    // At 344C → 155 bar (saturation relationship)
    node.pressure = node.temperature * 0.45;  // rough approximation for now

    // Determine fluid state
    if (node.temperature >= 344.0) {
        node.isSteam = true;
    } else {
        node.isSteam = false;
    }
}

int main() {
    // Create one node
    Node n1;
    n1.temperature = 300.0;  // starting temp
    n1.pressure = 135.0;     // starting pressure
    n1.mass = 10.0;          // 10kg of water in this node
    n1.isSteam = false;

    double heatInput = 5000.0;    // watts from heater rod
    double specificHeat = 4200.0; // J/kg°C for water
    double deltaTime = 0.1;       // 0.1 second timestep

    // Run 10 timesteps
    for (int i = 0; i < 10; i++) {
        updateNode(n1, heatInput, specificHeat, deltaTime);
        std::cout << "Timestep " << i+1 
                  << " | Temp: " << n1.temperature 
                  << "C | Pressure: " << n1.pressure 
                  << " bar | State: " << (n1.isSteam ? "STEAM" : "LIQUID") 
                  << std::endl;
    }

    return 0;
}