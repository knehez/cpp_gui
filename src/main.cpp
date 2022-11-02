#include <SDL2/SDL.h>
#include <functional>
#include <iostream>
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_sdlrenderer.h"

#include <spdlog/spdlog.h>
#include <docopt/docopt.h>

static constexpr auto USAGE =
  R"(Naval Fate.

    Usage:
          naval_fate
          naval_fate ship new <name>...
          naval_fate ship <name> move <x> <y> [--speed=<kn>]
          naval_fate ship shoot <x> <y>
          naval_fate mine (set|remove) <x> <y> [--moored | --drifting]
          naval_fate (-h | --help)
          naval_fate --version
 Options:
          -h --help     Show this screen.
          --version     Show version.
          --speed=<kn>  Speed in knots [default: 10].
          --moored      Moored (anchored) mine.
          --drifting    Drifting mine.
)";


static auto windowWidth = 640;
static auto windowHeight = 480;

bool handleQuitEvent(const SDL_Event& event)
{
    if ((SDL_QUIT == event.type) ||
        (SDL_KEYDOWN == event.type && SDL_SCANCODE_ESCAPE == event.key.keysym.scancode))
    {
        return false;
    }
    return true;
}

int main(int argc, char *argv[])
{

  std::map<std::string, docopt::value> args = docopt::docopt(USAGE,
    { std::next(argv), std::next(argv, argc) },
    true,// show help if requested
    "Naval Fate 2.0");// version string

  for (auto const &arg : args) {
    std::cout << arg.first << "=" << arg.second << std::endl;
  }


  // Use the default logger (stdout, multi-threaded, colored)
  spdlog::info("Hello, {}!", "World");

  fmt::print("Hello, from {}\n", "{fmt}");

  SDL_version version;
  SDL_GetVersion(&version);
  spdlog::info("SDL version {}.{}.{}", version.major, version.minor, version.patch);

  SDL_Window *m_window;
  SDL_Renderer *m_renderer;

  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    printf("[ERROR] %s\n", SDL_GetError());
    return -1;
  }
  atexit(SDL_Quit);
  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");

  m_window = SDL_CreateWindow("Milling Simulator",
    SDL_WINDOWPOS_UNDEFINED,
    SDL_WINDOWPOS_UNDEFINED,
    windowHeight,
    windowWidth,
    SDL_WINDOW_RESIZABLE);

  m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  ImGui::CreateContext();
  ImGui_ImplSDL2_InitForSDLRenderer(m_window, m_renderer);
  ImGui_ImplSDLRenderer_Init(m_renderer);

  ImGui::StyleColorsDark();
  ImGuiIO const &imguiIO = ImGui::GetIO();

  bool running = true;
  while (running) {
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(m_renderer);

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      ImGui_ImplSDL2_ProcessEvent(&event);
      running = handleQuitEvent(event);
      // within imgui windows
      if (!imguiIO.WantCaptureMouse && !imguiIO.WantCaptureKeyboard) {
        running = handleQuitEvent(event);
      }
    }

    // start the Dear ImGui frame
    ImGui_ImplSDLRenderer_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    ImGui::Text("hello");


    ImGui::Render();

    ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
    SDL_RenderPresent(m_renderer);
  }

  ImGui_ImplSDLRenderer_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();

  SDL_DestroyRenderer(m_renderer);

  SDL_DestroyWindow(m_window);
  SDL_Quit();

  return EXIT_SUCCESS;
}
