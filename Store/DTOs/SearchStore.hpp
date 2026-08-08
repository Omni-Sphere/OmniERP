#pragma once

#include <optional>
#include <string>

namespace omnisphere::dtos {
struct SearchStore {
  int Entry;
  std::string Code;
  std::string Name;
  int GuestCustomer;
  std::optional<std::string> Address;
  std::optional<std::string> Address2;
  std::optional<int> City;
  std::optional<int> State;
  std::optional<int> ZipCode;
  std::optional<int> Country;
  int Currency;
  std::optional<std::string> Phone1;
  std::optional<std::string> Phone2;
  std::optional<std::string> Email;
  std::string IsActive;
};
} // namespace omnisphere::dtos
