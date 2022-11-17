add_compile_definitions(_POSIX_C_SOURCE=200809L _XOPEN_SOURCE=700)

if(APPLE)
    add_definitions(-D_DARWIN_C_SOURCE)
endif()

find_program(LINT "clang-tidy")
IF (LINT)
    set(CMAKE_C_CLANG_TIDY "clang-tidy;-checks=*,-llvmlibc-restrict-system-libc-headers,-cppcoreguidelines-init-variables,-clang-analyzer-security.insecureAPI.strcpy,-concurrency-mt-unsafe,-android-cloexec-accept,-android-cloexec-dup,-google-readability-todo,-cppcoreguidelines-avoid-magic-numbers,-readability-magic-numbers,-cert-dcl03-c,-hicpp-static-assert,-misc-static-assert,-altera-struct-pack-align,-clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling;--quiet")
ENDIF ()

# Make an executable
add_executable(PROXY ${COMMON_SOURCE_LIST}  ${PROXY_SOURCE_LIST} ${PROXY_MAIN_SOURCE} ${HEADER_LIST})
add_executable(SENDER ${COMMON_SOURCE_LIST} ${SENDER_MAIN_SOURCE} ${HEADER_LIST})
add_executable(RECEIVER ${COMMON_SOURCE_LIST} ${RECEIVER_MAIN_SOURCE} ${HEADER_LIST})

# We need this directory, and users of our library will need it too
target_include_directories(PROXY PRIVATE ../include)
target_include_directories(PROXY PRIVATE /usr/include)
target_include_directories(PROXY PRIVATE /usr/local/include)
target_link_directories(PROXY PRIVATE /usr/lib)
target_link_directories(PROXY PRIVATE /usr/local/lib)
target_include_directories(SENDER PRIVATE ../include)
target_include_directories(SENDER PRIVATE /usr/include)
target_include_directories(SENDER PRIVATE /usr/local/include)
target_link_directories(SENDER PRIVATE /usr/lib)
target_link_directories(SENDER PRIVATE /usr/local/lib)
target_include_directories(RECEIVER PRIVATE ../include)
target_include_directories(RECEIVER PRIVATE /usr/include)
target_include_directories(RECEIVER PRIVATE /usr/local/include)
target_link_directories(RECEIVER PRIVATE /usr/lib)
target_link_directories(RECEIVER PRIVATE /usr/local/lib)

