FILe(GLOB SERVERSRC "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
add_executable(SimpleClient "${SERVERSRC}")

target_link_libraries(SimpleClient
    ${LOCAL_LINK_LIB}
)