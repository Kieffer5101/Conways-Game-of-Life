# CMakeList.txt : CMake project for Conways Game of Life, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.30)

project ("CGoL")

add_executable(SFML_CGoL "main.cpp"  "configuration.h" "events.cpp" "events.h")

set(Debug_Libs 
	${CMAKE_SOURCE_DIR}/library/sfml-graphics-d.lib
	${CMAKE_SOURCE_DIR}/library/sfml-system-d.lib
	${CMAKE_SOURCE_DIR}/library/sfml-window-d.lib
	${CMAKE_SOURCE_DIR}/library/sfml-graphics-d.lib)

#set(Release_Libs
#	${CMAKE_SOURCE_DIR}/library/sfml-graphics.lib
#	${CMAKE_SOURCE_DIR}/library/sfml-system.lib
#	${CMAKE_SOURCE_DIR}/library/sfml-window.lib
#	${CMAKE_SOURCE_DIR}/library/sfml-graphics.lib)

target_link_libraries(SFML_CGoL ${Debug_Libs})
#target_link_libraries(SFML_CGoL optimized ${Release_Libs})

include_directories(${CMAKE_SOURCE_DIR}/include)

set_property(TARGET SFML_CGoL PROPERTY CXX_STANDARD 17)
set_property(TARGET SFML_CGoL PROPERTY CXX_STANDARD_REQUIRED ON)






## Enable Hot Reload for MSVC compilers if supported.
#if (POLICY CMP0141)
#  cmake_policy(SET CMP0141 NEW)
#  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
#endif()


## Add source to this project's executable.
#if(SFML_FOUND)
#    include_directories(${SFML_INCLUDE_DIR})
#    
#    target_link_libraries(${PROJECT_NAME} ${SFML_LIBRARIES})
#else()
#    message(FATAL_ERROR "SFML not found")
#endif()
#
#
#
#if (CMAKE_VERSION VERSION_GREATER 3.12)
#  set_property(TARGET CMakeTarget PROPERTY CXX_STANDARD 20)
#endif()
#
## TODO: Add tests and install targets if needed.
