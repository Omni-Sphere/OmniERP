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
    Floor/Floor.cpp
    Floor/Repositories/Floor.cpp
    Area/Area.cpp
    Area/Repositories/Area.cpp
    Table/Table.cpp
    Table/Repositories/Table.cpp
)

target_include_directories(OmniERP
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Item>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/ItemGroup>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/ItemBrand>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Floor>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Area>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Table>
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
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_FILE:OmniERP>" "${CORE_RUNTIME_DIR}/"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_SOURCE_DIR}/OmniUtils/libOmniUtils.dll" "${CORE_RUNTIME_DIR}/"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_SOURCE_DIR}/OmniData/libOmniData.dll" "${CORE_RUNTIME_DIR}/"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_SOURCE_DIR}/OmniCore/libOmniCore.dll" "${CORE_RUNTIME_DIR}/"
        COMMENT "Copying OmniLibs DLLs to OmniCore root..."
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
