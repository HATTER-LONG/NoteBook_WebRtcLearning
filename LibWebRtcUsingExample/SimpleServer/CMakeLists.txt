FILe(GLOB SERVERSRC "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
add_executable(MeidaServer "${SERVERSRC}")

target_link_libraries( SimpleServer
    ${LOCAL_LINK_LIB} -levent
)