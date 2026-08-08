#include "Node/DTOs/GetNode.hpp"
#include <OmniData/DataTable.hpp>
#include <OmniData/Database.hpp>
#pragma once

#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "Node/DTOs/CreateNode.hpp"
#include "Node/DTOs/SearchNode.hpp"
#include "Node/DTOs/UpdateNode.hpp"
#include "Node/Models/Node.hpp"

namespace omnisphere::services {
class Node {
public:
  explicit Node(std::shared_ptr<omnisphere::data::Database> database);
  ~Node();

  std::optional<omnisphere::models::Node>
  Get(const omnisphere::dtos::GetNode &filter,
      const std::vector<std::string> &fields = {}) const;
  std::vector<omnisphere::models::Node>
  GetAll(const std::vector<std::string> &fields = {}) const;
  std::vector<omnisphere::models::Node>
  Search(const std::vector<std::string> &fields,
         const omnisphere::dtos::SearchNode &filter) const;
  bool Add(const omnisphere::dtos::CreateNode &_node) const;
  bool Modify(const omnisphere::dtos::UpdateNode &_node) const;
  bool Remove(int entry) const;

private:
  struct Impl;
  std::unique_ptr<Impl> pimpl;
};

} // namespace omnisphere::services
