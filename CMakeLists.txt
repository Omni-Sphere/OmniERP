cmake_minimum_required(VERSION 3.16)
project(OmniERP LANGUAGES CXX)

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

if(WIN32)
    target_link_libraries(OmniERP PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/OmniUtils/Libs/libOmniUtils.dll.a
        ${CMAKE_CURRENT_SOURCE_DIR}/OmniData/Libs/libOmniData.dll.a
        ${CMAKE_CURRENT_SOURCE_DIR}/OmniCore/Libs/libOmniCore.dll.a
    )
    add_custom_command(TARGET OmniERP POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "${ARCHIVE_DIR}"
        COMMAND ${CMAKE_COMMAND} -E make_directory "${INCLUDE_DIR}"
        COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/OmniCore/libOmniCore.dll" "${CORE_RUNTIME_DIR}/"
        COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/OmniUtils/libOmniUtils.dll" "${CORE_RUNTIME_DIR}/"
        COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/OmniUtils/libboost_container-mt.dll" "${CORE_RUNTIME_DIR}/"
        COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/OmniUtils/libboost_json-mt.dll" "${CORE_RUNTIME_DIR}/"
        COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/OmniUtils/libsodium-26.dll" "${CORE_RUNTIME_DIR}/"
        COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/OmniData/libOmniData.dll" "${CORE_RUNTIME_DIR}/"
        COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/OmniData/libboost_container-mt.dll" "${CORE_RUNTIME_DIR}/"
        COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/OmniData/libboost_json-mt.dll" "${CORE_RUNTIME_DIR}/"
        COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/OmniData/libsodium-26.dll" "${CORE_RUNTIME_DIR}/"
    )
elseif(APPLE)
    target_link_libraries(OmniERP PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/OmniUtils/libOmniUtils.dylib
        ${CMAKE_CURRENT_SOURCE_DIR}/OmniData/libOmniData.dylib
        ${CMAKE_CURRENT_SOURCE_DIR}/OmniCore/libOmniCore.dylib
    )
else()
    target_link_libraries(OmniERP PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/OmniUtils/libOmniUtils.so
        ${CMAKE_CURRENT_SOURCE_DIR}/OmniData/libOmniData.so
        ${CMAKE_CURRENT_SOURCE_DIR}/OmniCore/libOmniCore.so
    )
endif()

if(WIN32)
    set_target_properties(OmniERP PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CORE_RUNTIME_DIR}"
        LIBRARY_OUTPUT_DIRECTORY "${CORE_RUNTIME_DIR}"
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
