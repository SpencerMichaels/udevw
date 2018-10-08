#ifndef UDEVW_UTILITY_HPP
#define UDEVW_UTILITY_HPP

namespace udevw {

  using OptionalString = std::optional<std::string>;

  template <typename Ret, typename Raw>
  std::optional<Ret> raw_to_optional(Raw raw) {
    return raw ? std::optional<Ret>(raw) : std::nullopt;
  }

}

#endif //UDEVW_UTILITY_HPP
