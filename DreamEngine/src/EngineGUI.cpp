/**
 * @file EngineGUI.cpp
 * @brief Implementación del sistema de interfaz gráfica del motor (EngineGUI).
 * @details Proporciona inicialización, actualización y renderizado de la UI basada en ImGui,
 *  además de paneles comunes: barra de menús, jerarquía, inspector, consola, resaltado de
 *  selección y gestor de archivos para guardar/cargar actores.
 */

#include "EngineGUI.h"
#include "Window.h"

#include "ECS/Actor.h"
#include "Window.h"
#include "ECS/Transform.h"

#include <filesystem>
#include "ActorSerializer.h"

namespace fs = std::filesystem;

/**
 * @brief Inicializa ImGui para la ventana dada y configura el estilo del editor.
 * @param window Puntero compartido a la ventana principal del motor.
 * @post Deja el estilo personalizado y reinicia la selección de actor.
 */
void
EngineGUI::init(const EngineUtilities::TSharedPointer<Window>& window) {
  ImGui::SFML::Init(*window->m_windowPtr);
  SetupMungoEngineGUIStyle();
  selectedActorIndex = -1;
}

/**
 * @brief Actualiza el estado de ImGui con el tiempo delta del frame.
 * @param window Puntero compartido a la ventana principal.
 * @param deltaTime Tiempo transcurrido desde el último frame.
 */
void
EngineGUI::update(const EngineUtilities::TSharedPointer<Window>& window,
  sf::Time deltaTime) {
  ImGui::SFML::Update(*window->m_windowPtr, deltaTime);
}

/**
 * @brief Renderiza todos los elementos acumulados de ImGui.
 * @param window Puntero compartido a la ventana principal.
 */
void
EngineGUI::render(const EngineUtilities::TSharedPointer<Window>& window) {
  ImGui::SFML::Render(*window->m_windowPtr);
}

/**
 * @brief Libera los recursos asociados a ImGui/SFML.
 */
void
EngineGUI::destroy() {
  ImGui::SFML::Shutdown();
}

/**
 * @brief Propaga eventos de SFML a ImGui.
 * @param window Referencia a la ventana SFML origen de los eventos.
 * @param event Evento de SFML.
 */
void
EngineGUI::processEvent(const sf::Window& window, const sf::Event& event) {
  ImGui::SFML::ProcessEvent(window, event);
}

/**
 * @brief Configura el tema visual del editor (colores, redondeos, paddings, etc.).
 * @details Define una paleta oscura con acentos cian/teal para elementos interactivos.
 */
