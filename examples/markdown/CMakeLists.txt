qt_add_executable(example-markdown
        MANUAL_FINALIZATION
        main.cpp
        ${PROJECT_SOURCE_DIR}/src/markdown/mdtransform.hpp
        ${PROJECT_SOURCE_DIR}/src/markdown/mdtransform.cpp)

target_link_libraries(example-markdown PRIVATE Qt${QT_VERSION_MAJOR}::Core)