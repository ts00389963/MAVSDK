#include <iostream>
#include "integration_test_helper.h"
#include "mavsdk.h"
#include "plugins/telemetry/telemetry.h"

using namespace mavsdk;

TEST_F(SitlTest, TelemetrySync)
{
    Mavsdk mavsdk;

    ConnectionResult ret = mavsdk.add_udp_connection();
    ASSERT_EQ(ret, ConnectionResult::SUCCESS);

    std::this_thread::sleep_for(std::chrono::seconds(2));
    System& system = mavsdk.system();
    ASSERT_TRUE(system.is_connected());

    auto telemetry = std::make_shared<Telemetry>(system);

    EXPECT_EQ(telemetry->set_rate_position(10.0), Telemetry::Result::SUCCESS);
    EXPECT_EQ(telemetry->set_rate_home_position(10.0), Telemetry::Result::SUCCESS);
    EXPECT_EQ(telemetry->set_rate_in_air(10.0), Telemetry::Result::SUCCESS);
    EXPECT_EQ(telemetry->set_rate_attitude(10.0), Telemetry::Result::SUCCESS);
    EXPECT_EQ(telemetry->set_rate_ground_speed_ned(10.0), Telemetry::Result::SUCCESS);
    EXPECT_EQ(telemetry->set_rate_gps_info(10.0), Telemetry::Result::SUCCESS);
    EXPECT_EQ(telemetry->set_rate_battery(10.0), Telemetry::Result::SUCCESS);
    EXPECT_EQ(telemetry->set_rate_battery_current(10.0), Telemetry::Result::SUCCESS);
    EXPECT_EQ(telemetry->set_rate_actuator_control_target(10.0), Telemetry::Result::SUCCESS);

    for (unsigned i = 0; i < 10; ++i) {
        std::cout << "Position: " << telemetry->position() << std::endl;
        std::cout << "Home Position: " << telemetry->home_position() << std::endl;
        std::cout << "Attitude: " << telemetry->attitude_quaternion() << std::endl;
        std::cout << "Attitude: " << telemetry->attitude_euler_angle() << std::endl;
        std::cout << "Angular velocity: " << telemetry->attitude_angular_velocity_body()
                  << std::endl;
        std::cout << "Ground speed: " << telemetry->ground_speed_ned() << std::endl;
        std::cout << "GPS Info: " << telemetry->gps_info() << std::endl;
        std::cout << "Battery: " << telemetry->battery() << std::endl;
        std::cout << "Battery_Current: " << telemetry->battery_current() << std::endl;
        std::cout << "Actuators: " << telemetry->actuator_control_target() << std::endl;
        std::cout << "Flight mode: " << telemetry->flight_mode() << std::endl;
        std::cout << "Landed state: " << telemetry->landed_state()
                  << "(in air: " << telemetry->in_air() << ")" << std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}
