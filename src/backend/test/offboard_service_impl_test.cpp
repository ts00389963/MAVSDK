#include <gmock/gmock.h>
#include <string>
#include <vector>

#include "offboard/offboard_service_impl.h"
#include "offboard/mocks/offboard_mock.h"

namespace {

using testing::_;
using testing::NiceMock;
using testing::Return;

using MockOffboard = NiceMock<mavsdk::testing::MockOffboard>;
using OffboardServiceImpl = mavsdk::backend::OffboardServiceImpl<MockOffboard>;
using OffboardResult = mavsdk::rpc::offboard::OffboardResult;
using InputPair = std::pair<std::string, mavsdk::Offboard::Result>;

static constexpr float ARBITRARY_ACTUATOR_CONTROL_0 = -0.42f;
static constexpr float ARBITRARY_ACTUATOR_CONTROL_1 = 0.15f;
static constexpr float ARBITRARY_ACTUATOR_CONTROL_2 = 0.56f;
static constexpr float ARBITRARY_ACTUATOR_CONTROL_3 = -0.95f;
static constexpr float ARBITRARY_ACTUATOR_CONTROL_4 = 0.34f;
static constexpr float ARBITRARY_ACTUATOR_CONTROL_5 = 0.98f;
static constexpr float ARBITRARY_ACTUATOR_CONTROL_6 = -0.15f;
static constexpr float ARBITRARY_ACTUATOR_CONTROL_7 = 0.15f;
static constexpr float ARBITRARY_ROLL = 25.0f;
static constexpr float ARBITRARY_PITCH = 40.0f;
static constexpr float ARBITRARY_YAW = 37.0f;
static constexpr float ARBITRARY_ROLL_RATE = 2.5f;
static constexpr float ARBITRARY_PITCH_RATE = 4.0f;
static constexpr float ARBITRARY_YAW_RATE = 3.7f;
static constexpr float ARBITRARY_THRUST = 0.5f;
static constexpr float ARBITRARY_NORTH_M = 10.54f;
static constexpr float ARBITRARY_EAST_M = 5.62f;
static constexpr float ARBITRARY_DOWN_M = 1.44f;
static constexpr float ARBITRARY_VELOCITY_LOW = 1.7f;
static constexpr float ARBITRARY_VELOCITY_MID = 7.3f;
static constexpr float ARBITRARY_VELOCITY_HIGH = 14.6f;
static constexpr float ARBITRARY_VELOCITY_NEG = -0.5f;
static constexpr float ARBITRARY_YAWSPEED = 3.1f;

std::vector<InputPair> generateInputPairs();
std::string startAndGetTranslatedResult(mavsdk::Offboard::Result start_result);
std::string stopAndGetTranslatedResult(mavsdk::Offboard::Result stop_result);

class OffboardServiceImplTest : public ::testing::TestWithParam<InputPair> {
protected:
    void checkReturnsCorrectIsActiveStatus(const bool expected_is_active_status);

