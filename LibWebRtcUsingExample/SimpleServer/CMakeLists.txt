FILe(GLOB SERVERSRC "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
add_executable(SimpleServer "${SERVERSRC}")

target_link_libraries(SimpleServer
    ${LOCAL_LINK_LIB} -levent
)

add_subdirectory(client)