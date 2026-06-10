#include <Database.hpp>
#include <DataTable.hpp>
#include <DataTable.hpp>
#include <Database.hpp>
#include <DataTable.hpp>
#include <RoleModule/RoleModule.hpp>
#include <RoleModule/Repositories/RoleModule.hpp>
#include <stdexcept>
namespace omnisphere::rolemodule {
struct RoleModule::Impl {
    std::shared_ptr<omnisphere::repositories::RoleModuleRepository> repository;
    explicit Impl(std::shared_ptr<omnisphere::services::Database> database) : repository(std::make_shared<omnisphere::repositories::RoleModuleRepository>(database)) {}
};
RoleModule::RoleModule(std::shared_ptr<omnisphere::services::Database> database) : pImpl(std::make_unique<Impl>(database)) {}
RoleModule::~RoleModule() = default;
omnisphere::models::RoleModule RoleModule::Add(const omnisphere::dtos::CreateRoleModule& roleModule) const {
    if (pImpl->repository->Create(roleModule)) { omnisphere::dtos::GetRoleModule get; get.RoleEntry = roleModule.RoleEntry; get.ModuleEntry = roleModule.ModuleEntry; return Get(get); }
    throw std::runtime_error("Error adding role module");
}
omnisphere::models::RoleModule RoleModule::Modify(const omnisphere::dtos::UpdateRoleModule& roleModule) const {
    if (pImpl->repository->Update(roleModule)) { omnisphere::dtos::GetRoleModule get; get.Entry = roleModule.Entry; return Get(get); }
    throw std::runtime_error("Error updating role module");
}
std::vector<omnisphere::models::RoleModule> RoleModule::GetAll() const {
    std::vector<omnisphere::models::RoleModule> list;
    omnisphere::types::DataTable data = pImpl->repository->ReadAll();
    for (int i = 0; i < data.RowsCount(); i++) list.push_back({data[i]["Entry"], data[i]["RoleEntry"], data[i]["ModuleEntry"], data[i]["IsActive"], data[i]["CreatedBy"], data[i]["CreateDate"], data[i]["LastUpdatedBy"].GetOptional<int>(), data[i]["UpdateDate"].GetOptional<std::string>()});
    return list;
}
omnisphere::models::RoleModule RoleModule::Get(const omnisphere::dtos::GetRoleModule& getRoleModule) const {
    omnisphere::types::DataTable data = pImpl->repository->Read(getRoleModule);
    if (data.RowsCount() == 0) throw std::runtime_error("RoleModule doesn't exist");
    return {data[0]["Entry"], data[0]["RoleEntry"], data[0]["ModuleEntry"], data[0]["IsActive"], data[0]["CreatedBy"], data[0]["CreateDate"], data[0]["LastUpdatedBy"].GetOptional<int>(), data[0]["UpdateDate"].GetOptional<std::string>()};
}
bool RoleModule::Remove(int entry) const { return pImpl->repository->Delete(entry); }
}
