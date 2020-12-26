#include <iostream>
#include <iomanip>
#include <ctime>
#include "integration_test_helper.h"
#include "mavsdk.h"
#include "plugins/telemetry/telemetry.h"

#define CAMERA_AVAILABLE 0 // Set to 1 if camera is available and should be tested.

using namespace std::placeholders; // for `_1`
using namespace mavsdk;

static void receive_result(Telemetry::Result result);
static void print_position(Telemetry::Position position);
static void print_home_position(Telemetry::Position home_position);
static void print_in_air(bool in_air);
static void print_armed(bool armed);
static void print_quaternion(Telemetry::Quaternion quaternion);
static void print_euler_angle(Telemetry::EulerAngle euler_angle);
static void print_angular_velocity_body(Telemetry::AngularVelocityBody angular_velocity_body);
#if CAMERA_AVAILABLE == 1
static void print_camera_quaternion(Telemetry::Quaternion quaternion);
static void print_camera_euler_angle(Telemetry::EulerAngle euler_angle);
#endif
static void print_ground_speed_ned(Telemetry::GroundSpeedNED ground_speed_ned);
static void print_imu_reading_ned(Telemetry::IMUReadingNED imu_reading_ned);
static void print_gps_info(Telemetry::GPSInfo gps_info);
static void print_battery(Telemetry::Battery battery);
static void print_battery_current(Telemetry::Battery_Current battery_current);
static void print_rc_status(Telemetry::RCStatus rc_status);
static void print_position_velocity_ned(Telemetry::PositionVelocityNED position_velocity_ned);
static void print_unix_epoch_time_us(uint64_t time_us);
static void print_actuator_control_target(Telemetry::ActuatorControlTarget actuator_control_target);
static void print_actuator_output_status(Telemetry::ActuatorOutputStatus actuator_output_status);

static bool _set_rate_error = false;
static bool _received_position = false;
static bool _received_home_position = false;
static bool _received_in_air = false;
static bool _received_armed = false;
static bool _received_quaternion = false;
static bool _received_euler_angle = false;
static bool _received_angular_velocity_body = false;
#if CAMERA_AVAILABLE == 1
static bool _received_camera_quaternion = false;
static bool _received_camera_euler_angle = false;
#endif
static bool _received_ground_speed = false;
static bool _received_imu_reading_ned = false;
static bool _received_gps_info = false;
static bool _received_battery = false;
static bool _received_battery_current = false;
static bool _received_rc_status = false;
static bool _received_position_velocity_ned = false;
static bool _received_actuator_control_target = false;
static bool _received_actuator_output_status = false;

