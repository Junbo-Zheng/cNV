cmake_minimum_required(VERSION 3.16)

project(cNV LANGUAGES C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(NV_DEBUG_LOG "nv debug log" ON)
option(ENABLE_SANITIZER "Enables sanitizer" ON)
option(NV_DEBUG_MOCK_DATA "nv debug mock data" ON)

set(SOURCE nv/nv.c cJSON/cJSON.c cJSON/cJSON_Utils.c test.c)
set(NV_DATA_BUFFER_SIZE
    "1024"
    CACHE STRING "")

add_executable(${PROJECT_NAME} ${SOURCE})

if(NV_DEBUG_LOG)
  target_compile_definitions(${PROJECT_NAME} PUBLIC -DCONFIG_NV_DEBUG_LOG=1)
endif()

if(NV_DEBUG_MOCK_DATA)
  target_compile_definitions(${PROJECT_NAME}
                             PUBLIC -DCONFIG_NV_DEBUG_MOCK_DATA=1)

  target_compile_definitions(${PROJECT_NAME}
                             PUBLIC -DCONFIG_NV_PATH="./nv.json")
endif()

target_compile_definitions(
  ${PROJECT_NAME} PUBLIC -DCONFIG_NV_DATA_BUFFER_SIZE=${NV_DATA_BUFFER_SIZE})

add_compile_options(-Wall -Wextra -Werror -Wno-format -g)

target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/nv/")
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/cJSON/")

if(ENABLE_SANITIZER)
  add_compile_options(-fsanitize=address)
  add_compile_options(-fsanitize=undefined)
  add_compile_options(-fno-omit-frame-pointer)
  add_compile_options(-fsanitize-address-use-after-scope)
endif()
