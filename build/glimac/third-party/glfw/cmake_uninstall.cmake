
if (NOT EXISTS "C:/Users/aya20/Desktop/Master Gystave Eiffel/M2 cours/SyntheseImage3D/Projet/Projet_SynthImage/build/glimac/third-party/glfw/install_manifest.txt")
    message(FATAL_ERROR "Cannot find install manifest: \"C:/Users/aya20/Desktop/Master Gystave Eiffel/M2 cours/SyntheseImage3D/Projet/Projet_SynthImage/build/glimac/third-party/glfw/install_manifest.txt\"")
endif()

file(READ "C:/Users/aya20/Desktop/Master Gystave Eiffel/M2 cours/SyntheseImage3D/Projet/Projet_SynthImage/build/glimac/third-party/glfw/install_manifest.txt" files)
string(REGEX REPLACE "\n" ";" files "${files}")

foreach (file ${files})
  message(STATUS "Uninstalling \"$ENV{DESTDIR}${file}\"")
  if (EXISTS "$ENV{DESTDIR}${file}")
    exec_program("C:/Users/aya20/miniconda3/Library/bin/cmake.exe" ARGS "-E remove \"$ENV{DESTDIR}${file}\""
                 OUTPUT_VARIABLE rm_out
                 RETURN_VALUE rm_retval)
    if (NOT "${rm_retval}" STREQUAL 0)
      MESSAGE(FATAL_ERROR "Problem when removing \"$ENV{DESTDIR}${file}\"")
    endif()
  elseif (IS_SYMLINK "$ENV{DESTDIR}${file}")
    EXEC_PROGRAM("C:/Users/aya20/miniconda3/Library/bin/cmake.exe" ARGS "-E remove \"$ENV{DESTDIR}${file}\""
                 OUTPUT_VARIABLE rm_out
                 RETURN_VALUE rm_retval)
    if (NOT "${rm_retval}" STREQUAL 0)
      message(FATAL_ERROR "Problem when removing symlink \"$ENV{DESTDIR}${file}\"")
    endif()
  else()
    message(STATUS "File \"$ENV{DESTDIR}${file}\" does not exist.")
  endif()
endforeach()

