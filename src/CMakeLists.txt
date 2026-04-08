#在源文件中生成动态库给别人用
aux_source_directory(. SRC_LIST)
add_library(mprpc STATIC ${SRC_LIST})

#链接一下线程库，protobuf内部用到了线程
target_link_libraries(mprpc protobuf pthread)
#链接muduo库
target_link_libraries(mprpc muduo_net muduo_base pthread zookeeper_mt)