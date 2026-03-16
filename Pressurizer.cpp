#include<iostream>
#include<fstream>
double dummywtemp;
struct ERod {
    double epow; // Electric rod power
    double hgen; // heat generated
    double hdem; // heat demand
    double wmass; // water mass
    double rstat; // electric rod state(increasing power/ decreasing power / remains the same or unchanged)
    double voltage = 480; // taking it as standard voltage of an electric industrial heater which varies from 240V to 600V
    double resistance = 10; // taking it as 10 ohms simply
    double watertemp;
    int numofrods = 112;
};

void update(ERod& er1, double dtemp, double dti ) {   //dti means delta time

    if( dtemp > 0.2000){ // margin of 0.2000
        // if the the amount of water present is supplied with 5111J/kg to raise water temp by 1C because dynamic water heat requirement value change
        // based on the coolant press is something i'll do afterwards but for now target temp is 345C
        er1.hdem = dtemp * 5111 * er1.wmass;
        er1.rstat = 1; 
        //Power = (Voltage)^2 / R,
        er1.epow =(((er1.voltage * er1.voltage) / er1. resistance) * er1.numofrods) ;
        // 1W = 1J

        er1.hgen = er1.epow * dti; // to make the heat level step ups sensible and not just instant
        er1.watertemp = er1.watertemp + (er1.hgen / (er1.wmass * 5111));
        

        
    }
    else if( dtemp < -0.2000){     // margin of -0.2000
        er1.hdem =  dtemp * 5111 * er1.wmass;
        er1.rstat = -1;
        er1.epow =(((er1.voltage * er1.voltage) / er1. resistance) * er1.numofrods); 
        er1.hgen = -er1.epow * dti ;
        er1.watertemp = er1.watertemp + (er1.hgen / (er1.wmass * 5111));
    
    } 
    else {
        er1.rstat = 0;
        er1.hdem = dtemp * 5111 * er1.wmass;
        er1.epow = 0;
        er1.hgen = er1.epow * dti;
    }
}

int main(){
    ERod r1;
    double dwt; // dwt = delta water temperature
    std::cout << "Enter The current water temperature: ";
    std::cin >> r1.watertemp;
    dummywtemp = r1.watertemp;
    dwt = 345 - r1.watertemp;
    std :: cout << "Enter Water Mass present: ";
    std :: cin >> r1.wmass;
    double dti = 0.1;
    int i = 0;
    std::ofstream datafile("pressurizer_data.txt");
    while (r1.watertemp < 344.8){
        dwt = 345 - r1.watertemp; // calculated ONCE before loop, never again
        update(r1, dwt, dti);
        datafile << i << " " << r1.watertemp << " " << r1.hdem/1000 << " " << r1.epow << " " << r1.hgen << "\n";
        std :: cout << "Timestep " << i << std:: endl;
        std :: cout << "Heat demand is: " << r1.hdem/1000<< "kJ" << std:: endl;
        if(r1.rstat == 1){
            std::cout << "Electric Rod power is: " << r1.epow <<" kW"<<std:: endl;
            std::cout << "Electric Rod state is On" << std :: endl;
            std::cout << "Electric Rod heat generated is: " << r1.hgen << std :: endl;
            std::cout << "New Water temperature is: " << r1.watertemp << std :: endl;
        }
        else if(r1.rstat == -1){
            std::cout << "Electric Rod power is: " << r1.epow << " kW"<<std:: endl;
            std::cout << "Electric Rod state is Off" << std :: endl;
            std::cout << "Electric Rod heat generated is: " << r1.hgen << std :: endl;
               std::cout << "New Water temperature is: " << r1.watertemp << std :: endl;
        }
        else {
            std::cout << "Electric Rod power is: " << r1.epow << " kW"<<std :: endl;
            std::cout << "Electric Rod state is Off" << std :: endl;
            std::cout << "Electric Rod heat generated is: " << r1.hgen << std :: endl;
               std::cout << "New Water temperature is: " << r1.watertemp << std :: endl;
        }
        i++;
    }
    datafile.close();
    return 0;
}