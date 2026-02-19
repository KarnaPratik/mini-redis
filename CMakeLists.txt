cmake_minimum_required(VERSION 3.10)
project(mini_redis)

set(CMAKE_CXX_STANDARD 17)

add_definitions(-DCROW_USE_BOOST=0 -DASIO_STANDALONE)

# Find packages from vcpkg
find_package(Crow CONFIG REQUIRED)
find_package(asio CONFIG REQUIRED)

include_directories(include)

add_executable(mini_redis
    src/main.cpp
    src/lru_cache.cpp
)

target_link_libraries(mini_redis PRIVATE Crow::Crow asio::asio)

# target_compile_definitions(mini_redis PRIVATE
#     ASIO_STANDALONE
#     CROW_USE_BOOST=0
# )