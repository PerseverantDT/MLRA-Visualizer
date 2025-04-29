cmake_minimum_required(VERSION 3.28) # Lowered for broader compatibility unless 3.30 is essential
project(mlra-visualizer
    LANGUAGES C
    VERSION 0.0.0
)

# --- Compiler/Standard Settings ---
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED true)

# --- Performance Options ---
include(CheckIPOSupported)
check_ipo_supported(RESULT IPO_SUPPORTED OUTPUT IPO_OUTPUT)
if(IPO_SUPPORTED)
    message(STATUS "IPO/LTO potentially enabled for Release builds.")
    set(ENABLE_IPO true)
else()
    message(WARNING "IPO/LTO not supported by toolchain: ${IPO_OUTPUT}")
    set(ENABLE_IPO false)
endif()

option(MLRA_TUNE_FOR_HOST_MACHINE "Enable architecture specific optimizations (-march=native)" OFF) 
option(MLRA_TUNE_FOR_AVX2 "Force AVX2 instructions (-mavx2) - WARNING: requires AVX2 CPU" OFF)

# --- Compilation Options ---
option(MLRA_ENABLE_ADDITIONAL_WARNINGS "Check for additional warnings during compilation. May cause noisy output." OFF)
option(MLRA_ENABLE_ANALYZER "Enable GCC Static Analyzer (slows build)" OFF)

# --- Dependencies ---
find_package(raylib CONFIG REQUIRED)
find_path(RAYGUI_INCLUDE_DIRS "raygui.h")

# --- Executable Definition ---
add_executable(mlra-visualizer
    src/main.c
    src/Core/RegisterCost.c
    src/Core/RegisterInstruction.c
    src/Core/Scenario.c
)

add_custom_command(TARGET mlra-visualizer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/assets $<TARGET_FILE_DIR:mlra-visualizer>/assets
    VERBATIM
)

target_include_directories(mlra-visualizer PRIVATE
    inc
)

# --- Common Compile Options ---
target_compile_options(mlra-visualizer PRIVATE
    # Base Warnings
    -Wall -Wextra -Wpedantic
    -fno-ms-extensions
)
if (MLRA_ENABLE_ADDITIONAL_WARNINGS)
    target_compile_options(mlra-visualizer PRIVATE
        # Additional Warnings
        -Wformat=2
        -Wnull-dereference
        -Wimplicit-fallthrough
        -Wshift-overflow=2
        -Wswitch-default
        -Wuse-after-free=3
        -Wuninitialized
        -Wstrict-flex-arrays
        -fstrict-flex-arrays=3
        -Wsuggest-attribute=pure
        -Wsuggest-attribute=const
        -Wsuggest-attribute=noreturn
        -Wsuggest-attribute=malloc
        -Wsuggest-attribute=returns_nonnull
        -Wsuggest-attribute=format
        -Wsuggest-attribute=cold
        -Walloc-size
        -Walloc-zero
        -Wcalloc-transposed-args
        -Wattribute-alias=2
        -Wbidi-chars=any,ucn
        -Wduplicated-branches
        -Wduplicated-cond
        -Wtrampolines
        -Wfloat-equal
        -Wshadow
        -Wstack-usage=1024
        -Wunsafe-loop-optimizations
        -Wundef
        -Wbad-function-cast
        -Wcast-qual
        -Wcast-align=strict
        -Wwrite-strings
        -Wconversion
        -Wdate-time
        -Wjump-misses-init
        -Wflex-array-member-not-at-end
        -Wlogical-op
        -Wno-attributes
        -Wstrict-prototypes
        -Wmissing-prototypes
        -Wmissing-variable-declarations
        -Wmissing-declarations
        -Wnormalized
        -Wrestrict
        -Winline
        -Wno-pointer-to-int-cast
        -Wdisabled-optimization
        -Werror=implicit-function-declaration
    )
endif ()

# --- Build Type Specific Options ---
# Optimization level
target_compile_options(mlra-visualizer PRIVATE
    $<$<CONFIG:Release>:-O3>
)

# IPO for Release
if(MLRA_ENABLE_IPO)
    set_property(TARGET mlra-visualizer PROPERTY INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)
    target_link_options(mlra-visualizer PRIVATE $<$<CONFIG:Release>:-fuse-linker-plugin>)
endif()

# Architecture specific optimisations (Apply to optimized builds)
if(MLRA_TUNE_FOR_HOST_MACHINE)
    message(STATUS "Optimized builds tuned for host machine.")
    target_compile_options(mlra-visualizer PRIVATE $<$<CONFIG:Release>:-march=native>)
elseif(MLRA_ENABLE_AVX2)
    message(STATUS "AVX2 optimization enabled.")
    target_compile_options(mlra-visualizer PRIVATE $<$<CONFIG:Release>:-mavx2>)
endif()


# Static Analyzer
if(MLRA_ENABLE_ANALYZER)
    target_compile_options(mlra-visualizer PRIVATE
        -fanalyzer
        -Wanalyzer-symbol-too-complex
        -Wanalyzer-too-complex
    )
endif()


# --- Linking ---
target_link_libraries(mlra-visualizer PRIVATE
    raylib
)
target_include_directories(mlra-visualizer PRIVATE
    ${RAYGUI_INCLUDE_DIRS}
)
