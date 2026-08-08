#include "Node/DTOs/SearchNode.hpp"
#include <OmniData/DataMapper.hpp>
#include "Node/Node.hpp"
#include "Node/Repositories/Node.hpp"
#include <OmniData/DataTable.hpp>
#include <OmniData/Database.hpp>
#include <stdexcept>

namespace omnisphere::services {
struct Node::Impl {
  explicit Impl(std::shared_ptr<omnisphere::data::Database> database)
      : repository(database) {}

  omnisphere::repositories::Node repository;
};

Node::Node(std::shared_ptr<omnisphere::data::Database> database)
    : pimpl(std::make_unique<Impl>(database)) {}

Node::~Node() = default;

std::optional<omnisphere::models::Node>
Node::Get(const omnisphere::dtos::GetNode &filter,
          const std::vector<std::string> &fields) const {
  if (!filter.Entry.has_value() && !filter.Name.has_value() &&
      !filter.Code.has_value())
    throw std::invalid_argument(
        "Filter (Entry, Code, or Name) cannot be empty");

  omnisphere::types::DataTable dataTable =
      pimpl->repository.Read(filter, fields);

  if (dataTable.IsEmpty()) {
    return std::nullopt;
  }

  return omnisphere::repositories::Node::MapNodeRow(dataTable[0]);
}

std::vector<omnisphere::models::Node>
Node::GetAll(const std::vector<std::string> &fields) const {
  omnisphere::types::DataTable dataTable = pimpl->repository.ReadAll(fields);
  std::vector<omnisphere::models::Node> results;

  for (int i = 0; i < dataTable.RowsCount(); i++) {
    results.emplace_back(
        omnisphere::repositories::Node::MapNodeRow(dataTable[i]));
  }

  return results;
}

std::vector<omnisphere::models::Node>
Node::Search(const std::vector<std::string> &fields,
             const omnisphere::dtos::SearchNode &filter) const {
  omnisphere::types::DataTable dataTable =
      pimpl->repository.Search(fields, filter);
  std::vector<omnisphere::models::Node> results;

  for (int i = 0; i < dataTable.RowsCount(); i++) {
    results.emplace_back(
        omnisphere::repositories::Node::MapNodeRow(dataTable[i]));
  }

  return results;
}

bool Node::Add(const omnisphere::dtos::CreateNode &_node) const {
  return pimpl->repository.Create(_node);
}

bool Node::Modify(const omnisphere::dtos::UpdateNode &_node) const {
  return pimpl->repository.Update(_node);
}

bool Node::Remove(int entry) const { return pimpl->repository.Delete(entry); }

} // namespace omnisphere::services
