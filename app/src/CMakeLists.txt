add_executable(confManagerApplication1
    main.cpp
)

target_link_libraries(confManagerApplication1
    PRIVATE
    sdbus-c++
)
