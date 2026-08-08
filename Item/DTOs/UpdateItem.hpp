#pragma once
#include <OmniCore/Base/BaseUpdateDTO.hpp>
#include <optional>
#include <regex>
#include <stdexcept>
#include <string>

#include <boost/describe.hpp>

namespace omnisphere::dtos {
struct UpdateItem : public omnisphere::dtos::BaseUpdateDTO {
  UpdateItem(std::string _Code, std::optional<std::string> _Name,
             std::optional<std::string> _Description,
             std::optional<std::string> _Image, std::optional<bool> _IsActive,
             std::optional<bool> _PurchaseItem, std::optional<bool> _SellItem,
             std::optional<bool> _InventoryItem, std::optional<double> _Price,
             std::optional<int> _Brand, std::optional<int> _Group,
             std::optional<double> _MinStock, std::optional<double> _MaxStock,
             std::optional<double> _MinOrder, std::optional<double> _MaxOrder,
             std::optional<double> _MinRequest,
             std::optional<double> _MaxRequest, int _LastUpdatedBy,
             std::string _UpdateDate)
      : omnisphere::dtos::BaseUpdateDTO(std::move(_Code), std::move(_Name),
                                        _LastUpdatedBy, std::move(_UpdateDate)),
        Description(std::move(_Description)), Image(std::move(_Image)),
        IsActive(_IsActive), PurchaseItem(_PurchaseItem), SellItem(_SellItem),
        InventoryItem(_InventoryItem), Price(_Price), Brand(std::move(_Brand)),
        Group(std::move(_Group)), MinStock(_MinStock), MaxStock(_MaxStock),
        MinOrder(_MinOrder), MaxOrder(_MaxOrder), MinRequest(_MinRequest),
        MaxRequest(_MaxRequest) {
    ValidateUpdateItem();
  }

  std::optional<std::string> Description;
  std::optional<std::string> Image;
  std::optional<bool> IsActive;
  std::optional<bool> PurchaseItem;
  std::optional<bool> SellItem;
  std::optional<bool> InventoryItem;
  std::optional<double> Price;
  std::optional<int> Brand;
  std::optional<int> Group;
  std::optional<double> MinStock;
  std::optional<double> MaxStock;
  std::optional<double> MinOrder;
  std::optional<double> MaxOrder;
  std::optional<double> MinRequest;
  std::optional<double> MaxRequest;

  void ValidateUpdateItem() const {}
};

BOOST_DESCRIBE_STRUCT(UpdateItem, (),
                      (Name, Description, Image, IsActive, PurchaseItem,
                       SellItem, InventoryItem, Price, Brand, Group, MinStock,
                       MaxStock, MinOrder, MaxOrder, MinRequest, MaxRequest))
} // namespace omnisphere::dtos