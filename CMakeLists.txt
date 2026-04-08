#设置cmake版本
cmake_minimum_required(VERSION 3.15)
project(my_mprps)

# ===================== Protobuf 自动生成 =====================

# 1. 强制查找 Protobuf
find_package(Protobuf REQUIRED)
if(NOT Protobuf_FOUND)
    message(FATAL_ERROR "Protobuf not found! Please install it first.")
endif()

message(STATUS "Protobuf include dir: ${Protobuf_INCLUDE_DIRS}")
message(STATUS "Protobuf libraries: ${Protobuf_LIBRARIES}")

# 2. 包含头文件路径
include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR}/src)

# 3. 自动生成代码（修复版：加上 -I 参数）
# 生成 src/rpcheader.pb.h/cc
execute_process(
    COMMAND ${Protobuf_PROTOC_EXECUTABLE}
        -I ${PROJECT_SOURCE_DIR}/src          # 🔴 关键修复：指定 .proto 搜索路径
        --cpp_out=${PROJECT_SOURCE_DIR}/src
        rpcheader.proto                        # 🔴 这里用相对路径（配合 -I）
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/src
    RESULT_VARIABLE PROTOC_RESULT
)
if(NOT PROTOC_RESULT EQUAL 0)
    message(FATAL_ERROR "Failed to generate rpcheader.pb.h/cc!")
endif()

# 生成 example/friend.pb.h/cc
execute_process(
    COMMAND ${Protobuf_PROTOC_EXECUTABLE}
        -I ${PROJECT_SOURCE_DIR}/example      # 🔴 关键修复：指定 .proto 搜索路径
        --cpp_out=${PROJECT_SOURCE_DIR}/example
        friend.proto                           # 🔴 这里用相对路径
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/example
    RESULT_VARIABLE PROTOC_RESULT
)
if(NOT PROTOC_RESULT EQUAL 0)
    message(FATAL_ERROR "Failed to generate friend.pb.h/cc!")
endif()

#设置可执行文件的输出路径  ${PROJECT_SOURCE_DIR}cmake文件所在的路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

#设置项目的库文件的输出路径
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

#设置编译文件的搜索路径，也就是.cpp文件所存放的路径
include_directories(${PROJECT_SOURCE_DIR}/src/include)
include_directories(${PROJECT_SOURCE_DIR}/example)

#设置项目库文件的搜索路径
link_directories(${PROJECT_SOURCE_DIR}/lib)

#关联子目录中的cmake文件
add_subdirectory(src)
add_subdirectory(example)
                