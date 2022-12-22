find_package(fmt CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(docopt CONFIG REQUIRED)

file(GLOB_RECURSE SOURCES *.h *.cpp)

add_executable(intro ${SOURCES})

target_link_libraries(
  intro
  PRIVATE project_options
          project_warnings
          fmt::fmt
          spdlog::spdlog 
          SDL2::SDL2 
          SDL2::SDL2main 
          imgui::imgui
          )

if(NOT MSVC)
  target_link_libraries(
    intro
    PRIVATE docopt_s)
else()
  target_link_libraries(
    intro
    PRIVATE docopt)
endif()