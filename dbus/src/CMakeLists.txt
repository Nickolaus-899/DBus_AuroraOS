add_executable(configurationManager
    main.cpp
)

target_link_libraries(configurationManager
    PRIVATE
    sdbus-c++
)

