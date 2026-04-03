#在源文件中生成动态库给别人用
aux_source_directory(. SRC_LIST)
add_library(mprpc SHARED ${SRC_LIST})

#链接一下线程库，protobuf内部用到了线程
target_link_libraries(mprpc protobuf pthread)