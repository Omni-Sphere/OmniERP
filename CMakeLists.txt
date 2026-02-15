cmake_minimum_required(VERSION 3.16)
project(OmniERP LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

set(LIBS_DIR "${CMAKE_BINARY_DIR}/OmniERP/Libs")
set(INCLUDE_DIR "${CMAKE_BINARY_DIR}/OmniERP/Include")

add_library(OmniERP SHARED
    Item/Item.cpp
    Item/Repositories/Item.cpp
    ItemGroup/ItemGroup.cpp
    ItemGroup/Repositories/ItemGroup.cpp
    ItemBrand/ItemBrand.cpp
    ItemBrand/Repositories/ItemBrand.cpp
)

target_include_directories(OmniERP
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Item>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/ItemGroup>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/ItemBrand>
        $<INSTALL_INTERFACE:include/OmniSphere/OmniERP>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/OmniCore/Include
        ${CMAKE_CURRENT_SOURCE_DIR}/OmniUtils/Include
        ${CMAKE_CURRENT_SOURCE_DIR}/OmniData/Include
)

find_package(Boost REQUIRED COMPONENTS json)

if(WIN32)
    target_link_libraries(OmniERP PUBLIC
        Boost::json
        ${CMAKE_CURRENT_SOURCE_DIR}/OmniUtils/libOmniUtils.dll.a
        ${CMAKE_CURRENT_SOURCE_DIR}/OmniData/libOmniData.dll.a
        ${CMAKE_CURRENT_SOURCE_DIR}/OmniCore/libOmniCore.dll.a
    )
    add_custom_command(TARGET OmniERP POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "${LIBS_DIR}"
        COMMAND ${CMAKE_COMMAND} -E make_directory "${INCLUDE_DIR}"
        # Copy dependency DLLs to Libs directory
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_CURRENT_SOURCE_DIR}/OmniData/Libs/libOmniData.dll"
            "${LIBS_DIR}/libOmniData.dll"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_CURRENT_SOURCE_DIR}/OmniUtils/Libs/libOmniUtils.dll"
            "${LIBS_DIR}/libOmniUtils.dll"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_CURRENT_SOURCE_DIR}/OmniCore/Libs/libOmniCore.dll"
            "${LIBS_DIR}/libOmniCore.dll"
    )
endif()

# Build output directories
if(WIN32)
    set_target_properties(OmniERP PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${LIBS_DIR}"
        LIBRARY_OUTPUT_DIRECTORY "${LIBS_DIR}"
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/OmniERP"
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
    Item/Models
    Item/DTOs
    ItemGroup/Models
    ItemGroup/DTOs
    ItemBrand/Models
    ItemBrand/DTOs
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

if(UNIX AND NOT APPLE)
    install(TARGETS OmniERP
        LIBRARY DESTINATION lib/OmniSphere
        ARCHIVE DESTINATION lib/OmniSphere
        RUNTIME DESTINATION bin
    )
    
    foreach(HDR ${PUBLIC_HEADERS})
        file(RELATIVE_PATH REL_PATH "${CMAKE_CURRENT_SOURCE_DIR}" "${HDR}")
        get_filename_component(HDR_DIR "${REL_PATH}" DIRECTORY)
        
        if(HDR_DIR)
            install(FILES "${HDR}" DESTINATION "include/OmniSphere/OmniERP/${HDR_DIR}")
        else()
            install(FILES "${HDR}" DESTINATION "include/OmniSphere/OmniERP")
        endif()
    endforeach()
    
    foreach(DIR ${HEADER_DIRS})
        file(GLOB_RECURSE HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/${DIR}/*.hpp")
        foreach(HDR ${HEADERS})
            file(RELATIVE_PATH REL_PATH "${CMAKE_CURRENT_SOURCE_DIR}" "${HDR}")
            get_filename_component(HDR_DIR "${REL_PATH}" DIRECTORY)
            
            if(HDR_DIR)
                install(FILES "${HDR}" DESTINATION "include/OmniSphere/OmniERP/${HDR_DIR}")
            else()
                install(FILES "${HDR}" DESTINATION "include/OmniSphere/OmniERP")
            endif()
        endforeach()
    endforeach()
    
    install(CODE "
        file(WRITE \"/etc/ld.so.conf.d/omnisphere.conf\" \"/usr/local/lib/OmniSphere\\n\")
        execute_process(COMMAND ldconfig)
        message(STATUS \"Configured ldconfig for OmniSphere libraries\")
    ")
endif()
