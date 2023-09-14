# SPDX-License-Identifier: MIT

create_mock_lib(
  NAME
    mock_log_aggregator
  SOURCES
    mock_LogAggregator.c
  INCLUDES
    ${CMAKE_CURRENT_SOURCE_DIR}
  LIBRARIES
    scanner_manager_static_weak
    cmocka_extensions::cmocka_extensions
  WRAP_FLAG
    MOCK_ELOS_ELOSD_EVENTLOGGING_FUNC_LINKER_WRAPPING
  WRAPS
    elosLogAggregatorStart
    elosLogAggregatorAdd
    elosLogAggregatorShutdown
    elosLogAggregatorFindEvents
)
