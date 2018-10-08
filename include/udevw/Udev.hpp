#ifndef UDEVW_UDEV_HPP
#define UDEVW_UDEV_HPP

#include <memory>

#include <libudev.h>

namespace udevw {

  class Udev : public std::enable_shared_from_this<Udev> {
  public:
    using Pointer = std::shared_ptr<Udev>;
    using Resource = struct udev;
    using ResourceRawPtr = Resource*;
    using ResourcePtr = std::unique_ptr<Resource, decltype(&udev_unref)>;

  public:
    static Pointer create() {
      return Pointer(new Udev(udev_new()));
    }

  public:
    Udev(const Udev &) = delete;
    Udev(Udev &&other) = delete;
    Udev &operator=(const Udev &) = delete;
    Udev &operator=(Udev &&other) = delete;

    ResourceRawPtr raw() const {
      return _udev.get();
    }

    template <typename T>
    void set_userdata(T *userdata) const {
      udev_set_userdata(raw(), static_cast<void*>(userdata));
    }

    template <typename T>
    void get_userdata() const {
      return static_cast<T*>(udev_get_userdata(raw()));
    }

  private:
    Udev(ResourceRawPtr udev)
      : _udev(udev, &udev_unref)
    {
    }

    ResourcePtr _udev;
  };

}

#endif //UDEVW_UDEV_HPP
