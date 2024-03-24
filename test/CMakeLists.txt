add_executable(datagen "datagen.cpp")

add_executable(preload "preload.cpp")
target_link_libraries(preload pmallocator)

