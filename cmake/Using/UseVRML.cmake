MACRO(USE_VRML)
  set(MISSING_DEPS "")
  if (NOT JPEGTURBO_FOUND)
    if (NOT JPEG_FOUND)
      set(MISSING_DEPS "${MISSING_DEPS} JPEG")
    endif()
  endif()
  if (NOT PNG_FOUND)
    set(MISSING_DEPS "${MISSING_DEPS} PNG")
  endif()
  if (NOT TIFF_FOUND)
    set(MISSING_DEPS "${MISSING_DEPS} TIFF")
  endif()
  if (NOT ZLIB_FOUND)
    set(MISSING_DEPS "${MISSING_DEPS} ZLIB")
  endif()
  if (NOT FLEX_FOUND)
    set(MISSING_DEPS "${MISSING_DEPS} FLEX")
  endif()
  if (NOT BISON_FOUND)
    set(MISSING_DEPS "${MISSING_DEPS} BISON")
  endif()

  if (NOT "${MISSING_DEPS}" STREQUAL "")
    if (${ARGC} LESS 1)
        using_message("Skipping because of missing VRML dependencies:${MISSING_DEPS}")
      return()
    endif()
  else()
    if(NOT VRML_USED)
      set(VRML_USED TRUE)
      include_directories(${COVISEDIR}/src/OpenCOVER)
      set(EXTRA_LIBS ${EXTRA_LIBS} coVRML)
    endif()
  endif()
ENDMACRO(USE_VRML)
