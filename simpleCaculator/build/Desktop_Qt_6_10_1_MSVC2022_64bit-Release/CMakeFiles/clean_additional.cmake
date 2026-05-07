# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Release")
  file(REMOVE_RECURSE
  "CMakeFiles\\simpleCaculator_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\simpleCaculator_autogen.dir\\ParseCache.txt"
  "simpleCaculator_autogen"
  )
endif()
