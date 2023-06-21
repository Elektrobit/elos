
create_unit_test(
  NAME
		wrap_extref_example
  SOURCES
    example.c
  LIBRARIES
		extref_library_static
    mock_extref_dependency
)
