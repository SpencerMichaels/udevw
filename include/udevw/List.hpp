#ifndef UDEVW_LIST_HPP
#define UDEVW_LIST_HPP

#include <string>
#include <vector>

#include <libudev.h>

namespace udevw {

  struct ListEntry {
    ListEntry(std::string name, std::string value)
      : name(std::move(name)), value(std::move(value))
    {
    }

    std::string name, value;
  };

  using List = std::vector<ListEntry>;

  List flatten_list(struct udev_list_entry *list) {
    static const auto null_to_empty_string = [](const char *s) {
      return s ? s : "";
    };

    List ret;

    struct udev_list_entry *entry;
    udev_list_entry_foreach(entry, list) {
      ret.emplace_back(
          null_to_empty_string(udev_list_entry_get_name(entry)),
          null_to_empty_string(udev_list_entry_get_value(entry))
      );
    }

    return ret;
  }

}

#endif //UDEVW_LIST_HPP
