# SPDX-License-Identifier: MIT

# Function for generating a ready to use pkg-config file that is
# automatically installed alongside the other library files.

# Notes:
#  NAME is optional and allows installation of multiple .pc files if used
#  CFLAGS, LIBS and LIBS_PRIVATE take normal gcc compiler flags, e.g. "-lxyz"
#  REQUIRES is a package name plus optional version, e.g. "foo >= 0.1"
#    Multiple packages can be set here, e.g. "foo >= 0.1, bar = 0.2"
#
# See https://people.freedesktop.org/~dbn/pkg-config-guide.html for more info

function(create_pkgconfig)
  cmake_parse_arguments(PARSED_ARGS "" "NAME" "REQUIRES;REQUIRES_PRIVATE;CFLAGS;LIBS;LIBS_PRIVATE" ${ARGN})

  if(PARSED_ARGS_NAME)
    set(pkgconfig_name "${PARSED_ARGS_NAME}")
  else()
    set(pkgconfig_name "${PROJECT_NAME}")
  endif()

  set(pkgconfig_requires "${PARSED_ARGS_REQUIRES}")
  set(pkgconfig_requires_private "${PARSED_ARGS_REQUIRES_PRIVATE}")
  set(pkgconfig_cflags "${PARSED_ARGS_CFLAGS}")
  set(pkgconfig_libs "${PARSED_ARGS_LIBS}")
  set(pkgconfig_libs_private "${PARSED_ARGS_LIBS_PRIVATE}")

  configure_file(
    ${PROJECT_SOURCE_DIR}/cmake/pkgconfig/template.pc.in
    ${CMAKE_CURRENT_BINARY_DIR}/${pkgconfig_name}.pc
    @ONLY
  )

  install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/${pkgconfig_name}.pc
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
  )

endfunction()
