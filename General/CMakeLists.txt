add_library(General INTERFACE)

target_include_directories(General INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# ��������� ��������� ������, ����� ���������� ������������ � Visual Studio
add_custom_target(GeneralTarget SOURCES utility.h)
set_property(TARGET GeneralTarget PROPERTY FOLDER "General")

