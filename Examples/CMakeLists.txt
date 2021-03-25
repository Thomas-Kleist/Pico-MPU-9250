add_executable(eulerAngleExample eulerAngleExample.c)

# Pull in our pico_stdlib which pulls in commonly used features
target_link_libraries(eulerAngleExample PRIVATE pico_stdlib pico_mpu_9250)

pico_enable_stdio_usb(eulerAngleExample 1)
pico_enable_stdio_uart(eulerAngleExample 0)

# create map/bin/hex file etc.
pico_add_extra_outputs(eulerAngleExample)

# add url via pico_set_program_url
example_auto_set_url(eulerAngleExample)


add_executable(objectExample objectExample.cpp)

# Pull in our pico_stdlib which pulls in commonly used features
target_link_libraries(objectExample PRIVATE pico_stdlib pico_mpu_9250_object)

pico_enable_stdio_usb(objectExample 1)
pico_enable_stdio_uart(objectExample 0)

# create map/bin/hex file etc.
pico_add_extra_outputs(objectExample)

# add url via pico_set_program_url
example_auto_set_url(objectExample)