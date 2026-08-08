
#pragma once

#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "Node/DTOs/CreateNode.hpp"
#include "Node/DTOs/UpdateNode.hpp"

#include "Node/Enums/NodeType.hpp"
#include "Node/Enums/OperationMode.hpp"
#include "Node/Models/Node.hpp"

#include "Node/DTOs/GetNode.hpp"
#include "Node/DTOs/SearchNode.hpp"
#include <OmniData/DataTable.hpp>
#include <OmniData/Database.hpp>

namespace omnisphere::repositories {
class Node {
public:
  explicit Node(std::shared_ptr<omnisphere::data::Database> database);
  ~Node();

  static omnisphere::enums::NodeType CharToNodeType(const std::string &str);
  static std::string NodeTypeToChar(omnisphere::enums::NodeType type);
  static omnisphere::enums::OperationMode
  CharToOperationMode(const std::string &str);
  static std::string OperationModeToChar(omnisphere::enums::OperationMode mode);
  static omnisphere::models::Node
  MapNodeRow(omnisphere::types::DataTable::Row &row);

  bool Create(const omnisphere::dtos::CreateNode &_node) const;
  bool Update(const omnisphere::dtos::UpdateNode &_node) const;
  omnisphere::types::DataTable
  Read(const omnisphere::dtos::GetNode &filter,
       const std::vector<std::string> &fields = {}) const;
  omnisphere::types::DataTable
  ReadAll(const std::vector<std::string> &fields = {}) const;
  omnisphere::types::DataTable
  Search(const std::vector<std::string> &fields,
         const omnisphere::dtos::SearchNode &filter) const;
  bool Delete(int entry) const;

private:
  std::shared_ptr<omnisphere::data::Database> Database;

  int GetCurrentSequence() const;
  bool UpdateNodeSequence() const;
};
} // namespace omnisphere::repositories