    std::unique_ptr<mavsdk::rpc::offboard::Attitude> createArbitraryRPCAttitude() const;
    std::unique_ptr<mavsdk::rpc::offboard::AttitudeRate> createArbitraryRPCAttitudeRate() const;
    std::unique_ptr<mavsdk::rpc::offboard::PositionNedYaw> createArbitraryRPCPositionNedYaw() const;
    std::unique_ptr<mavsdk::rpc::offboard::VelocityBodyYawspeed>
    createArbitraryRPCVelocityBodyYawspeed() const;
    std::unique_ptr<mavsdk::rpc::offboard::VelocityNedYaw> createArbitraryRPCVelocityNedYaw() const;
    std::unique_ptr<mavsdk::rpc::offboard::ActuatorControl>
    createArbitraryRPCActuatorControl() const;
};

TEST_P(OffboardServiceImplTest, startResultIsTranslatedCorrectly)
{
    const auto rpc_result = startAndGetTranslatedResult(GetParam().second);
    EXPECT_EQ(rpc_result, GetParam().first);
}

std::string startAndGetTranslatedResult(const mavsdk::Offboard::Result start_result)
{
    MockOffboard offboard;
    ON_CALL(offboard, start()).WillByDefault(Return(start_result));
    OffboardServiceImpl offboardService(offboard);
    mavsdk::rpc::offboard::StartResponse response;

    offboardService.Start(nullptr, nullptr, &response);

    return OffboardResult::Result_Name(response.offboard_result().result());
}

TEST_F(OffboardServiceImplTest, startsEvenWhenArgsAreNull)
{
    MockOffboard offboard;
    OffboardServiceImpl offboardService(offboard);
    EXPECT_CALL(offboard, start()).Times(1);

    offboardService.Start(nullptr, nullptr, nullptr);
}

TEST_P(OffboardServiceImplTest, stopResultIsTranslatedCorrectly)
{
    const auto rpc_result = stopAndGetTranslatedResult(GetParam().second);
    EXPECT_EQ(rpc_result, GetParam().first);
}

std::string stopAndGetTranslatedResult(const mavsdk::Offboard::Result stop_result)
{
    MockOffboard offboard;
    ON_CALL(offboard, stop()).WillByDefault(Return(stop_result));
    OffboardServiceImpl offboardService(offboard);
    mavsdk::rpc::offboard::StopResponse response;

    offboardService.Stop(nullptr, nullptr, &response);

    return OffboardResult::Result_Name(response.offboard_result().result());
}

TEST_F(OffboardServiceImplTest, stopsEvenWhenArgsAreNull)
{
    MockOffboard offboard;
    OffboardServiceImpl offboardService(offboard);
    EXPECT_CALL(offboard, stop()).Times(1);

    offboardService.Stop(nullptr, nullptr, nullptr);
}

TEST_F(OffboardServiceImplTest, isActiveCallsGetter)
{
    MockOffboard offboard;
    OffboardServiceImpl offboardService(offboard);
    EXPECT_CALL(offboard, is_active()).Times(1);
    mavsdk::rpc::offboard::IsActiveResponse response;

    offboardService.IsActive(nullptr, nullptr, &response);
}

TEST_F(OffboardServiceImplTest, isActiveGetsCorrectValue)
{
    checkReturnsCorrectIsActiveStatus(false);
    checkReturnsCorrectIsActiveStatus(true);
}

void OffboardServiceImplTest::checkReturnsCorrectIsActiveStatus(
    const bool expected_is_active_status)
{
    MockOffboard offboard;
    OffboardServiceImpl offboardService(offboard);
    ON_CALL(offboard, is_active()).WillByDefault(Return(expected_is_active_status));
    mavsdk::rpc::offboard::IsActiveResponse response;

    offboardService.IsActive(nullptr, nullptr, &response);

    EXPECT_EQ(expected_is_active_status, response.is_active());
}

TEST_F(OffboardServiceImplTest, isActiveDoesNotCrashWithNullResponse)
{
    MockOffboard offboard;
    OffboardServiceImpl offboardService(offboard);

    offboardService.IsActive(nullptr, nullptr, nullptr);
}

TEST_F(OffboardServiceImplTest, setAttitudeDoesNotFailWithAllNullParams)
{
    MockOffboard offboard;
    OffboardServiceImpl offboardService(offboard);

    offboardService.SetAttitude(nullptr, nullptr, nullptr);
}

TEST_F(OffboardServiceImplTest, setAttitudeRateDoesNotFailWithAllNullParams)
{
    MockOffboard offboard;
    OffboardServiceImpl offboardService(offboard);

    offboardService.SetAttitudeRate(nullptr, nullptr, nullptr);
}

TEST_F(OffboardServiceImplTest, setActuatorControlDoesNotFailWithAllNullParams)
{
    MockOffboard offboard;
    OffboardServiceImpl offboardService(offboard);

    offboardService.SetActuatorControl(nullptr, nullptr, nullptr);
}

TEST_F(OffboardServiceImplTest, setActuatorControlDoesNotFailWithNullResponse)
{
    MockOffboard offboard;
    OffboardServiceImpl offboardService(offboard);
    mavsdk::rpc::offboard::SetActuatorControlRequest request;

    auto rpc_actuator_control = createArbitraryRPCActuatorControl();
    request.set_allocated_actuator_control(rpc_actuator_control.release());

    offboardService.SetActuatorControl(nullptr, &request, nullptr);
}

TEST_F(OffboardServiceImplTest, setsActuatorControlCorrectly)
{
    MockOffboard offboard;
    OffboardServiceImpl offboardService(offboard);
    mavsdk::rpc::offboard::SetActuatorControlRequest request;

    auto rpc_actuator_control = createArbitraryRPCActuatorControl();
    const auto expected_actuator_control =
        OffboardServiceImpl::translateRPCActuatorControl(*rpc_actuator_control);
    EXPECT_CALL(offboard, set_actuator_control(expected_actuator_control)).Times(1);

    request.set_allocated_actuator_control(rpc_actuator_control.release());

    offboardService.SetActuatorControl(nullptr, &request, nullptr);
}

std::unique_ptr<mavsdk::rpc::offboard::ActuatorControl>
OffboardServiceImplTest::createArbitraryRPCActuatorControl() const
{
    auto rpc_actuator_control = std::make_unique<mavsdk::rpc::offboard::ActuatorControl>(
        mavsdk::rpc::offboard::ActuatorControl());

    auto rpc_actuator_group_0 = rpc_actuator_control.get()->add_groups();

    rpc_actuator_group_0->add_controls(ARBITRARY_ACTUATOR_CONTROL_0);
    rpc_actuator_group_0->add_controls(ARBITRARY_ACTUATOR_CONTROL_1);
    rpc_actuator_group_0->add_controls(ARBITRARY_ACTUATOR_CONTROL_2);
    rpc_actuator_group_0->add_controls(ARBITRARY_ACTUATOR_CONTROL_3);
    rpc_actuator_group_0->add_controls(ARBITRARY_ACTUATOR_CONTROL_4);
    rpc_actuator_group_0->add_controls(ARBITRARY_ACTUATOR_CONTROL_5);
    rpc_actuator_group_0->add_controls(ARBITRARY_ACTUATOR_CONTROL_6);
    rpc_actuator_group_0->add_controls(ARBITRARY_ACTUATOR_CONTROL_7);

    auto rpc_actuator_group_1 = rpc_actuator_control.get()->add_groups();

    rpc_actuator_group_1->add_controls(ARBITRARY_ACTUATOR_CONTROL_0 + 0.01f);
    rpc_actuator_group_1->add_controls(ARBITRARY_ACTUATOR_CONTROL_1 + 0.02f);
    rpc_actuator_group_1->add_controls(ARBITRARY_ACTUATOR_CONTROL_2 + 0.03f);
    rpc_actuator_group_1->add_controls(ARBITRARY_ACTUATOR_CONTROL_3 + 0.04f);
    rpc_actuator_group_1->add_controls(ARBITRARY_ACTUATOR_CONTROL_4 + 0.05f);
    rpc_actuator_group_1->add_controls(ARBITRARY_ACTUATOR_CONTROL_5 + 0.06f);
    rpc_actuator_group_1->add_controls(ARBITRARY_ACTUATOR_CONTROL_6 + 0.07f);
    rpc_actuator_group_1->add_controls(ARBITRARY_ACTUATOR_CONTROL_7 + 0.08f);

    return rpc_actuator_control;
}

TEST_F(OffboardServiceImplTest, setAttitudeDoesNotFailWithNullResponse)
{
    MockOffboard offboard;
    OffboardServiceImpl offboardService(offboard);
    mavsdk::rpc::offboard::SetAttitudeRequest request;

    auto rpc_attitude = createArbitraryRPCAttitude();
    request.set_allocated_attitude(rpc_attitude.release());

    offboardService.SetAttitude(nullptr, &request, nullptr);
}

TEST_F(OffboardServiceImplTest, setAttitudeRateDoesNotFailWithNullResponse)
{
    MockOffboard offboard;
    OffboardServiceImpl offboardService(offboard);
    mavsdk::rpc::offboard::SetAttitudeRateRequest request;

    auto rpc_attitude_rate = createArbitraryRPCAttitudeRate();
    request.set_allocated_attitude_rate(rpc_attitude_rate.release());

    offboardService.SetAttitudeRate(nullptr, &request, nullptr);
}

std::unique_ptr<mavsdk::rpc::offboard::Attitude>
OffboardServiceImplTest::createArbitraryRPCAttitude() const
{
    auto rpc_attitude =
        std::make_unique<mavsdk::rpc::offboard::Attitude>(mavsdk::rpc::offboard::Attitude());
    rpc_attitude->set_roll_deg(ARBITRARY_ROLL);
    rpc_attitude->set_pitch_deg(ARBITRARY_PITCH);
    rpc_attitude->set_yaw_deg(ARBITRARY_YAW);
    rpc_attitude->set_thrust_value(ARBITRARY_THRUST);

    return rpc_attitude;
}
std::unique_ptr<mavsdk::rpc::offboard::AttitudeRate>
OffboardServiceImplTest::createArbitraryRPCAttitudeRate() const
{
    auto rpc_attitude_rate = std::make_unique<mavsdk::rpc::offboard::AttitudeRate>(
        mavsdk::rpc::offboard::AttitudeRate());
    rpc_attitude_rate->set_roll_deg_s(ARBITRARY_ROLL_RATE);
    rpc_attitude_rate->set_pitch_deg_s(ARBITRARY_PITCH_RATE);
    rpc_attitude_rate->set_yaw_deg_s(ARBITRARY_YAW_RATE);
    rpc_attitude_rate->set_thrust_value(ARBITRARY_THRUST);

    return rpc_attitude_rate;
}

TEST_F(OffboardServiceImplTest, setsAttitudeCorrectly)
{
    MockOffboard offboard;
    OffboardServiceImpl offboardService(offboard);
    mavsdk::rpc::offboard::SetAttitudeRequest request;

    auto rpc_attitude = createArbitraryRPCAttitude();
    const auto expected_attitude = OffboardServiceImpl::translateRPCAttitude(*rpc_attitude);
    EXPECT_CALL(offboard, set_attitude(expected_attitude)).Times(1);

    request.set_allocated_attitude(rpc_attitude.release());

    offboardService.SetAttitude(nullptr, &request, nullptr);
}

TEST_F(OffboardServiceImplTest, setsAttitudeRateCorrectly)
{
    MockOffboard offboard;
    OffboardServiceImpl offboardService(offboard);
    mavsdk::rpc::offboard::SetAttitudeRateRequest request;

    auto rpc_attitude_rate = createArbitraryRPCAttitudeRate();
    const auto expected_attitude_rate =
        OffboardServiceImpl::translateRPCAttitudeRate(*rpc_attitude_rate);
    EXPECT_CALL(offboard, set_attitude_rate(expected_attitude_rate)).Times(1);

    request.set_allocated_attitude_rate(rpc_attitude_rate.release());

    offboardService.SetAttitudeRate(nullptr, &request, nullptr);
}

TEST_F(OffboardServiceImplTest, setPositionNedYawDoesNotFailWithAllNullParams)
{
    MockOffboard offboard;
    OffboardServiceImpl offboardService(offboard);

    offboardService.SetPositionNed(nullptr, nullptr, nullptr);
}

TEST_F(OffboardServiceImplTest, setPositionNedYawDoesNotFailWithNullResponse)
{
    MockOffboard offboard;
    OffboardServiceImpl offboardService(offboard);
    mavsdk::rpc::offboard::SetPositionNedRequest request;

    auto rpc_position_ned_yaw = createArbitraryRPCPositionNedYaw();
    request.set_allocated_position_ned_yaw(rpc_position_ned_yaw.release());

    offboardService.SetPositionNed(nullptr, &request, nullptr);
}

std::unique_ptr<mavsdk::rpc::offboard::PositionNedYaw>
OffboardServiceImplTest::createArbitraryRPCPositionNedYaw() const
{
    auto rpc_position_ned_yaw = std::make_unique<mavsdk::rpc::offboard::PositionNedYaw>(
        mavsdk::rpc::offboard::PositionNedYaw());
    rpc_position_ned_yaw->set_north_m(ARBITRARY_NORTH_M);
    rpc_position_ned_yaw->set_east_m(ARBITRARY_EAST_M);
    rpc_position_ned_yaw->set_down_m(ARBITRARY_DOWN_M);
    rpc_position_ned_yaw->set_yaw_deg(ARBITRARY_YAW);

    return rpc_position_ned_yaw;
}

TEST_F(OffboardServiceImplTest, setsPositionNedYawCorrectly)
{
    MockOffboard offboard;
    OffboardServiceImpl offboardService(offboard);
    mavsdk::rpc::offboard::SetPositionNedRequest request;

    auto rpc_position_ned_yaw = createArbitraryRPCPositionNedYaw();
    const auto expected_position_ned_yaw =
        OffboardServiceImpl::translateRPCPositionNedYaw(*rpc_position_ned_yaw);
    EXPECT_CALL(offboard, set_position_ned(expected_position_ned_yaw)).Times(1);

    request.set_allocated_position_ned_yaw(rpc_position_ned_yaw.release());

    offboardService.SetPositionNed(nullptr, &request, nullptr);
}

TEST_F(OffboardServiceImplTest, setVelocityBodyDoesNotFailWithAllNullParams)
{
    MockOffboard offboard;
    OffboardServiceImpl offboardService(offboard);

    offboardService.SetVelocityBody(nullptr, nullptr, nullptr);
}

TEST_F(OffboardServiceImplTest, setVelocityBodyDoesNotFailWithNullResponse)
{
    MockOffboard offboard;
    OffboardServiceImpl offboardService(offboard);
    mavsdk::rpc::offboard::SetVelocityBodyRequest request;

    auto rpc_velocity_body = createArbitraryRPCVelocityBodyYawspeed();
    request.set_allocated_velocity_body_yawspeed(rpc_velocity_body.release());

    offboardService.SetVelocityBody(nullptr, &request, nullptr);
}

std::unique_ptr<mavsdk::rpc::offboard::VelocityBodyYawspeed>
OffboardServiceImplTest::createArbitraryRPCVelocityBodyYawspeed() const
{
    auto rpc_velocity_body = std::make_unique<mavsdk::rpc::offboard::VelocityBodyYawspeed>(
        mavsdk::rpc::offboard::VelocityBodyYawspeed());
    rpc_velocity_body->set_forward_m_s(ARBITRARY_VELOCITY_HIGH);
    rpc_velocity_body->set_right_m_s(ARBITRARY_VELOCITY_LOW);
    rpc_velocity_body->set_down_m_s(ARBITRARY_VELOCITY_NEG);
    rpc_velocity_body->set_yawspeed_deg_s(ARBITRARY_YAWSPEED);

    return rpc_velocity_body;
}

TEST_F(OffboardServiceImplTest, setsVelocityBodyCorrectly)
{
    MockOffboard offboard;
    OffboardServiceImpl offboardService(offboard);
    mavsdk::rpc::offboard::SetVelocityBodyRequest request;

    auto rpc_velocity_body = createArbitraryRPCVelocityBodyYawspeed();
    const auto expected_velocity_body =
        OffboardServiceImpl::translateRPCVelocityBodyYawspeed(*rpc_velocity_body);
    EXPECT_CALL(offboard, set_velocity_body(expected_velocity_body)).Times(1);

    request.set_allocated_velocity_body_yawspeed(rpc_velocity_body.release());

    offboardService.SetVelocityBody(nullptr, &request, nullptr);
}

TEST_F(OffboardServiceImplTest, setVelocityNedDoesNotFailWithAllNullParams)
{
    MockOffboard offboard;
    OffboardServiceImpl offboardService(offboard);

    offboardService.SetVelocityNed(nullptr, nullptr, nullptr);
}

TEST_F(OffboardServiceImplTest, setVelocityNedDoesNotFailWithNullResponse)
{
    MockOffboard offboard;
    OffboardServiceImpl offboardService(offboard);
    mavsdk::rpc::offboard::SetVelocityNedRequest request;

    auto rpc_velocity_ned = createArbitraryRPCVelocityNedYaw();
    request.set_allocated_velocity_ned_yaw(rpc_velocity_ned.release());

    offboardService.SetVelocityNed(nullptr, &request, nullptr);
}

std::unique_ptr<mavsdk::rpc::offboard::VelocityNedYaw>
OffboardServiceImplTest::createArbitraryRPCVelocityNedYaw() const
{
    auto rpc_velocity_ned = std::make_unique<mavsdk::rpc::offboard::VelocityNedYaw>(
        mavsdk::rpc::offboard::VelocityNedYaw());
    rpc_velocity_ned->set_north_m_s(ARBITRARY_VELOCITY_MID);
    rpc_velocity_ned->set_east_m_s(ARBITRARY_VELOCITY_LOW);
    rpc_velocity_ned->set_down_m_s(ARBITRARY_VELOCITY_NEG);
    rpc_velocity_ned->set_yaw_deg(ARBITRARY_YAW);

    return rpc_velocity_ned;
}

TEST_F(OffboardServiceImplTest, setsVelocityNedCorrectly)
{
    MockOffboard offboard;
    OffboardServiceImpl offboardService(offboard);
    mavsdk::rpc::offboard::SetVelocityNedRequest request;

    auto rpc_velocity_ned = createArbitraryRPCVelocityNedYaw();
    const auto expected_velocity_ned =
        OffboardServiceImpl::translateRPCVelocityNedYaw(*rpc_velocity_ned);
    EXPECT_CALL(offboard, set_velocity_ned(expected_velocity_ned)).Times(1);

    request.set_allocated_velocity_ned_yaw(rpc_velocity_ned.release());

    offboardService.SetVelocityNed(nullptr, &request, nullptr);
}

INSTANTIATE_TEST_CASE_P(
    OffboardResultCorrespondences,
    OffboardServiceImplTest,
    ::testing::ValuesIn(generateInputPairs()));

std::vector<InputPair> generateInputPairs()
{
    std::vector<InputPair> input_pairs;
    input_pairs.push_back(std::make_pair("SUCCESS", mavsdk::Offboard::Result::SUCCESS));
    input_pairs.push_back(std::make_pair("NO_SYSTEM", mavsdk::Offboard::Result::NO_SYSTEM));
    input_pairs.push_back(
        std::make_pair("CONNECTION_ERROR", mavsdk::Offboard::Result::CONNECTION_ERROR));
    input_pairs.push_back(std::make_pair("BUSY", mavsdk::Offboard::Result::BUSY));
    input_pairs.push_back(
        std::make_pair("COMMAND_DENIED", mavsdk::Offboard::Result::COMMAND_DENIED));
    input_pairs.push_back(std::make_pair("TIMEOUT", mavsdk::Offboard::Result::TIMEOUT));
    input_pairs.push_back(
        std::make_pair("NO_SETPOINT_SET", mavsdk::Offboard::Result::NO_SETPOINT_SET));
    input_pairs.push_back(std::make_pair("UNKNOWN", mavsdk::Offboard::Result::UNKNOWN));

    return input_pairs;
}

} // namespace
