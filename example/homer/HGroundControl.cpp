//
// Ground control app modified from the takeoff_and_land example.
//
// Author: JoJoChen <P46054341@mail.ncku.edu.tw>

#include <chrono>
#include <cstdint>
#include <mavsdk/mavsdk.h>
#include <mavsdk/plugins/action/action.h>
#include <mavsdk/plugins/telemetry/telemetry.h>
#include <iostream>
#include <iomanip>
#include <thread>

using namespace mavsdk;
using namespace std::this_thread;
using namespace std::chrono;

#define ERROR_CONSOLE_TEXT "\033[31m" // Turn text on console red
#define TELEMETRY_CONSOLE_TEXT "\033[34m" // Turn text on console blue
#define SOLAR_IN_TEXT "\033[33m" // Turn text on console yellow
#define SOLAR_OUT_TEXT "\033[36m" // Turn text on console cyan
#define CHARGING_TEXT "\033[32m" // Turn text on console cyan
#define NORMAL_CONSOLE_TEXT "\033[0m" // Restore normal console colour

float batt_voltage;
float batt_current;
float batt_power;
int batt_SOC;
float solar_in_voltage;
float solar_in_current;
float solar_in_power;
float solar_out_voltage;
float solar_out_current;
float solar_out_power;

void usage(std::string bin_name)
{
    std::cout << NORMAL_CONSOLE_TEXT << "Usage : " << bin_name << " <connection_url>" << std::endl
              << "Connection URL format should be :" << std::endl
              << " For TCP : tcp://[server_host][:server_port]" << std::endl
              << " For UDP : udp://[bind_host][:bind_port]" << std::endl
              << " For Serial : serial:///path/to/serial/dev[:baudrate]" << std::endl
              << "For example, to connect to the simulator use URL: udp://:14540" << std::endl;
}

void component_discovered(ComponentType component_type)
{
    std::cout << NORMAL_CONSOLE_TEXT << "Discovered a component with type "
              << unsigned(component_type) << std::endl;
}

