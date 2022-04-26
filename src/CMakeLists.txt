file (GLOB_RECURSE sources *.h *.cpp *.hpp *.tpp)

include_directories(include)
#set(sources geoWordle.cpp problem.cpp problem_ILP.cpp miscellaneous.cpp problem_CELL.cpp )
add_executable(tourGenerator ${sources})
include_directories(solvers)
include_directories(solvers/ant-colony)
include_directories(tools)

if (MSVC)
	set(CMAKE_CXX_FLAGS_RELEASE "/O2 /DNDEBUG")
else ()
	#set(CMAKE_CXX_FLAGS "-Wextra")
	set(CMAKE_CXX_FLAGS_DEBUG "-g")
	set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
	target_compile_features(tourGenerator PUBLIC cxx_std_17)
	target_link_libraries(tourGenerator PUBLIC stdc++fs)
	
endif()