void
EngineGUI::SetupMungoEngineGUIStyle() {
  ImGuiStyle& style = ImGui::GetStyle();

  style.WindowRounding = 4.0f;
  style.FrameRounding = 4.0f;
  style.GrabRounding = 4.0f;
  style.ScrollbarRounding = 4.0f;
  style.TabRounding = 3.0f;
  style.WindowBorderSize = 1.5f;
  style.FrameBorderSize = 1.0f;
  style.WindowPadding = ImVec2(10, 10);
  style.FramePadding = ImVec2(8, 4);
  style.ItemSpacing = ImVec2(8, 6);
  style.ItemInnerSpacing = ImVec2(6, 4);

  ImVec4* colors = style.Colors;

  ImVec4 accent = ImVec4(0.08f, 0.90f, 0.90f, 1.00f);
  ImVec4 accentDim = ImVec4(0.05f, 0.70f, 0.70f, 1.00f);
  ImVec4 bg0 = ImVec4(0.02f, 0.03f, 0.05f, 1.00f);
  ImVec4 bg1 = ImVec4(0.05f, 0.07f, 0.10f, 1.00f);
  ImVec4 bg2 = ImVec4(0.08f, 0.10f, 0.14f, 1.00f);
  ImVec4 bg3 = ImVec4(0.12f, 0.15f, 0.20f, 1.00f);

  colors[ImGuiCol_WindowBg] = bg0;
  colors[ImGuiCol_ChildBg] = bg0;
  colors[ImGuiCol_PopupBg] = bg1;
  colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.85f);

  colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
  colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.70f, 0.75f, 0.60f);

  colors[ImGuiCol_FrameBg] = bg1;
  colors[ImGuiCol_FrameBgHovered] = accentDim;
  colors[ImGuiCol_FrameBgActive] = accent;

  colors[ImGuiCol_TitleBg] = bg1;
  colors[ImGuiCol_TitleBgActive] = accentDim;
  colors[ImGuiCol_TitleBgCollapsed] = bg1;
  colors[ImGuiCol_MenuBarBg] = bg1;

  colors[ImGuiCol_ScrollbarBg] = bg1;
  colors[ImGuiCol_ScrollbarGrab] = accentDim;
  colors[ImGuiCol_ScrollbarGrabHovered] = accent;
  colors[ImGuiCol_ScrollbarGrabActive] = accent;

  colors[ImGuiCol_CheckMark] = accent;
  colors[ImGuiCol_SliderGrab] = accentDim;
  colors[ImGuiCol_SliderGrabActive] = accent;

  colors[ImGuiCol_Button] = bg2;
  colors[ImGuiCol_ButtonHovered] = accentDim;
  colors[ImGuiCol_ButtonActive] = accent;

  colors[ImGuiCol_Header] = bg2;
  colors[ImGuiCol_HeaderHovered] = accentDim;
  colors[ImGuiCol_HeaderActive] = accent;

  colors[ImGuiCol_Separator] = accentDim;
  colors[ImGuiCol_SeparatorHovered] = accent;
  colors[ImGuiCol_SeparatorActive] = accent;

  colors[ImGuiCol_ResizeGrip] = accentDim;
  colors[ImGuiCol_ResizeGripHovered] = accent;
  colors[ImGuiCol_ResizeGripActive] = accent;

  colors[ImGuiCol_Tab] = bg2;
  colors[ImGuiCol_TabHovered] = accentDim;
  colors[ImGuiCol_TabActive] = accent;
  colors[ImGuiCol_TabUnfocused] = bg1;
  colors[ImGuiCol_TabUnfocusedActive] = accentDim;

  colors[ImGuiCol_PlotLines] = accent;
  colors[ImGuiCol_PlotLinesHovered] = accent;
  colors[ImGuiCol_PlotHistogram] = accent;
  colors[ImGuiCol_PlotHistogramHovered] = accent;

  colors[ImGuiCol_TextSelectedBg] = ImVec4(accent.x, accent.y, accent.z, 0.35f);
  colors[ImGuiCol_DragDropTarget] = accent;
  colors[ImGuiCol_NavHighlight] = accent;
  colors[ImGuiCol_NavWindowingHighlight] = accent;
  colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.40f);

  colors[ImGuiCol_Border] = accentDim;
  colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.70f);

  colors[ImGuiCol_ChildBg] = bg0;
  colors[ImGuiCol_PopupBg] = ImVec4(0.06f, 0.08f, 0.12f, 0.98f);
}

/**
 * @brief Dibuja la barra de menús principal (File/Edit/View/Window/Help).
 * @details Los elementos son ilustrativos y no ejecutan acciones, salvo estructura de menús.
 */
