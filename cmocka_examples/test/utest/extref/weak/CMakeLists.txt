
create_unit_test(
  NAME
		weak_extref_example
  SOURCES
    example.c
  LIBRARIES
    extref_library
    mock_extref_dependency_static # important must come befor selfref_static_weak to work
    extref_dependency_static_weak
)
