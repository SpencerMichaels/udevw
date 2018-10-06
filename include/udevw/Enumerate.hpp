#ifndef UDEVW_ENUMERATE_HPP
#define UDEVW_ENUMERATE_HPP

#include <libudev.h>

#include "Device.hpp"
#include "Exception.hpp"
#include "Handle.hpp"
#include "List.hpp"
#include "Udev.hpp"

namespace udevw {

  class Enumerate : public Handle<struct udev_enumerate, &udev_enumerate_unref> {
  public:
    static Enumerate create(UdevPtr udev) {
      auto raw_resource = udev_enumerate_new(udev->raw());
      return Enumerate(std::move(udev), raw_resource);
    }

  public:
    Enumerate(UdevPtr udev, ResourceRawPtr enumerate)
      : Handle(std::move(udev), enumerate)
    {
    }

    void add_match_subsystem(const std::string& subsystem) const {
      if (const auto err = udev_enumerate_add_match_subsystem(raw(), subsystem.c_str()))
        throw Exception(err);
    }

    void add_nomatch_subsystem(const std::string& subsystem) const {
      if (const auto err = udev_enumerate_add_nomatch_subsystem(raw(), subsystem.c_str()))
        throw Exception(err);
    }

    void add_match_sysattr(const std::string& sysattr, const std::string& value) const {
      if (const auto err = udev_enumerate_add_match_sysattr(raw(), sysattr.c_str(), value.c_str()))
        throw Exception(err);
    }

    void add_nomatch_sysattr(const std::string& sysattr, const std::string& value) const {
      if (const auto err =  udev_enumerate_add_nomatch_sysattr(raw(), sysattr.c_str(), value.c_str()))
        throw Exception(err);
    }

    void add_match_property(const std::string& property_, const std::string& value) const {
      if (const auto err = udev_enumerate_add_match_property(raw(), property_.c_str(), value.c_str()))
        throw Exception(err);
    }

    void add_match_tag(const std::string& tag) const {
      if (const auto err = udev_enumerate_add_match_tag(raw(), tag.c_str()))
        throw Exception(err);
    }

    void add_match_parent(const Device& device) const {
      if (const auto err = udev_enumerate_add_match_parent(raw(), device.raw()))
        throw Exception(err);
    }

    void add_match_is_initialized() const {
      if (const auto err = udev_enumerate_add_match_is_initialized(raw()))
        throw Exception(err);
    }

    void add_match_sysname(const std::string& sysname) const {
      if (const auto err = udev_enumerate_add_match_sysname(raw(), sysname.c_str()))
        throw Exception(err);
    }

    void add_syspath(const std::string& syspath) const {
      if (const auto err = udev_enumerate_add_syspath(raw(), syspath.c_str()))
        throw Exception(err);
    }

    std::vector<ListEntry> scan_devices() const {
      udev_enumerate_scan_devices(raw());
      return get_list();
    }

    std::vector<ListEntry> scan_subsystems() const {
      udev_enumerate_scan_subsystems(raw());
      return get_list();
    }

  private:
    List get_list() const {
      return flatten_list(udev_enumerate_get_list_entry(raw()));
    }
  };

}

#endif //UDEVW_ENUMERATE_HPP
