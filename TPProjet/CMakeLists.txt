# Un seul exécutable pour tout le projet, construit à partir de tous les .cpp du dossier
get_filename_component(DIR ${CMAKE_CURRENT_SOURCE_DIR} NAME)
set(OUTPUT ${DIR}_musee)

file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS *.cpp)
file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS *.hpp)

add_executable(${OUTPUT} ${SRC_FILES} ${HEADER_FILES})
target_link_libraries(${OUTPUT} glimac)

set_target_properties(${OUTPUT} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
)

if (MSVC)
    target_compile_options(${OUTPUT} PRIVATE /W3)
else()
    target_compile_options(${OUTPUT} PRIVATE -Wall -Wextra -Wpedantic)
endif()

include("../CMakeUtils/files_and_folders.cmake")
Cool__target_copy_folder(${OUTPUT} ../assets)
Cool__target_copy_folder(${OUTPUT} shaders)