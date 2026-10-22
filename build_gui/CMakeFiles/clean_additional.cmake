# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Release")
  file(REMOVE_RECURSE
  "CMakeFiles\\StudentAcademicDashboardGUI_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\StudentAcademicDashboardGUI_autogen.dir\\ParseCache.txt"
  "StudentAcademicDashboardGUI_autogen"
  )
endif()