# All users of this library will need at least C17
target_compile_features(PROXY PUBLIC c_std_17)
target_compile_options(PROXY PRIVATE -g)
target_compile_options(PROXY PRIVATE -fstack-protector-all -ftrapv)
target_compile_options(PROXY PRIVATE -Wpedantic -Wall -Wextra)
target_compile_options(PROXY PRIVATE -Wdouble-promotion -Wformat-nonliteral -Wformat-security -Wformat-y2k -Wnull-dereference -Winit-self -Wmissing-include-dirs -Wswitch-default -Wswitch-enum -Wunused-local-typedefs -Wstrict-overflow=5 -Wmissing-noreturn -Walloca -Wfloat-equal -Wdeclaration-after-statement -Wshadow -Wpointer-arith -Wabsolute-value -Wundef -Wexpansion-to-defined -Wunused-macros -Wno-endif-labels -Wbad-function-cast -Wcast-qual -Wwrite-strings -Wconversion -Wdangling-else -Wdate-time -Wempty-body -Wsign-conversion -Wfloat-conversion -Waggregate-return -Wstrict-prototypes -Wold-style-definition -Wmissing-prototypes -Wmissing-declarations -Wpacked -Wredundant-decls -Wnested-externs -Winline -Winvalid-pch -Wlong-long -Wvariadic-macros -Wdisabled-optimization -Wstack-protector -Woverlength-strings)
target_compile_features(SENDER PUBLIC c_std_17)
target_compile_options(SENDER PRIVATE -g)
target_compile_options(SENDER PRIVATE -fstack-protector-all -ftrapv)
target_compile_options(SENDER PRIVATE -Wpedantic -Wall -Wextra)
target_compile_options(SENDER PRIVATE -Wdouble-promotion -Wformat-nonliteral -Wformat-security -Wformat-y2k -Wnull-dereference -Winit-self -Wmissing-include-dirs -Wswitch-default -Wswitch-enum -Wunused-local-typedefs -Wstrict-overflow=5 -Wmissing-noreturn -Walloca -Wfloat-equal -Wdeclaration-after-statement -Wshadow -Wpointer-arith -Wabsolute-value -Wundef -Wexpansion-to-defined -Wunused-macros -Wno-endif-labels -Wbad-function-cast -Wcast-qual -Wwrite-strings -Wconversion -Wdangling-else -Wdate-time -Wempty-body -Wsign-conversion -Wfloat-conversion -Waggregate-return -Wstrict-prototypes -Wold-style-definition -Wmissing-prototypes -Wmissing-declarations -Wpacked -Wredundant-decls -Wnested-externs -Winline -Winvalid-pch -Wlong-long -Wvariadic-macros -Wdisabled-optimization -Wstack-protector -Woverlength-strings)
target_compile_features(RECEIVER PUBLIC c_std_17)
target_compile_options(RECEIVER PRIVATE -g)
target_compile_options(RECEIVER PRIVATE -fstack-protector-all -ftrapv)
target_compile_options(RECEIVER PRIVATE -Wpedantic -Wall -Wextra)
target_compile_options(RECEIVER PRIVATE -Wdouble-promotion -Wformat-nonliteral -Wformat-security -Wformat-y2k -Wnull-dereference -Winit-self -Wmissing-include-dirs -Wswitch-default -Wswitch-enum -Wunused-local-typedefs -Wstrict-overflow=5 -Wmissing-noreturn -Walloca -Wfloat-equal -Wdeclaration-after-statement -Wshadow -Wpointer-arith -Wabsolute-value -Wundef -Wexpansion-to-defined -Wunused-macros -Wno-endif-labels -Wbad-function-cast -Wcast-qual -Wwrite-strings -Wconversion -Wdangling-else -Wdate-time -Wempty-body -Wsign-conversion -Wfloat-conversion -Waggregate-return -Wstrict-prototypes -Wold-style-definition -Wmissing-prototypes -Wmissing-declarations -Wpacked -Wredundant-decls -Wnested-externs -Winline -Winvalid-pch -Wlong-long -Wvariadic-macros -Wdisabled-optimization -Wstack-protector -Woverlength-strings)

find_library(LIBM m REQUIRED)
find_library(LIBSOCKET socket)

target_link_libraries(PROXY PRIVATE ${LIBM})
target_link_libraries(SENDER PRIVATE ${LIBM})
target_link_libraries(RECEIVER PRIVATE ${LIBM})

set_target_properties(PROXY PROPERTIES OUTPUT_NAME "proxy")
set_target_properties(SENDER PROPERTIES OUTPUT_NAME "sender")
set_target_properties(RECEIVER PROPERTIES OUTPUT_NAME "receiver")
install(TARGETS PROXY DESTINATION bin)
install(TARGETS SENDER DESTINATION bin)
install(TARGETS RECEIVER DESTINATION bin)

# IDEs should put the headers in a nice place
source_group(
        TREE "${PROJECT_SOURCE_DIR}/include"
        PREFIX "Header Files"
        FILES ${HEADER_LIST}
)

add_custom_target(
        format
        COMMAND clang-format
        -i
        ${HEADER_LIST}
        ${COMMON_SOURCE_LIST}
        ${PROXY_SOURCE_LIST}
        ${PROXY_MAIN_SOURCE}
        ${SENDER_MAIN_SOURCE}
        ${RECEIVER_MAIN_SOURCE}
)