void
EngineGUI::menuBar()
{
  if (ImGui::BeginMainMenuBar())
  {
    if (ImGui::BeginMenu("File"))
    {
      if (ImGui::MenuItem("New Scene", "Ctrl+N")) {}
      if (ImGui::MenuItem("Open Scene...", "Ctrl+O")) {}
      if (ImGui::MenuItem("Save Scene", "Ctrl+S")) {}
      if (ImGui::MenuItem("Save Scene As…")) {}
      ImGui::Separator();

      if (ImGui::BeginMenu("Recent Files"))
      {
        const char* recents[] = { "level01.scene", "tutorial.scene", "myTest.scene" };
        for (const char* f : recents)
          if (ImGui::MenuItem(f)) {}
        ImGui::EndMenu();
      }

      ImGui::Separator();
      if (ImGui::MenuItem("Exit", "Alt+F4")) {}
      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Edit"))
    {
      ImGui::MenuItem("Undo", "Ctrl+Z");
      ImGui::MenuItem("Redo", "Ctrl+Y");
      ImGui::Separator();
      ImGui::MenuItem("Cut", "Ctrl+X");
      ImGui::MenuItem("Copy", "Ctrl+C");
      ImGui::MenuItem("Paste", "Ctrl+V");
      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("View"))
    {
      ImGui::MenuItem("Hierarchy");
      ImGui::MenuItem("Inspector");
      ImGui::MenuItem("Console");
      ImGui::MenuItem("File Manager");
      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Window"))
    {
      ImGui::MenuItem("Maximize");
      ImGui::MenuItem("Minimize");
      ImGui::MenuItem("Reset Layout");
      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Help"))
    {
      ImGui::MenuItem("Documentation  (F1)");
      ImGui::MenuItem("Report a Bug");
      ImGui::MenuItem("About DreamEngine");
      ImGui::EndMenu();
    }

    ImGui::EndMainMenuBar();
  }
}

/**
 * @brief Dibuja el panel de jerarquía con la lista de actores.
 * @param actors Vector de actores presentes en la escena.
 * @details Permite seleccionar un actor (simple o doble clic) y resalta el seleccionado.
 * @post Actualiza @ref selectedActorIndex con el índice del actor seleccionado.
 */
void
EngineGUI::hierarchy(const std::vector<EngineUtilities::TSharedPointer<Actor>>& actors) {
  ImGui::Begin("Hierarchy");
  for (int i = 0; i < (int)actors.size(); ++i) {
    std::string name = actors[i]->getName();
    bool isSelected = (selectedActorIndex == i);
    if (ImGui::Selectable(name.c_str(), isSelected, ImGuiSelectableFlags_AllowDoubleClick)) {
      selectedActorIndex = i;
    }
    if (isSelected) {
      ImDrawList* draw_list = ImGui::GetWindowDrawList();
      ImVec2 p_min = ImGui::GetItemRectMin();
      ImVec2 p_max = ImGui::GetItemRectMax();
      draw_list->AddRect(p_min, p_max, IM_COL32(20, 220, 220, 255), 4.0f, 0, 3.5f);
    }
  }
  ImGui::End();
}

/**
 * @brief Dibuja el panel de inspector para el actor seleccionado.
 * @param actors Vector de actores presentes en la escena.
 * @details Muestra y permite editar nombre (no se escribe de vuelta al actor aquí),
 *  tag/layer (solo UI), y transforma (posición, rotación, escala) escribiendo en su Transform.
 * @note Escribe directamente en el componente @ref Transform del actor seleccionado si existe.
 */
void
EngineGUI::inspector(const std::vector<EngineUtilities::TSharedPointer<Actor>>& actors) {
  ImGui::Begin("Inspector");

  if (selectedActorIndex >= 0 && selectedActorIndex < (int)actors.size() && actors[selectedActorIndex]) {
    auto actor = actors[selectedActorIndex];

    bool isStatic = false;
    ImGui::Checkbox("##Static", &isStatic);
    ImGui::SameLine();

    char objectName[128];
    std::string name = actor->getName();
    std::copy(name.begin(), name.end(), objectName);
    objectName[name.size()] = '\0';

    ImGui::InputText("##ObjectName", objectName, IM_ARRAYSIZE(objectName));
    ImGui::SameLine();

    if (ImGui::Button("Icon")) {}

    ImGui::Separator();

    const char* tags[] = { "Untagged", "Player", "Enemy", "Environment" };
    static int currentTag = 0;
    ImGui::Combo("Tag", &currentTag, tags, IM_ARRAYSIZE(tags));
    ImGui::SameLine();

    const char* layers[] = { "Default", "TransparentFX", "Ignore Raycast", "Water", "UI" };
    static int currentLayer = 0;
    ImGui::Combo("Layer", &currentLayer, layers, IM_ARRAYSIZE(layers));

    ImGui::Separator();

    auto transform = actor->getComponent<Transform>();
    if (transform) {
      float pos[3] = { transform->getPosition().x, transform->getPosition().y, 0.0f };
      float rot[3] = { transform->getRotation().x, transform->getRotation().y, 0.0f };
      float sca[3] = { transform->getScale().x, transform->getScale().y, 1.0f };

      ImGui::PushItemWidth(60);

      ImGui::Text("Position");
      ImGui::SameLine(100);
      ImGui::PushID("PosX");
      ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.85f, 0.30f, 0.30f, 1.0f));
      ImGui::Button("X"); ImGui::PopStyleColor();
      ImGui::SameLine(); ImGui::DragFloat("##PosX", &pos[0], 0.5f);
      ImGui::PopID();

      ImGui::SameLine(); ImGui::PushID("PosY");
      ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.30f, 0.85f, 0.50f, 1.0f));
      ImGui::Button("Y"); ImGui::PopStyleColor();
      ImGui::SameLine(); ImGui::DragFloat("##PosY", &pos[1], 0.5f);
      ImGui::PopID();

      ImGui::SameLine(); ImGui::PushID("PosZ");
      ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.30f, 0.55f, 0.95f, 1.0f));
      ImGui::Button("Z"); ImGui::PopStyleColor();
      ImGui::SameLine(); ImGui::DragFloat("##PosZ", &pos[2], 0.5f);
      ImGui::PopID();

      ImGui::Text("Rotation");
      ImGui::SameLine(100);
      ImGui::PushID("RotX");
      ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.85f, 0.30f, 0.30f, 1.0f));
      ImGui::Button("X"); ImGui::PopStyleColor();
      ImGui::SameLine(); ImGui::DragFloat("##RotX", &rot[0], 1.0f);
      ImGui::PopID();

      ImGui::SameLine(); ImGui::PushID("RotY");
      ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.30f, 0.85f, 0.50f, 1.0f));
      ImGui::Button("Y"); ImGui::PopStyleColor();
      ImGui::SameLine(); ImGui::DragFloat("##RotY", &rot[1], 1.0f);
      ImGui::PopID();

      ImGui::SameLine(); ImGui::PushID("RotZ");
      ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.30f, 0.55f, 0.95f, 1.0f));
      ImGui::Button("Z"); ImGui::PopStyleColor();
      ImGui::SameLine(); ImGui::DragFloat("##RotZ", &rot[2], 1.0f);
      ImGui::PopID();

      ImGui::Text("Scale");
      ImGui::SameLine(100);
      ImGui::PushID("ScaX");
      ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.85f, 0.30f, 0.30f, 1.0f));
      ImGui::Button("X"); ImGui::PopStyleColor();
      ImGui::SameLine(); ImGui::DragFloat("##ScaX", &sca[0], 0.05f, 0.01f, 100.0f);
      ImGui::PopID();

      ImGui::SameLine(); ImGui::PushID("ScaY");
      ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.30f, 0.85f, 0.50f, 1.0f));
      ImGui::Button("Y"); ImGui::PopStyleColor();
      ImGui::SameLine(); ImGui::DragFloat("##ScaY", &sca[1], 0.05f, 0.01f, 100.0f);
      ImGui::PopID();

      ImGui::SameLine(); ImGui::PushID("ScaZ");
      ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.30f, 0.55f, 0.95f, 1.0f));
      ImGui::Button("Z"); ImGui::PopStyleColor();
      ImGui::SameLine(); ImGui::DragFloat("##ScaZ", &sca[2], 0.05f, 0.01f, 100.0f);
      ImGui::PopID();

      transform->setPosition(sf::Vector2f(pos[0], pos[1]));
      transform->setRotation(sf::Vector2f(rot[0], rot[1]));
      transform->setScale(sf::Vector2f(sca[0], sca[1]));

      ImGui::PopItemWidth();
    }
  }

  ImGui::End();
}

