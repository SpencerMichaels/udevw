#ifndef UDEVW_HWDB_HPP
#define UDEVW_HWDB_HPP

#include "Handle.hpp"
#include "List.hpp"

namespace udevw {

  class HwDb : public Handle<struct udev_hwdb, &udev_hwdb_unref> {
  public:
    static HwDb create(UdevPtr udev) {
      auto hwdb = udev_hwdb_new(udev->raw());
      return HwDb(std::move(udev), hwdb);
    }

  public:
    List get_properties(const std::string &modalias, unsigned int flags) {
      auto list = udev_hwdb_get_properties_list_entry(raw(), modalias.c_str(), flags);
      return flatten_list(list);
    }

  private:
    HwDb(UdevPtr udev, ResourceRawPtr hwdb)
    : Handle(std::move(udev), hwdb)
    {
    }
  };

}

#endif //UDEVW_HWDB_HPP