int main(int argc, char** argv)
{
    Mavsdk dc;
    std::string connection_url;
    ConnectionResult connection_result;

    bool discovered_system = false;
    if (argc == 2) {
        connection_url = argv[1];
        connection_result = dc.add_any_connection(connection_url);
    } else {
        usage(argv[0]);
        return 1;
    }

    if (connection_result != ConnectionResult::SUCCESS) {
        std::cout << ERROR_CONSOLE_TEXT
                  << "Connection failed: " << connection_result_str(connection_result)
                  << NORMAL_CONSOLE_TEXT << std::endl;
        return 1;
    }

    // We don't need to specify the UUID if it's only one system anyway.
    // If there were multiple, we could specify it with:
    // dc.system(uint64_t uuid);
    System& system = dc.system();

    std::cout << "Waiting to discover system..." << std::endl;
    dc.register_on_discover([&discovered_system](uint64_t uuid) {
        std::cout << "Discovered system with UUID: " << uuid << std::endl;
        discovered_system = true;
    });

    // We usually receive heartbeats at 1Hz, therefore we should find a system after around 2
    // seconds.
    sleep_for(seconds(2));

    if (!discovered_system) {
        std::cout << ERROR_CONSOLE_TEXT << "No system found, exiting." << NORMAL_CONSOLE_TEXT
                  << std::endl;
        return 1;
    }

    // Register a callback so we get told when components (camera, gimbal) etc
    // are found.
    system.register_component_discovered_callback(component_discovered);

    auto telemetry = std::make_shared<Telemetry>(system);
    auto action = std::make_shared<Action>(system);

    // We want to listen to the Voltage of the drone at 1 Hz.
    const Telemetry::Result set_rate_result = telemetry->set_rate_battery(10.0);
    if (set_rate_result != Telemetry::Result::SUCCESS) {
        std::cout << ERROR_CONSOLE_TEXT
                  << "Setting rate failed:" << Telemetry::result_str(set_rate_result)
                  << NORMAL_CONSOLE_TEXT << std::endl;
        return 1;
    }

    const Telemetry::Result set_rate_result_solar_power = telemetry->set_rate_solar_power(10.0);
    if (set_rate_result_solar_power != Telemetry::Result::SUCCESS) {
        std::cout << ERROR_CONSOLE_TEXT
                  << "Setting rate failed:" << Telemetry::result_str(set_rate_result_solar_power)
                  << NORMAL_CONSOLE_TEXT << std::endl;
        return 1;
    }

    // Set up callback to monitor altitude while the vehicle is in flight
    telemetry->battery_async([](Telemetry::Battery battery) {
        batt_voltage=battery.voltage_v;
        batt_current=battery.current_A;
        batt_power=batt_voltage*batt_current;
        batt_SOC=battery.remaining_percent;
        if (batt_voltage>=60){
            batt_voltage=0;
        }
        if (batt_current<0){
            batt_current=0;
        }
        if (batt_power<0){
            batt_power=0;
        }
    });

    telemetry->solar_power_async([](Telemetry::Solar_Power solar_power) {
        solar_in_voltage=solar_power.voltage_in_V;
        solar_in_current=solar_power.current_in_A;
        solar_in_power=solar_power.power_in_W;
        solar_out_voltage=solar_power.voltage_out_V;
        solar_out_current=solar_power.current_out_A;
        solar_out_power=solar_power.power_out_W;
    });
    
    sleep_for(seconds(1));

    // Continuously report the battery and solar power status
    while (true) {
        uint16_t SOC_count=0;
        std::cout << TELEMETRY_CONSOLE_TEXT // set to blue
                  << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n" 
                  << std::left << std::setw(22) << "Battery Voltage:" 
                  << std::right << std::setw(5) << std::fixed << std::setprecision(2) << batt_voltage 
                  << std::left << std::setw(1) << " V\n"
                  << std::left << std::setw(22) << "Discharging Current:"
                  << std::right << std::setw(5) << std::fixed << std::setprecision(2) << batt_current 
                  << std::left << std::setw(1) << " A\n"
                  << std::left << std::setw(22) << "Discharging Power: " 
                  << std::right << std::setw(5) << std::fixed << std::setprecision(2) << batt_power
                  << " W\n" 
                  /*<< SOLAR_IN_TEXT // set to blue
                  << std::left << std::setw(22) << "Solar Voltage (In):"
                  << std::right << std::setw(5) << std::fixed << std::setprecision(2) << solar_in_voltage 
                  << std::left << std::setw(6) << " V"
                  << std::left << std::setw(22) << "Solar Current (In): " 
                  << std::right << std::setw(5) << std::fixed << std::setprecision(2) << solar_in_current 
                  << std::left << std::setw(6) << " A"
                  << std::left << std::setw(20) << "Solar Power (In): " 
                  << std::right << std::setw(5) << std::fixed << std::setprecision(2) << solar_in_power
                  << " W\n"*/ 
                  << SOLAR_IN_TEXT // set to blue
                  << std::left << std::setw(22) << "Solar Voltage:"
                  << std::right << std::setw(5) << std::fixed << std::setprecision(2) << solar_out_voltage
                  << std::left << std::setw(1) << " V\n"
                  << std::left << std::setw(22) << "Solar Current: " 
                  << std::right << std::setw(5) << std::fixed << std::setprecision(2) << solar_out_current 
                  << std::left << std::setw(1) << " A\n"
                  << std::left << std::setw(22) << "Solar Power: " 
                  << std::right << std::setw(5) << std::fixed << std::setprecision(2) << solar_out_power
                  << " W" 
                  << std::endl;
        std::cout << NORMAL_CONSOLE_TEXT 
                  << "Battery State of Charge:  \n[";  
                    while (SOC_count<batt_SOC){
                        std::cout << NORMAL_CONSOLE_TEXT
                                << "▉";
                        SOC_count+=5;
                    }
                    while (SOC_count<100){
                        std::cout << NORMAL_CONSOLE_TEXT
                                << " ";
                        SOC_count+=5;
                    }
        std::cout << "] " << batt_SOC << "%  \n";
                    if (batt_power>solar_out_power){
                        std::cout << ERROR_CONSOLE_TEXT << "Discharging" << std::endl;
                    }
                    else if (batt_power<solar_out_power){
                        std::cout << CHARGING_TEXT << "Charging" << std::endl;
                    }
                    else {
                        std::cout << std::endl;
                    }
                  
        sleep_for(milliseconds(500));
    }
/*
    // Arm vehicle
    std::cout << "Arming..." << std::endl;
    const Action::Result arm_result = action->arm();

    if (arm_result != Action::Result::SUCCESS) {
        std::cout << ERROR_CONSOLE_TEXT << "Arming failed:" << Action::result_str(arm_result)
                  << NORMAL_CONSOLE_TEXT << std::endl;
        return 1;
    }

    // Take off
    std::cout << "Taking off..." << std::endl;
    const Action::Result takeoff_result = action->takeoff();
    if (takeoff_result != Action::Result::SUCCESS) {
        std::cout << ERROR_CONSOLE_TEXT << "Takeoff failed:" << Action::result_str(takeoff_result)
                  << NORMAL_CONSOLE_TEXT << std::endl;
        return 1;
    }

    // Let it hover for a bit before landing again.
    sleep_for(seconds(10));

    std::cout << "Landing..." << std::endl;
    const Action::Result land_result = action->land();
    if (land_result != Action::Result::SUCCESS) {
        std::cout << ERROR_CONSOLE_TEXT << "Land failed:" << Action::result_str(land_result)
                  << NORMAL_CONSOLE_TEXT << std::endl;
        return 1;
    }

    // Check if vehicle is still in air
    while (telemetry->in_air()) {
        std::cout << "Vehicle is landing..." << std::endl;
        sleep_for(seconds(1));
    }
    std::cout << "Landed!" << std::endl;
*/
    // We are relying on auto-disarming but let's keep watching the telemetry for a bit longer.
    sleep_for(seconds(3));
    std::cout << "Finished..." << std::endl;

    return 0;
}
