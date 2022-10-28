add_compile_definitions(_POSIX_C_SOURCE=200809L _XOPEN_SOURCE=700)

if(APPLE)
    add_definitions(-D_DARWIN_C_SOURCE)
endif()

find_program(LINT "clang-tidy")
IF (LINT)
    set(CMAKE_C_CLANG_TIDY "clang-tidy;-checks=*,-llvmlibc-restrict-system-libc-headers,-cppcoreguidelines-init-variables,-clang-analyzer-security.insecureAPI.strcpy,-concurrency-mt-unsafe,-android-cloexec-accept,-android-cloexec-dup,-google-readability-todo,-cppcoreguidelines-avoid-magic-numbers,-readability-magic-numbers,-cert-dcl03-c,-hicpp-static-assert,-misc-static-assert,-altera-struct-pack-align,-clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling;--quiet")
ENDIF ()

# Make an executable
add_executable(SERVER ${COMMON_SOURCE_LIST}  ${SERVER_SOURCE_LIST} ${SERVER_MAIN_SOURCE} ${HEADER_LIST})
add_executable(CLIENTA ${COMMON_SOURCE_LIST}  ${CLIENT_SOURCE_LIST} ${CLIENTA_MAIN_SOURCE} ${HEADER_LIST})
add_executable(CLIENTB ${COMMON_SOURCE_LIST}  ${CLIENT_SOURCE_LIST} ${CLIENTB_MAIN_SOURCE} ${HEADER_LIST})

# We need this directory, and users of our library will need it too
target_include_directories(SERVER PRIVATE ../include)
target_include_directories(SERVER PRIVATE /usr/include)
target_include_directories(SERVER PRIVATE /usr/local/include)
target_link_directories(SERVER PRIVATE /usr/lib)
target_link_directories(SERVER PRIVATE /usr/local/lib)
target_include_directories(CLIENTA PRIVATE ../include)
target_include_directories(CLIENTA PRIVATE /usr/include)
target_include_directories(CLIENTA PRIVATE /usr/local/include)
target_link_directories(CLIENTA PRIVATE /usr/lib)
target_link_directories(CLIENTA PRIVATE /usr/local/lib)
target_include_directories(CLIENTB PRIVATE ../include)
target_include_directories(CLIENTB PRIVATE /usr/include)
target_include_directories(CLIENTB PRIVATE /usr/local/include)
target_link_directories(CLIENTB PRIVATE /usr/lib)
target_link_directories(CLIENTB PRIVATE /usr/local/lib)

# All users of this library will need at least C17
target_compile_features(SERVER PUBLIC c_std_17)
target_compile_options(SERVER PRIVATE -g)
target_compile_options(SERVER PRIVATE -fstack-protector-all -ftrapv)
target_compile_options(SERVER PRIVATE -Wpedantic -Wall -Wextra)
target_compile_options(SERVER PRIVATE -Wdouble-promotion -Wformat-nonliteral -Wformat-security -Wformat-y2k -Wnull-dereference -Winit-self -Wmissing-include-dirs -Wswitch-default -Wswitch-enum -Wunused-local-typedefs -Wstrict-overflow=5 -Wmissing-noreturn -Walloca -Wfloat-equal -Wdeclaration-after-statement -Wshadow -Wpointer-arith -Wabsolute-value -Wundef -Wexpansion-to-defined -Wunused-macros -Wno-endif-labels -Wbad-function-cast -Wcast-qual -Wwrite-strings -Wconversion -Wdangling-else -Wdate-time -Wempty-body -Wsign-conversion -Wfloat-conversion -Waggregate-return -Wstrict-prototypes -Wold-style-definition -Wmissing-prototypes -Wmissing-declarations -Wpacked -Wredundant-decls -Wnested-externs -Winline -Winvalid-pch -Wlong-long -Wvariadic-macros -Wdisabled-optimization -Wstack-protector -Woverlength-strings)
target_compile_features(CLIENTA PUBLIC c_std_17)
target_compile_options(CLIENTA PRIVATE -g)
target_compile_options(CLIENTA PRIVATE -fstack-protector-all -ftrapv)
target_compile_options(CLIENTA PRIVATE -Wpedantic -Wall -Wextra)
target_compile_options(CLIENTA PRIVATE -Wdouble-promotion -Wformat-nonliteral -Wformat-security -Wformat-y2k -Wnull-dereference -Winit-self -Wmissing-include-dirs -Wswitch-default -Wswitch-enum -Wunused-local-typedefs -Wstrict-overflow=5 -Wmissing-noreturn -Walloca -Wfloat-equal -Wdeclaration-after-statement -Wshadow -Wpointer-arith -Wabsolute-value -Wundef -Wexpansion-to-defined -Wunused-macros -Wno-endif-labels -Wbad-function-cast -Wcast-qual -Wwrite-strings -Wconversion -Wdangling-else -Wdate-time -Wempty-body -Wsign-conversion -Wfloat-conversion -Waggregate-return -Wstrict-prototypes -Wold-style-definition -Wmissing-prototypes -Wmissing-declarations -Wpacked -Wredundant-decls -Wnested-externs -Winline -Winvalid-pch -Wlong-long -Wvariadic-macros -Wdisabled-optimization -Wstack-protector -Woverlength-strings)
target_compile_features(CLIENTB PUBLIC c_std_17)
target_compile_options(CLIENTB PRIVATE -g)
target_compile_options(CLIENTB PRIVATE -fstack-protector-all -ftrapv)
target_compile_options(CLIENTB PRIVATE -Wpedantic -Wall -Wextra)
target_compile_options(CLIENTB PRIVATE -Wdouble-promotion -Wformat-nonliteral -Wformat-security -Wformat-y2k -Wnull-dereference -Winit-self -Wmissing-include-dirs -Wswitch-default -Wswitch-enum -Wunused-local-typedefs -Wstrict-overflow=5 -Wmissing-noreturn -Walloca -Wfloat-equal -Wdeclaration-after-statement -Wshadow -Wpointer-arith -Wabsolute-value -Wundef -Wexpansion-to-defined -Wunused-macros -Wno-endif-labels -Wbad-function-cast -Wcast-qual -Wwrite-strings -Wconversion -Wdangling-else -Wdate-time -Wempty-body -Wsign-conversion -Wfloat-conversion -Waggregate-return -Wstrict-prototypes -Wold-style-definition -Wmissing-prototypes -Wmissing-declarations -Wpacked -Wredundant-decls -Wnested-externs -Winline -Winvalid-pch -Wlong-long -Wvariadic-macros -Wdisabled-optimization -Wstack-protector -Woverlength-strings)

find_library(LIBM m REQUIRED)
find_library(LIBSOCKET socket)

target_link_libraries(SERVER PRIVATE ${LIBM})
target_link_libraries(CLIENTA PRIVATE ${LIBM})
target_link_libraries(CLIENTB PRIVATE ${LIBM})

set_target_properties(SERVER PROPERTIES OUTPUT_NAME "server")
set_target_properties(CLIENTA PROPERTIES OUTPUT_NAME "clientA")
set_target_properties(CLIENTB PROPERTIES OUTPUT_NAME "clientB")
install(TARGETS SERVER DESTINATION bin)
install(TARGETS CLIENTA DESTINATION bin)
install(TARGETS CLIENTB DESTINATION bin)

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
        ${SERVER_SOURCE_LIST}
        ${CLIENT_SOURCE_LIST}
        ${SERVER_MAIN_SOURCE}
        ${CLIENTA_MAIN_SOURCE}
        ${CLIENTB_MAIN_SOURCE}
)