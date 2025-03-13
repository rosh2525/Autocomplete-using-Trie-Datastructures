# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "Autocomplete_using_Trie_Datastructures_autogen"
  "CMakeFiles/Autocomplete_using_Trie_Datastructures_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/Autocomplete_using_Trie_Datastructures_autogen.dir/ParseCache.txt"
  )
endif()
