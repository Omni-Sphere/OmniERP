#pragma once
#include <OmniData/Database.hpp>
#include "Module/DTOs/Module.hpp"
#include "Module/Models/Module.hpp"

namespace omnisphere::module {
class Module {
public:
  explicit Module(std::shared_ptr<omnisphere::data::Database> database);
  ~Module();

  std::vector<omnisphere::models::Module> GetAll() const;
  omnisphere::models::Module
  Get(const omnisphere::dtos::GetModule &getModule) const;

private:
  struct Impl;
  std::unique_ptr<Impl> pImpl;
};
} // namespace omnisphere::module
