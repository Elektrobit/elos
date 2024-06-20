# SPDX-License-Identifier: MIT

if (ELOS_TOOLS)
  add_executable(
    elosc
    elosc.c
  )

  target_link_libraries(
    elosc
    PRIVATE
      libelos
      version
  )

  if (INSTALL_ELOS_TOOLS)
    install(TARGETS elosc DESTINATION ${CMAKE_INSTALL_BINDIR})
  endif (INSTALL_ELOS_TOOLS)
endif (ELOS_TOOLS)

if (ELOS_DEMOS)
  add_subdirectory(elos_log4c_demo)

  find_package(PkgConfig REQUIRED)
  pkg_check_modules(LIBESMTP REQUIRED libesmtp-1.0 )

  add_executable(
    demo_libelos_v2
    demo_libelos_v2.c
  )

  target_link_libraries(
    demo_libelos_v2
    PRIVATE
      libelos
      version
  )

  if (INSTALL_ELOS_DEMOS)
    install(TARGETS demo_libelos_v2 DESTINATION ${CMAKE_INSTALL_BINDIR})
  endif()

  add_executable(
    demo_scanner_shmem
    demo_scanner_shmem.c
  )

  target_link_libraries(
    demo_scanner_shmem
    PRIVATE
      libelos
      version
  )

  if (INSTALL_ELOS_DEMOS)
    install(TARGETS demo_scanner_shmem DESTINATION ${CMAKE_INSTALL_BINDIR})
  endif()

  add_executable(
    syslog_example
    syslog.c
  )

  target_link_libraries(
    syslog_example
    PRIVATE
      libelos
      version
  )

  if (INSTALL_ELOS_DEMOS)
    install(TARGETS syslog_example DESTINATION ${CMAKE_INSTALL_BINDIR})
  endif()

  add_executable(
    tinyElosc
    tinyElosc.c
  )

  target_link_libraries(
    tinyElosc
    PRIVATE
      libelos_common
      libelos
      version
  )

  if (INSTALL_ELOS_DEMOS)
    install(TARGETS tinyElosc DESTINATION ${CMAKE_INSTALL_BINDIR})
  endif()

  add_executable(
    elosMon
    elosMon.c
  )

  target_link_libraries(
    elosMon
    PRIVATE
    #elos_common_interface
      libelos_common
      esmtp
      libelos
      version
  )

  if (INSTALL_ELOS_DEMOS)
    install(TARGETS elosMon DESTINATION ${CMAKE_INSTALL_BINDIR})
  endif()

  add_executable(
    demo_eloslog
    demo_eloslog.c
  )

  target_link_libraries(
    demo_eloslog
    PRIVATE
      libelos_common
      eventprocessor_static
      eloslog_static
      libelos
      version
  )

  if (INSTALL_ELOS_DEMOS)
    install(TARGETS demo_eloslog DESTINATION ${CMAKE_INSTALL_BINDIR})
  endif()

  add_executable(
    demo_eventbuffer
    demo_eventbuffer.c
  )

  target_link_libraries(
    demo_eventbuffer
    PRIVATE
      eventbuffer_static
  )

  if(CMAKE_BUILD_TYPE STREQUAL "Tsan")
    target_link_libraries(demo_eventbuffer PRIVATE tsan)
    target_compile_options(demo_eventbuffer PUBLIC "-fsanitize=thread")
  endif()

  if (INSTALL_ELOS_DEMOS)
    install(TARGETS demo_eventbuffer DESTINATION ${CMAKE_INSTALL_BINDIR})
  endif()

  if (ELOSD_LIBDLT)
    add_executable(
      elosDlt
      elosDlt.c
    )

    target_link_libraries(
      elosDlt
      PRIVATE
        libelosdlt
    )

    if (INSTALL_ELOS_DEMOS)
      install(TARGETS elosDlt DESTINATION ${CMAKE_INSTALL_BINDIR})
    endif()
  endif()

endif()
