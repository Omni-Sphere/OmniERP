#pragma once

#include <optional>
#include <Base/Filters.hpp>
#include <string>

namespace omnisphere::dtos
{


    struct BrandFilterInput {
        std::optional<StringFilter> Name;
    };

    struct GroupFilterInput {
        std::optional<StringFilter> Name;
    };

    struct ItemFilter
    {
        std::optional<std::string> Search;
        std::optional<StringFilter> Code;
        std::optional<StringFilter> Name;
        std::optional<bool> IsActive;
        std::optional<FloatFilter> Price;
        std::optional<FloatFilter> OnHand;
        std::optional<FloatFilter> OnOrder;
        std::optional<FloatFilter> OnRequest;
        std::optional<FloatFilter> MinStock;
        std::optional<FloatFilter> MaxStock;
        std::optional<FloatFilter> MinOrder;
        std::optional<FloatFilter> MaxOrder;
        std::optional<FloatFilter> MinRequest;
        std::optional<FloatFilter> MaxRequest;
        std::optional<BrandFilterInput> Brand;
        std::optional<GroupFilterInput> Group;
    };
} // namespace omnisphere::dtos
