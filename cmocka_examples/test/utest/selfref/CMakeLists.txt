
create_unit_test(
  NAME
		weak_selfref_example
  SOURCES
    example.c
  LIBRARIES
    mock_selfref_static # important must come befor selfref_static_weak to work
    selfref_static_weak
)