/**
 * @brief Muestra una consola simple de logs (placeholder).
 * @details Por ahora solo presenta un texto informativo.
 */
void
EngineGUI::console() {
  ImGui::Begin("Console");
  ImGui::Text("Logs and messages will appear here.");
  ImGui::End();
}

/**
 * @brief Dibuja un contorno resaltado alrededor del actor seleccionado.
 * @param renderWindow Puntero a la ventana SFML para dibujar.
 * @param actors Vector con los actores de la escena.
 * @details Obtiene el componente @ref CShape del actor seleccionado y dibuja su borde
 *  temporalmente, preservando y restaurando el estado (color/espesor/relleno) de la forma.
 * @note Reduce el grosor del contorno para el actor de pista (Track Actor).
 */
void
EngineGUI::drawSelectedOutline(
  sf::RenderWindow* renderWindow,
  const std::vector<EngineUtilities::TSharedPointer<Actor>>& actors)
{
  int idx = selectedActorIndex;
  if (idx >= 0 && idx < (int)actors.size() && actors[idx]) {
    auto shapeComp = actors[idx]->getComponent<CShape>();
    if (shapeComp) {
      auto shapePtr = shapeComp->getShapePtr();
      if (shapePtr) {
        float thickness = 2.0f;
        if (actors[idx]->getName() == "Track Actor")
          thickness = 0.5f;
        sf::Color oldOutlineColor = shapePtr->getOutlineColor();
        float oldThickness = shapePtr->getOutlineThickness();
        sf::Color oldFill = shapePtr->getFillColor();

        shapePtr->setOutlineThickness(thickness);
        shapePtr->setOutlineColor(sf::Color(20, 220, 220));
        shapePtr->setFillColor(sf::Color(0, 0, 0, 0));
        renderWindow->draw(*shapePtr);

        shapePtr->setOutlineColor(oldOutlineColor);
        shapePtr->setOutlineThickness(oldThickness);
        shapePtr->setFillColor(oldFill);
      }
    }
  }
}

