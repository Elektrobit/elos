# SPDX-License-Identifier: MIT

if (INSTALL_ELOSD_SYSTEMD_UNIT)
  if (NOT ELOSD_SYSTEMD)
    message(FATAL_ERROR "elosd is configured without systemd support, cannot use default service unit!")
  endif()
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/systemd/elosd.service.in
    ${CMAKE_CURRENT_BINARY_DIR}/elosd.service
    @ONLY
  )

  install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/elosd.service
    DESTINATION ${INSTALL_ELOSD_SYSTEMD_UNIT_DIR}
  )
endif()

if (INSTALL_ELOSD_SYSVINIT_SCRIPT)
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/sysvinit/elosd.in
    ${CMAKE_CURRENT_BINARY_DIR}/elosd
    @ONLY
  )

  install(
    PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/elosd
    DESTINATION ${INSTALL_ELOSD_SYSVINIT_SCRIPT_DIR}
  )
endif()
