# 强制直接寻找名为 clang-format 的程序
find_program(CLANG_FORMAT_EXECUTABLE NAMES clang-format)

if (CLANG_FORMAT_EXECUTABLE)
    file (GLOB_RECURSE ALL_CC_FILES *.cc)
    file (GLOB_RECURSE ALL_HH_FILES *.hh)
    add_custom_target (format ${CLANG_FORMAT_EXECUTABLE} -i ${ALL_CC_FILES} ${ALL_HH_FILES} COMMENT "Formatted all source files.")
    message(STATUS "Found clang-format: ${CLANG_FORMAT_EXECUTABLE}")
else ()
    add_custom_target (format echo "Could not find clang-format. Please install and re-run cmake")
endif ()