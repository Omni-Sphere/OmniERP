cmake_minimum_required(VERSION 3.16)
project(OmniERP LANGUAGES CXX)

include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

set(INCLUDE_DIR "${CMAKE_BINARY_DIR}/OmniERP/Include")
set(LIBS_DIR "${CMAKE_BINARY_DIR}/OmniERP")

if(WIN32)
    set(ARCHIVE_DIR "${CMAKE_BINARY_DIR}/OmniERP/Libs")
    set(CORE_RUNTIME_DIR "${CMAKE_CURRENT_SOURCE_DIR}/OmniCore")
else()
    set(ARCHIVE_DIR "${LIBS_DIR}")
endif()

add_library(OmniERP SHARED
    Item/Item.cpp
    Item/Repositories/Item.cpp
    ItemGroup/ItemGroup.cpp
    ItemGroup/Repositories/ItemGroup.cpp
    ItemBrand/ItemBrand.cpp
    ItemBrand/Repositories/ItemBrand.cpp
    Floor/Floor.cpp
    Floor/Repositories/Floor.cpp
    Area/Area.cpp
    Area/Repositories/Area.cpp
    Table/Table.cpp
    Table/Repositories/Table.cpp
    Company/Company.cpp
    Company/Repositories/Company.cpp
    Store/Store.cpp
    Store/Repositories/Store.cpp
    Department/Department.cpp
    Department/Repositories/Department.cpp
    Employee/Employee.cpp
    Employee/Repositories/Employee.cpp
    Module/Module.cpp
    Module/Repositories/Module.cpp
    Permission/Permission.cpp
    Permission/Repositories/Permission.cpp
    Role/Role.cpp
    Role/Repositories/Role.cpp
    UserModule/UserModule.cpp
    UserModule/Repositories/UserModule.cpp
    RoleModule/RoleModule.cpp
    RoleModule/Repositories/RoleModule.cpp
    UserPermission/UserPermission.cpp
    UserPermission/Repositories/UserPermission.cpp
    RolePermission/RolePermission.cpp
    RolePermission/Repositories/RolePermission.cpp
    Customer/Customer.cpp
    Customer/Repositories/Customer.cpp
    Node/Node.cpp
    Node/Repositories/Node.cpp
)

target_include_directories(OmniERP
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

add_library(OmniERP::OmniERP ALIAS OmniERP)

if(NOT TARGET OmniUtils::OmniUtils)
    find_package(OmniUtils REQUIRED)
endif()
if(NOT TARGET OmniData::OmniData)
    find_package(OmniData REQUIRED)
endif()
if(NOT TARGET OmniCore::OmniCore)
    find_package(OmniCore REQUIRED)
endif()

target_link_libraries(OmniERP PUBLIC OmniUtils::OmniUtils OmniData::OmniData OmniCore::OmniCore)

if(WIN32)
    set_target_properties(OmniERP PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${LIBS_DIR}"
        LIBRARY_OUTPUT_DIRECTORY "${LIBS_DIR}"
        ARCHIVE_OUTPUT_DIRECTORY "${ARCHIVE_DIR}"
    )
else()
    set_target_properties(OmniERP PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY "${LIBS_DIR}"
        ARCHIVE_OUTPUT_DIRECTORY "${LIBS_DIR}"
    )
endif()

set(PUBLIC_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/Item/Item.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ItemGroup/ItemGroup.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ItemBrand/ItemBrand.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Floor/Floor.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Area/Area.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Table/Table.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Company/Company.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Store/Store.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Department/Department.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Employee/Employee.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Module/Module.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Permission/Permission.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Role/Role.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UserModule/UserModule.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/RoleModule/RoleModule.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UserPermission/UserPermission.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/RolePermission/RolePermission.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Customer/Customer.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Node/Node.hpp
)

foreach(HDR ${PUBLIC_HEADERS})
    file(RELATIVE_PATH REL_PATH "${CMAKE_CURRENT_SOURCE_DIR}" "${HDR}")
    set(DEST_PATH "${INCLUDE_DIR}/${REL_PATH}")

    get_filename_component(DEST_INC_DIR "${DEST_PATH}" DIRECTORY)
    add_custom_command(TARGET OmniERP POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "${DEST_INC_DIR}"
        COMMAND ${CMAKE_COMMAND} -E copy "${HDR}" "${DEST_PATH}"
    )
endforeach()

set(
    HEADER_DIRS
    Floor/Models
    Floor/DTOs
    Item/Models
    Item/DTOs
    ItemGroup/Models
    ItemGroup/DTOs
    ItemBrand/Models
    ItemBrand/DTOs
    Floor/Models
    Floor/DTOs
    Area/Models
    Area/DTOs
    Table/Models
    Table/DTOs
    Company/Models
    Company/DTOs
    Store/Models
    Store/DTOs
    Department/Models
    Department/DTOs
    Employee/Models
    Employee/DTOs
    Module/Models
    Module/DTOs
    Permission/Models
    Permission/DTOs
    Role/Models
    Role/DTOs
    UserModule/Models
    UserModule/DTOs
    RoleModule/Models
    RoleModule/DTOs
    UserPermission/Models
    UserPermission/DTOs
    RolePermission/Models
    RolePermission/DTOs
    Customer/Models
    Customer/DTOs
    Node/Models
    Node/DTOs
    Node/Enums
)

foreach(DIR ${HEADER_DIRS})
    file(GLOB_RECURSE HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/${DIR}/*.hpp")
    foreach(HDR ${HEADERS})
        file(RELATIVE_PATH REL_PATH "${CMAKE_CURRENT_SOURCE_DIR}" "${HDR}")
        set(DEST_PATH "${INCLUDE_DIR}/${REL_PATH}")

        get_filename_component(DEST_INC_DIR "${DEST_PATH}" DIRECTORY)
        add_custom_command(TARGET OmniERP POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory "${DEST_INC_DIR}"
            COMMAND ${CMAKE_COMMAND} -E copy "${HDR}" "${DEST_PATH}"
        )
    endforeach()
endforeach()

# --- Installation & Export ---
include(GNUInstallDirs)

install(TARGETS OmniERP
    EXPORT OmniERPTargets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING PATTERN "*.hpp" PATTERN "*.h"
    PATTERN "build" EXCLUDE
    PATTERN ".git" EXCLUDE
    PATTERN ".vscode" EXCLUDE
)

file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/OmniERPConfig.cmake"
"include(CMakeFindDependencyMacro)\n"
"find_dependency(Boost REQUIRED COMPONENTS json)\n"
"find_dependency(OmniUtils REQUIRED)\n"
"find_dependency(OmniData REQUIRED)\n"
"find_dependency(OmniCore REQUIRED)\n"
"include(\"\${CMAKE_CURRENT_LIST_DIR}/OmniERPTargets.cmake\")\n"
)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/OmniERPConfig.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/OmniERP
)

install(EXPORT OmniERPTargets
    FILE OmniERPTargets.cmake
    NAMESPACE OmniERP::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/OmniERP
)

