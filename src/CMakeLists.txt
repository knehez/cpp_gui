find_package(fmt CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(docopt CONFIG REQUIRED)
find_package(SDL2 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

# Generic test
add_executable(intro main.cpp)
target_link_libraries(
  intro
  PRIVATE project_options
          project_warnings
          fmt::fmt
          spdlog::spdlog 
          SDL2::SDL2 
          SDL2::SDL2main 
          imgui::imgui)

if(NOT MSVC)
  target_link_libraries(
    intro
    PRIVATE docopt_s)
else()
  target_link_libraries(
    intro
    PRIVATE docopt)
endif()