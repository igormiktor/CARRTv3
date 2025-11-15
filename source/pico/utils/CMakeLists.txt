# Various Pico-specific utilities

add_library( utils_library INTERFACE )

target_sources( utils_library
    PRIVATE
        # Nothing at this time
    PUBLIC FILE_SET HEADERS FILES
        CoreAtomic.hpp
        CriticalSection.h
)

target_include_directories( utils_library INTERFACE "${PROJECT_SOURCE_DIR}/utils" )

# Compile definitions and options inhereted from link libs (shared_library is the "root")

target_link_libraries( utils_library INTERFACE shared_library )

