include(FetchContent)
FetchContent_Declare(
        glpk
        DOWNLOAD_EXTRACT_TIMESTAMP ON
        URL http://ftp.gnu.org/gnu/glpk/glpk-5.0.tar.gz
)
FetchContent_MakeAvailable(glpk)

set(GLPK_LIB_PATH "${glpk_SOURCE_DIR}/src/.libs/libglpk.a")

add_custom_command(
        COMMAND "./configure"
        COMMAND "make"
        WORKING_DIRECTORY "${glpk_SOURCE_DIR}"
        OUTPUT "${GLPK_LIB_PATH}"
)
add_custom_target(GlpkBuild DEPENDS "${GLPK_LIB_PATH}")

add_library(LibGLPK STATIC IMPORTED)
set_target_properties(
        LibGLPK PROPERTIES
        IMPORTED_LOCATION "${GLPK_LIB_PATH}"
        INTERFACE_INCLUDE_DIRECTORIES "${glpk_SOURCE_DIR}/src"
)
add_dependencies(LibGLPK GlpkBuild)
