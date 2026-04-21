# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/appSICStudio_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/appSICStudio_autogen.dir/ParseCache.txt"
  "appSICStudio_autogen"
  )
endif()
