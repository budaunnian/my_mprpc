#服务提供者也会生成一个可执行程序
set(SRC_LIST callfriendservice.cc ../friend.pb.cc)
add_executable(consumer ${SRC_LIST})

#链接mprpc和protobufe和线程库这个动态库
#target_link_libraries(consumer mprpc protobuf)
target_link_libraries(consumer mprpc protobuf pthread)