file (GLOB_RECURSE sources *.h *.cpp *.hpp *.tpp)

option(CXX "enable C++ compilation" ON)

if(CXX)
  enable_language(CXX)
endif()

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR})
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release")
endif()
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

find_package(GUROBI REQUIRED)

include_directories(${GUROBI_INCLUDE_DIRS})


include_directories(include)
add_executable(tourGenerator ${sources})
include_directories(solvers)
include_directories(solvers/exact)
include_directories(solvers/greedy)
include_directories(solvers/ant-colony)
include_directories(tools)


if(CXX)
  set(CMAKE_CXX_STANDARD 20)
  target_link_libraries(${CMAKE_PROJECT_NAME} optimized ${GUROBI_CXX_LIBRARY}
                        debug ${GUROBI_CXX_DEBUG_LIBRARY})
endif()

target_link_libraries(${CMAKE_PROJECT_NAME} ${GUROBI_LIBRARY})

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
  include(FeatureSummary)
  feature_summary(WHAT ALL)
endif()
