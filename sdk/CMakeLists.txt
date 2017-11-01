file(GLOB_RECURSE sdk_SRC
    "src/*.h*"
    "src/*.cpp"
)

set(sdk_files ${sdk_SRC} PARENT_SCOPE)

include_directories("src/")

