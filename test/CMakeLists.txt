add_executable(datagen "datagen.cpp")

add_executable(preload "preload.cpp")
target_link_libraries(preload pmallocator)

add_executable(main "main.cpp")
target_link_libraries(main pmallocator)

add_executable(single_test "single_test.cpp")
target_link_libraries(single_test pmallocator)