TEST_F(SitlTest, TelemetryAsync)
{
    Mavsdk dc;

    ConnectionResult ret = dc.add_udp_connection();

    std::this_thread::sleep_for(std::chrono::seconds(3));

    std::vector<uint64_t> uuids = dc.system_uuids();

    for (auto it = uuids.begin(); it != uuids.end(); ++it) {
        std::cout << "found system with UUID: " << *it << std::endl;
    }

    ASSERT_EQ(uuids.size(), 1);

    uint64_t uuid = uuids.at(0);

    ASSERT_EQ(ret, ConnectionResult::SUCCESS);

    System& system = dc.system(uuid);

    auto telemetry = std::make_shared<Telemetry>(system);

    telemetry->set_rate_position_async(10.0, std::bind(&receive_result, _1));
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    telemetry->set_rate_home_position_async(10.0, std::bind(&receive_result, _1));
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    telemetry->set_rate_in_air_async(10.0, std::bind(&receive_result, _1));
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    telemetry->set_rate_attitude_async(10.0, std::bind(&receive_result, _1));
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    telemetry->set_rate_ground_speed_ned_async(10.0, std::bind(&receive_result, _1));
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    telemetry->set_rate_imu_reading_ned_async(10.0, std::bind(&receive_result, _1));
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    telemetry->set_rate_gps_info_async(10.0, std::bind(&receive_result, _1));
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    telemetry->set_rate_battery_async(10.0, std::bind(&receive_result, _1));
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    telemetry->set_rate_battery_current_async(10.0, std::bind(&receive_result, _1));
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    telemetry->set_rate_actuator_control_target_async(10.0, std::bind(&receive_result, _1));
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    telemetry->position_async(std::bind(&print_position, _1));

    telemetry->home_position_async(std::bind(&print_home_position, _1));

    telemetry->in_air_async(std::bind(&print_in_air, _1));

    telemetry->armed_async(std::bind(&print_armed, _1));

    telemetry->attitude_quaternion_async(std::bind(&print_quaternion, _1));

    telemetry->attitude_euler_angle_async(std::bind(&print_euler_angle, _1));

    telemetry->attitude_angular_velocity_body_async(std::bind(&print_angular_velocity_body, _1));

#if CAMERA_AVAILABLE == 1
    telemetry->camera_attitude_quaternion_async(std::bind(&print_camera_quaternion, _1));

    telemetry->camera_attitude_euler_angle_async(std::bind(&print_camera_euler_angle, _1));
#endif

    telemetry->ground_speed_ned_async(std::bind(&print_ground_speed_ned, _1));

    telemetry->imu_reading_ned_async(std::bind(&print_imu_reading_ned, _1));

    telemetry->gps_info_async(std::bind(&print_gps_info, _1));

    telemetry->battery_async(std::bind(&print_battery, _1));

    telemetry->battery_current_async(std::bind(&print_battery_current, _1));

    telemetry->rc_status_async(std::bind(&print_rc_status, _1));

    telemetry->position_velocity_ned_async(std::bind(&print_position_velocity_ned, _1));

    telemetry->unix_epoch_time_async(std::bind(&print_unix_epoch_time_us, _1));

    telemetry->actuator_control_target_async(std::bind(&print_actuator_control_target, _1));

    telemetry->actuator_output_status_async(std::bind(&print_actuator_output_status, _1));

    std::this_thread::sleep_for(std::chrono::seconds(10));

    EXPECT_FALSE(_set_rate_error);
    EXPECT_TRUE(_received_position);
    EXPECT_TRUE(_received_home_position);
    EXPECT_TRUE(_received_in_air);
    EXPECT_TRUE(_received_armed);
    EXPECT_TRUE(_received_quaternion);
    EXPECT_TRUE(_received_angular_velocity_body);
    EXPECT_TRUE(_received_euler_angle);
#if CAMERA_AVAILABLE == 1
    EXPECT_TRUE(_received_camera_quaternion);
    EXPECT_TRUE(_received_camera_euler_angle);
#endif
    EXPECT_TRUE(_received_ground_speed);
    EXPECT_TRUE(_received_imu_reading_ned);
    EXPECT_TRUE(_received_gps_info);
    EXPECT_TRUE(_received_battery);
    EXPECT_TRUE(_received_battery_current);
    // EXPECT_TRUE(_received_rc_status); // No RC is sent in SITL.
    EXPECT_TRUE(_received_position_velocity_ned);
    // EXPECT_TRUE(_received_actuator_control_target); TODO check is that sent in SITL.
    // EXPECT_TRUE(_received_actuator_output_status); TODO check is that sent in SITL.
}

void receive_result(Telemetry::Result result)
{
    if (result != Telemetry::Result::SUCCESS) {
        _set_rate_error = true;
        std::cerr << "Received ret: " << int(result) << std::endl;
        EXPECT_TRUE(false);
    }
}

void print_position(Telemetry::Position position)
{
    std::cout << "Got position, lat: " << position.latitude_deg << " deg, "
              << "lon: " << position.longitude_deg << " deg, "
              << "relative alt: " << position.relative_altitude_m << " m" << std::endl;
    _received_position = true;
}

void print_home_position(Telemetry::Position home_position)
{
    std::cout << "Got home position, lat: " << home_position.latitude_deg << " deg, "
              << "lon: " << home_position.longitude_deg << " deg, "
              << "relative alt: " << home_position.relative_altitude_m << " m" << std::endl;
    _received_home_position = true;
}

void print_in_air(bool in_air)
{
    std::cout << (in_air ? "in-air" : "on-ground") << std::endl;
    _received_in_air = true;
}

void print_armed(bool armed)
{
    std::cout << (armed ? "armed" : "disarmed") << std::endl;
    _received_armed = true;
}

void print_quaternion(Telemetry::Quaternion quaternion)
{
    std::cout << "Quaternion: [ " << quaternion.w << ", " << quaternion.x << ", " << quaternion.y
              << ", " << quaternion.z << " ]" << std::endl;

    _received_quaternion = true;
}

void print_euler_angle(Telemetry::EulerAngle euler_angle)
{
    std::cout << "Euler angle: [ " << euler_angle.roll_deg << ", " << euler_angle.pitch_deg << ", "
              << euler_angle.yaw_deg << " ] deg" << std::endl;

    _received_euler_angle = true;
}

void print_angular_velocity_body(Telemetry::AngularVelocityBody angular_velocity_body)
{
    std::cout << "Angular velocity: [ " << angular_velocity_body.roll_rad_s << ", "
              << angular_velocity_body.pitch_rad_s << ", " << angular_velocity_body.yaw_rad_s
              << " ] rad/s" << std::endl;

    _received_angular_velocity_body = true;
}

#if CAMERA_AVAILABLE == 1
void print_camera_quaternion(Telemetry::Quaternion quaternion)
{
    std::cout << "Camera Quaternion: [ " << quaternion.w << ", " << quaternion.x << ", "
              << quaternion.y << ", " << quaternion.z << " ]" << std::endl;

    _received_camera_quaternion = true;
}

