#设置cmake版本
cmake_minimum_required(VERSION 3.15)
project(my_mprps)

#自动更新protobuf文件
# ===================== Protobuf 自动生成 =====================
find_package(Protobuf REQUIRED)
include_directories(${Protobuf_INCLUDE_DIRS})

# 自动生成 src/rpcheader.proto
execute_process(
    COMMAND protoc --cpp_out=${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/src/rpcheader.proto
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

# 自动生成 example/friend.proto
execute_process(
    COMMAND protoc --cpp_out=${PROJECT_SOURCE_DIR}/example ${PROJECT_SOURCE_DIR}/example/friend.proto
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

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
                