cmake_minimum_required(VERSION 3.16)

project(cNV LANGUAGES C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(NV_DEBUG_LOG "nv debug log" ON)

option(NV_DEBUG_MOCK_DATA "nv debug mock data" ON)

set(SOURCE nv/nv.c cJSON/cJSON.c cJSON/cJSON_Utils.c test.c)

set(NV_DATA_BUFFER_SIZE "1024" CACHE STRING "")

add_executable(cNV ${SOURCE})

if (NV_DEBUG_LOG)
    target_compile_definitions(${PROJECT_NAME} PUBLIC -DCONFIG_NV_DEBUG_LOG=1)
endif ()

if (NV_DEBUG_MOCK_DATA)
    target_compile_definitions(${PROJECT_NAME}
            PUBLIC -DCONFIG_NV_DEBUG_MOCK_DATA=1)

    target_compile_definitions(${PROJECT_NAME}
            PUBLIC -DCONFIG_NV_PATH="./nv.json")
endif ()

add_compile_options(-Wall -Wextra -Werror -Wno-format -g)

target_include_directories(cNV PRIVATE "${CMAKE_SOURCE_DIR}/nv/")
target_include_directories(cNV PRIVATE "${CMAKE_SOURCE_DIR}/cJSON/")

target_compile_definitions(cNV PUBLIC -DCONFIG_NV_DATA_BUFFER_SIZE=${NV_DATA_BUFFER_SIZE})

option(ENABLE_SANITIZER "Enables sanitizer" ON)
if (ENABLE_SANITIZER)
    target_compile_options(
            ${PROJECT_NAME}
            PUBLIC -fsanitize=address
            -fno-omit-frame-pointer
            -fsanitize=undefined
            -fsanitize=float-cast-overflow
            -fsanitize-address-use-after-scope)
    target_link_options(${PROJECT_NAME} PUBLIC -fsanitize=address)
    target_link_options(${PROJECT_NAME} PUBLIC -fsanitize=undefined)
endif ()
