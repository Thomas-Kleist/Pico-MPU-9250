add_library(pico_mpu_9250 mpu9250.c mpu9250.h)

target_link_libraries(pico_mpu_9250 pico_stdlib hardware_spi)

target_include_directories(pico_mpu_9250 PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")


add_library(pico_mpu_9250_object mpu9250.c mpu9250.h mpuObject.cpp mpuObject.h)

target_link_libraries(pico_mpu_9250_object pico_stdlib hardware_spi)

target_include_directories(pico_mpu_9250_object PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")