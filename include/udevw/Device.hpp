#ifndef UDEVW_DEVICE_HPP
#define UDEVW_DEVICE_HPP

#include <optional>
#include <string>

#include <libudev.h>

#include "Exception.hpp"
#include "Handle.hpp"
#include "List.hpp"
#include "Udev.hpp"
#include "Utility.hpp"

namespace udevw {

  using SequenceNumber = unsigned long long int;

  class Device : public Handle<struct udev_device, &udev_device_unref> {
  public:
    static Device create_from_syspath(UdevPtr udev, const std::string& syspath) {
      auto device = udev_device_new_from_syspath(udev->raw(), syspath.c_str());
      return Device(std::move(udev), device);
    }

    static Device create_from_devnum(UdevPtr udev, char type, dev_t devnum) {
      auto device = udev_device_new_from_devnum(udev->raw(), type, devnum);
      return Device(std::move(udev), device);
    }

    static Device create_from_subsystem_sysname(UdevPtr udev, const std::string& subsystem, const std::string& sysname) {
      auto device = udev_device_new_from_subsystem_sysname(udev->raw(), subsystem.c_str(), sysname.c_str());
      return Device(std::move(udev), device);
    }

    static Device create_from_device_id(UdevPtr udev, const std::string& device_id) {
      auto device = udev_device_new_from_device_id(udev->raw(), device_id.c_str());
      return Device(std::move(udev), device);
    }

    static Device create_from_environment(UdevPtr udev) {
      auto device = udev_device_new_from_environment(udev->raw());
      return Device(std::move(udev), device);
    }

  public:
    Device(UdevPtr udev, ResourceRawPtr device)
      : Handle(std::move(udev), device)
    {
    }

    std::optional<Device> get_parent() const {
      if (udev_device_ref(udev_device_get_parent(raw())) == NULL)
        return {};
      else
        return std::optional<Device>(std::in_place, udev().shared_from_this(), udev_device_ref(udev_device_get_parent(raw())));
    }

    std::optional<Device> get_parent(const std::string& subsystem, const std::string& devtype) const {
      if (udev_device_ref(udev_device_get_parent_with_subsystem_devtype(raw(), subsystem.c_str(), devtype.c_str())) == NULL)
        return {};
      else
        return std::optional<Device>(std::in_place, udev().shared_from_this(), udev_device_ref(udev_device_get_parent_with_subsystem_devtype(raw(), subsystem.c_str(), devtype.c_str())));
    }

    OptionalString get_devpath() const {
      auto s = udev_device_get_devpath(raw());
      return raw_to_optional<std::string>(s);
    }

    OptionalString get_subsystem() const {
      auto s = udev_device_get_subsystem(raw());
      return raw_to_optional<std::string>(s);
    }

    OptionalString get_devtype() const {
      auto s = udev_device_get_devtype(raw());
      return raw_to_optional<std::string>(s);
    }

    OptionalString get_syspath() const {
      auto s = udev_device_get_syspath(raw());
      return raw_to_optional<std::string>(s);
    }

    OptionalString get_sysname() const {
      auto s = udev_device_get_sysname(raw());
      return raw_to_optional<std::string>(s);
    }

    OptionalString get_sysnum() const {
      auto s = udev_device_get_sysnum(raw());
      return raw_to_optional<std::string>(s);
    }

    OptionalString get_devnode() const {
      auto s = udev_device_get_devnode(raw());
      return raw_to_optional<std::string>(s);
    }

    bool get_is_initialized() const {
      return udev_device_get_is_initialized(raw());
    }

    List get_devlinks() const {
      return flatten_list(udev_device_get_devlinks_list_entry(raw()));
    }

    List get_properties() const {
      return flatten_list(udev_device_get_properties_list_entry(raw()));
    }

    List get_tags() const {
      return flatten_list(udev_device_get_tags_list_entry(raw()));
    }

    List get_sysattrs() const {
      return flatten_list(udev_device_get_sysattr_list_entry(raw()));
    }

    OptionalString get_property_value(const std::string& key) const {
      auto s = udev_device_get_property_value(raw(), key.c_str());
      return raw_to_optional<std::string>(s);
    }

    OptionalString get_driver() const {
      auto s = udev_device_get_driver(raw());
      return raw_to_optional<std::string>(s);
    }

    std::optional<dev_t> get_devnum() const {
      return raw_to_optional<dev_t>(udev_device_get_devnum(raw()));
    }

    OptionalString get_action() const {
      auto s = udev_device_get_action(raw());
      return raw_to_optional<std::string>(s);
    }

    SequenceNumber get_seqnum() const {
      return udev_device_get_seqnum(raw());
    }

    SequenceNumber get_usec_since_initialized() const {
      return udev_device_get_usec_since_initialized(raw());
    }

    OptionalString get_sysattr_value(const std::string& sysattr) const {
      auto s = udev_device_get_sysattr_value(raw(), (char*)sysattr.c_str());
      return raw_to_optional<std::string>(s);
    }

    void set_sysattr_value(const std::string& sysattr, const std::string& value) const {
      if (const auto err = udev_device_set_sysattr_value(raw(), sysattr.c_str(), (char*)value.c_str()))
        throw Exception(err);
    }

    bool has_tag(const std::string& tag) const {
      return udev_device_has_tag(raw(), tag.c_str());
    }
  };

}

#endif //UDEVW_DEVICE_HPP
