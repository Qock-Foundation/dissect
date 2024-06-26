include(FetchContent)
FetchContent_Declare(
        kissat
        GIT_REPOSITORY https://github.com/arminbiere/kissat.git
        GIT_TAG rel-3.1.1
)
FetchContent_MakeAvailable(kissat)

set(KISSAT_EXECUTABLE "${kissat_SOURCE_DIR}/build/kissat")

add_custom_command(
        COMMAND "./configure"
        COMMAND "make"
        WORKING_DIRECTORY "${kissat_SOURCE_DIR}"
        OUTPUT "${KISSAT_EXECUTABLE}"
)
add_custom_target(KissatBuild DEPENDS "${KISSAT_EXECUTABLE}")
add_compile_definitions(KISSAT_EXECUTABLE="${KISSAT_EXECUTABLE}")