void print_camera_euler_angle(Telemetry::EulerAngle euler_angle)
{
    std::cout << "Camera Euler angle: [ " << euler_angle.roll_deg << ", " << euler_angle.pitch_deg
              << ", " << euler_angle.yaw_deg << " ] deg" << std::endl;

    _received_camera_euler_angle = true;
}
#endif

void print_ground_speed_ned(Telemetry::GroundSpeedNED ground_speed_ned)
{
    std::cout << "Ground speed NED: [ " << ground_speed_ned.velocity_north_m_s << ", "
              << ground_speed_ned.velocity_east_m_s << ", " << ground_speed_ned.velocity_down_m_s
              << " ]" << std::endl;

    _received_ground_speed = true;
}

void print_imu_reading_ned(Telemetry::IMUReadingNED imu_reading_ned)
{
    std::cout << "Acceleration north: " << imu_reading_ned.acceleration.north_m_s2 << " m/s^2, "
              << "east: " << imu_reading_ned.acceleration.east_m_s2 << " m/s^2, "
              << "down: " << imu_reading_ned.acceleration.down_m_s2 << " m/s^2, "
              << "Angular velocity north: " << imu_reading_ned.angular_velocity.north_rad_s
              << " rad/s, "
              << "east: " << imu_reading_ned.angular_velocity.east_rad_s << " rad/s, "
              << "down: " << imu_reading_ned.angular_velocity.down_rad_s << " rad/s, "
              << "Magnetic field north: " << imu_reading_ned.magnetic_field.north_gauss << " G, "
              << "east: " << imu_reading_ned.magnetic_field.east_gauss << " G, "
              << "down: " << imu_reading_ned.magnetic_field.down_gauss << " G, "
              << "Temperature: " << imu_reading_ned.temperature_degC << " C" << std::endl;

    _received_imu_reading_ned = true;
}

void print_gps_info(Telemetry::GPSInfo gps_info)
{
    std::cout << "GPS, num satellites: " << gps_info.num_satellites << ", "
              << "fix type: " << gps_info.fix_type << std::endl;

    _received_gps_info = true;
}

void print_battery(Telemetry::Battery battery)
{
    std::cout << "Battery: " << battery.voltage_v << " v,"
              << "remaining: " << int(battery.remaining_percent * 1e2f) << " %" << std::endl;

    _received_battery = true;
}

void print_battery_current(Telemetry::Battery_Current battery_current)
{
    std::cout << "Battery_Current: " << battery_current.current_A << " A" << std::endl;

    _received_battery_current = true;
}

void print_rc_status(Telemetry::RCStatus rc_status)
{
    std::cout << "RC status [ RSSI: " << rc_status.signal_strength_percent * 100 << "]"
              << std::endl;
    _received_rc_status = true;
}

void print_position_velocity_ned(Telemetry::PositionVelocityNED position_velocity_ned)
{
    std::cout << "Got position north:  " << position_velocity_ned.position.north_m << " m, "
              << "east: " << position_velocity_ned.position.east_m << " m, "
              << "down: " << position_velocity_ned.position.down_m << " m" << std::endl
              << "velocity north: " << position_velocity_ned.velocity.north_m_s << " m/s, "
              << "velocity east: " << position_velocity_ned.velocity.east_m_s << " m/s, "
              << "velocity down: " << position_velocity_ned.velocity.down_m_s << " m/s"
              << std::endl;

    _received_position_velocity_ned = true;
}

void print_unix_epoch_time_us(uint64_t time_us)
{
    std::time_t time = time_us / 10E5;
#if __GNUC__ > 4
    LogInfo() << "UTC (" << time << "): " << std::put_time(std::gmtime(&time), "%c %Z");
#else
    // TODO: remove this once we don't have to build with gcc 4 any longer.
    char time_string[24];
    if (0 < strftime(time_string, sizeof(time_string), "%c %Z", std::gmtime(&time)))
        LogInfo() << time_string;
#endif
}

static void print_actuator_control_target(Telemetry::ActuatorControlTarget actuator_control_target)
{
    std::cout << "Group:  " << static_cast<int>(actuator_control_target.group) << ", Controls: [";
    for (int i = 0; i < 8; i++) {
        std::cout << actuator_control_target.controls[i];
        if (i != 7) {
            std::cout << ", ";
        } else {
            std::cout << "]" << std::endl;
        }
    }

    _received_actuator_control_target = true;
}

static void print_actuator_output_status(Telemetry::ActuatorOutputStatus actuator_output_status)
{
    std::cout << "Active:  " << actuator_output_status.active << ", Actuators: [";
    for (unsigned i = 0; i < actuator_output_status.active; i++) {
        std::cout << actuator_output_status.actuator[i];
        if (i != (actuator_output_status.active - 1)) {
            std::cout << ", ";
        } else {
            std::cout << "]" << std::endl;
        }
    }

    _received_actuator_output_status = true;
}