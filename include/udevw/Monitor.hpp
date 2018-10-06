#ifndef UDEVW_MONITOR_HPP
#define UDEVW_MONITOR_HPP

#include "Device.hpp"
#include "Exception.hpp"

namespace udevw {

  class Monitor : public Handle<struct udev_monitor, &udev_monitor_unref> {
  public:
    static Monitor create_from_netlink(UdevPtr udev, const std::string& netlink) {
      auto monitor = udev_monitor_new_from_netlink(udev->raw(), netlink.c_str());
      return Monitor(std::move(udev), monitor);
    }

  public:
    Monitor(UdevPtr udev, ResourceRawPtr monitor)
      : Handle(std::move(udev), monitor)
    {
    }

    void enable_receiving() {
      if (const auto err = udev_monitor_enable_receiving(raw()))
        throw Exception(-err);
    }

    void set_receive_buffer_size(int size) {
      if (const auto err = udev_monitor_set_receive_buffer_size(raw(), size))
        throw Exception(-err);
    }

    int get_fd() {
      const auto fd = udev_monitor_get_fd(raw());
      if (fd < 0)
        throw Exception(-fd);
      else
        return fd;
    }

    Device receive_device() {
      auto device = udev_monitor_receive_device(raw());
      if (!device)
        throw Exception(errno);
      return Device(udev().shared_from_this(), device);
    }

    void filter_add_match_subsystem(const std::string &subsystem) {
      if (const auto err = udev_monitor_filter_add_match_subsystem_devtype(raw(), subsystem.c_str(), nullptr))
        throw Exception(-err);
    }

    void filter_add_match_subsystem_devtype(const std::string &subsystem, const std::string &devtype) {
      if (const auto err = udev_monitor_filter_add_match_subsystem_devtype(raw(), subsystem.c_str(), devtype.c_str()))
        throw Exception(-err);
    }

    void filter_add_match_tag(const std::string &tag) {
      if (const auto err = udev_monitor_filter_add_match_tag(raw(), tag.c_str()))
        throw Exception(-err);
    }

    void filter_update() {
      if (const auto err = udev_monitor_filter_update(raw()))
        throw Exception(err);
    }

    void filter_remove() {
      if (const auto err = udev_monitor_filter_remove(raw()))
        throw Exception(-err);
    }
  };

}

#endif //UDEVW_MONITOR_HPP
