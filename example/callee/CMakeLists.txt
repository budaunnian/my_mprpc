#服务提供者也会生成一个可执行程序
set(SRC_LIST userservice.cc ../user.pb.cc)
add_executable(provider ${SRC_LIST})

#链接mprpc和protobufe和线程库这个动态库
#target_link_libraries(provider mprpc protobuf)
target_link_libraries(provider mprpc protobuf pthread)