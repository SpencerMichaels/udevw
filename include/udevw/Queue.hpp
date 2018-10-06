#ifndef UDEVW_QUEUE_HPP
#define UDEVW_QUEUE_HPP

#include "List.hpp"
#include "Handle.hpp"
#include "Utility.hpp"

namespace udevw {

  class Queue : public Handle<struct udev_queue, &udev_queue_unref> {
  public:
    static Queue create_from_netlink(UdevPtr udev) {
      auto queue = udev_queue_new(udev->raw());
      return Queue(std::move(udev), queue);
    }

    bool get_udev_is_active() {
      return udev_queue_get_udev_is_active(raw());
    }

    bool get_queue_is_empty() {
      return udev_queue_get_queue_is_empty(raw());
    }

    int get_fd() {
      const auto fd = udev_queue_get_fd(raw());
      if (fd < 0)
        throw Exception(-fd);
      else
        return fd;
    }

    void flush() {
      if (const auto err = udev_queue_flush(raw()))
        throw Exception(-err);
    }

  public:
    Queue(UdevPtr udev, ResourceRawPtr queue)
    : Handle(std::move(udev), queue)
    {
    }
  };

}

#endif //UDEVW_QUEUE_HPP
