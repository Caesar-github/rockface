cmake_minimum_required(VERSION 3.8.2)

project(rockface)

install(DIRECTORY sdk/rockface-${COMPILE_PLATFORM}/include/ DESTINATION include/rockface)
install(DIRECTORY sdk/rockface-${COMPILE_PLATFORM}/${COMPILE_LIB}/ DESTINATION lib)
install(DIRECTORY sdk/rockface-data/ DESTINATION lib)