/**
 * @brief Panel de gestor de archivos para guardar/cargar actores en un .txt.
 * @param actors Vector (modificable) de actores de la escena.
 * @details
 *  - Campo de texto para el nombre del archivo.
 *  - Botón para guardar con @ref ActorSerializer::saveActorsToFile.
 *  - Botón para cargar con @ref ActorSerializer::loadActorsFromFile (limpia y repuebla).
 *  - Lista de archivos .txt del directorio actual para selección rápida.
 * @note No valida el contenido del archivo cargado más allá del formato esperado por el serializer.
 */
void
EngineGUI::fileManagerPanel(std::vector<EngineUtilities::TSharedPointer<Actor>>& actors) {
  ImGui::Begin("File Manager");
  static char fileName[128] = "actors.txt";
  ImGui::InputText("Actors File", fileName, IM_ARRAYSIZE(fileName));
  if (ImGui::Button("Save Actors")) {
    ActorSerializer::saveActorsToFile(fileName, actors);
  }
  ImGui::SameLine();
  if (ImGui::Button("Load Actors")) {
    actors.clear();
    ActorSerializer::loadActorsFromFile(fileName, actors);
  }
  ImGui::Separator();
  ImGui::Text("Available files:");
  for (const auto& entry : fs::directory_iterator(".")) {
    std::string fname = entry.path().filename().string();
    if (fname.size() >= 4 && fname.substr(fname.size() - 4) == ".txt") {
      if (ImGui::Selectable(fname.c_str())) {
        strncpy(fileName, fname.c_str(), sizeof(fileName));
      }
    }
  }
  ImGui::End();
}
