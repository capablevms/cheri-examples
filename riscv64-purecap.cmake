set(CMAKE_SYSTEM_NAME Linux)

# Default SDK path
set(SDK "$ENV{HOME}/cheri/output/sdk" CACHE PATH "path to cheri SDK")

# Set toolchain compilers
set(CMAKE_C_COMPILER ${SDK}/bin/riscv64-unknown-freebsd13-cc)
set(CMAKE_CXX_COMPILER ${SDK}/bin/riscv64-unknown-freebsd13-c++)

# Don't run the linker on compiler check
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# Define sysroot path for CHERI-build`
set(CMAKE_SYSROOT ${SDK}/sysroot-riscv64-purecap)

# Use only cross compiler tools for compilation and linking
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# Set correct machine and abi flags
add_compile_options(-march=rv64imafdcxcheri -mabi=l64pc128d -mno-relax)
add_link_options(-march=rv64imafdcxcheri -mabi=l64pc128d -mno-relax)
