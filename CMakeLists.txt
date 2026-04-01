cmake_minimum_required(VERSION 3.24)
project(AGPEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#
# Target: AGPBaseAPI
#
if(NOT TARGET AGPBase::AGPBaseAPI)
  add_subdirectory(LumeBase)
endif()

#
# Target: AGPEngineAPI
#
add_library(AGPEngineAPI INTERFACE)
add_library(AGPEngine::AGPEngineAPI ALIAS AGPEngineAPI)
target_sources(AGPEngineAPI INTERFACE
  ${CMAKE_CURRENT_SOURCE_DIR}/api/core/property_tools/property_api_impl.inl
  ${CMAKE_CURRENT_SOURCE_DIR}/api/core/property_tools/property_data.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/api/core/property_tools/core_metadata.inl)
target_include_directories(AGPEngineAPI INTERFACE
  ${CMAKE_CURRENT_SOURCE_DIR}/api
  ${CMAKE_CURRENT_SOURCE_DIR}/api/platform/common)
target_link_libraries(AGPEngineAPI INTERFACE AGPBase::AGPBaseAPI)
target_compile_definitions(AGPEngineAPI INTERFACE CORE_USE_COMPILER_GENERATED_STATIC_LIST=1)
target_compile_definitions(AGPEngineAPI INTERFACE CORE_PERF_ENABLED=0)

#
# Target: ComponentHelperSDK
#
add_library(ComponentHelperSDK INTERFACE)
add_library(AGPEngine::ComponentHelperSDK ALIAS ComponentHelperSDK)
target_sources(ComponentHelperSDK INTERFACE
  ${CMAKE_CURRENT_SOURCE_DIR}/ecshelper/ComponentTools/base_manager.inl
  ${CMAKE_CURRENT_SOURCE_DIR}/ecshelper/ComponentTools/component_query.cpp)
target_include_directories(ComponentHelperSDK INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/ecshelper)

#
# Target: AGPEngine
#
file(GLOB_RECURSE AGPEngine_sources "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
list(FILTER AGPEngine_sources EXCLUDE REGEX ".*/src/os/ohos/.*")
add_library(AGPEngine STATIC ${AGPEngine_sources})
add_library(AGPEngine::AGPEngine ALIAS AGPEngine)
target_include_directories(AGPEngine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/)
target_include_directories(AGPEngine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/third_party/stb/)
target_link_libraries(AGPEngine PRIVATE
  AGPBase::AGPBaseAPI
  AGPEngine::AGPEngineAPI
  ComponentHelperSDK)
target_compile_definitions(AGPEngine PRIVATE USE_STB_IMAGE=1)
target_compile_definitions(AGPEngine PRIVATE CORE_DEV_ENABLED=0)
target_compile_definitions(AGPEngine PRIVATE CORE_LOG_TO_CONSOLE=1)
target_compile_definitions(AGPEngine PRIVATE CORE_LOG_ TO_DEBUG_OUTPUT=1)
target_compile_definitions(AGPEngine PRIVATE CORE_LOG_TO_FILE=0)
target_compile_definitions(AGPEngine PRIVATE CORE_EMBEDDED_ASSETS_ENABLED=0)
target_compile_definitions(AGPEngine PRIVATE CORE_VALIDATION_ENABLED=1)
target_compile_definitions(AGPEngine PRIVATE CORE_BUILDTYPE="$<CONFIG>")
target_compile_definitions(AGPEngine PRIVATE "CORE_PUBLIC=__declspec(dllexport)" "WIN32_LEAN_AND_MEAN=1")
target_compile_definitions(AGPEngine PUBLIC _CRT_SECURE_NO_WARNINGS)
target_compile_definitions(AGPEngine PUBLIC NOMINMAX)
target_compile_options(AGPEngine PRIVATE /GR-)

#
# Target: AGPEngineDLL
#
add_library(AGPEngineDLL SHARED
  ${CMAKE_CURRENT_SOURCE_DIR}/DLL/src/agpengine_64.def
  ${CMAKE_CURRENT_SOURCE_DIR}/DLL/src/exports.cpp)
add_library(AGPEngine::AGPEngineDLL ALIAS AGPEngineDLL)
target_link_libraries(AGPEngineDLL
  INTERFACE AGPEngine::AGPEngineAPI
  PRIVATE AGPEngine::AGPEngine)
target_compile_definitions(AGPEngineDLL INTERFACE "CORE_PUBLIC=__declspec(dllimport)")
target_compile_options(AGPEngineDLL PRIVATE /GR-)