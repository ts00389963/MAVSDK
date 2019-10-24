#include "mavsdk.h"

#include "mavsdk_impl.h"
#include "global_include.h"

namespace mavsdk {

Mavsdk::Mavsdk() : _impl(std::make_unique<MavsdkImpl>()) {}

Mavsdk::~Mavsdk() {}

std::string Mavsdk::version() const
{
    return _impl->version();
}

ConnectionResult Mavsdk::add_any_connection(const std::string& connection_url)
{
    return _impl->add_any_connection(connection_url);
}

ConnectionResult Mavsdk::add_udp_connection(int local_port)
{
    return Mavsdk::add_udp_connection(DEFAULT_UDP_BIND_IP, local_port);
}

ConnectionResult Mavsdk::add_udp_connection(const std::string& local_bind_ip, const int local_port)
{
    return _impl->add_udp_connection(local_bind_ip, local_port);
}

ConnectionResult Mavsdk::setup_udp_remote(const std::string& remote_ip, int remote_port)
{
    return _impl->setup_udp_remote(remote_ip, remote_port);
}

ConnectionResult Mavsdk::add_tcp_connection(const std::string& remote_ip, const int remote_port)
{
    return _impl->add_tcp_connection(remote_ip, remote_port);
}

ConnectionResult Mavsdk::add_serial_connection(const std::string& dev_path, const int baudrate)
{
    return _impl->add_serial_connection(dev_path, baudrate);
}

void Mavsdk::set_configuration(Configuration configuration)
{
    _impl->set_configuration(configuration);
}

std::vector<uint64_t> Mavsdk::system_uuids() const
{
    return _impl->get_system_uuids();
}

System& Mavsdk::system() const
{
    return _impl->get_system();
}

System& Mavsdk::system(const uint64_t uuid) const
{
    return _impl->get_system(uuid);
}

bool Mavsdk::is_connected() const
{
    return _impl->is_connected();
}

bool Mavsdk::is_connected(const uint64_t uuid) const
{
    return _impl->is_connected(uuid);
}

void Mavsdk::register_on_discover(const event_callback_t callback)
{
    _impl->register_on_discover(callback);
}

void Mavsdk::register_on_timeout(const event_callback_t callback)
{
    _impl->register_on_timeout(callback);
}

} // namespace mavsdk
