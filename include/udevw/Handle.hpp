#ifndef UDEVW_HANDLE_HPP
#define UDEVW_HANDLE_HPP

#include "Exception.hpp"
#include "Udev.hpp"

namespace udevw {

  template <typename Resource, typename std::add_pointer<Resource*(Resource*)>::type _deleter>
  class Handle {
  protected:
    using UdevPtr = std::shared_ptr<Udev>;
    using ResourceDeleter = typename std::add_pointer<Resource*(Resource*)>::type;
    using ResourceRawPtr = Resource*;
    using ResourcePtr = std::unique_ptr<Resource, ResourceDeleter>;

    Handle(UdevPtr udev, Resource *resource)
      : _udev(std::move(udev)), _resource(resource, _deleter)
    {
      if (!resource)
        throw Exception(-errno);
    }

  public:
    virtual ~Handle() = default;

    Handle &operator=(const Handle&) = delete;
    Handle &operator=(Handle&&) = delete;

    Udev &udev() const noexcept {
      return *_udev;
    };

    ResourceRawPtr raw() const noexcept {
      return _resource.get();
    }

    ResourceRawPtr raw() noexcept {
      return const_cast<Resource*>(const_cast<const Handle*>(this)->raw());
    }

  private:
    UdevPtr _udev;
    ResourcePtr _resource;
  };

}

#endif //UDEVW_HANDLE_HPP
