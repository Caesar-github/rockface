cmake_minimum_required(VERSION 3.8.2)

project(rockface)

install(DIRECTORY sdk/rockface-${COMPILE_PLATFORM}/include/ DESTINATION include/rockface)
install(DIRECTORY sdk/rockface-${COMPILE_PLATFORM}/${COMPILE_LIB}/ DESTINATION lib)

if(${COMPILE_PLATFORM} STREQUAL "rv1109-Linux")
install(DIRECTORY sdk/rockface-data-rv1109/ DESTINATION lib)
else()
install(DIRECTORY sdk/rockface-data/ DESTINATION lib)
endif()
