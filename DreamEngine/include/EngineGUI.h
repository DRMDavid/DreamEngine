#pragma once
#include "Prerequisites.h"
#include <vector>
class Window;
class Actor;


class
    EngineGUI {
public:
    EngineGUI() = default;
    ~EngineGUI() = default;

    void
        init(const EngineUtilities::TSharedPointer<Window>& window);

    void
        update(const EngineUtilities::TSharedPointer<Window>& window,
            sf::Time deltaTime);

    void
        render(const EngineUtilities::TSharedPointer<Window>& window);

    void
        destroy();

    void
        processEvent(const sf::Window& window, const sf::Event& event);

    void
        SetupMungoEngineGUIStyle();

    void
        menuBar();

    void
        hierarchy(const std::vector<EngineUtilities::TSharedPointer<Actor>>& actors);

    void
        inspector(const std::vector<EngineUtilities::TSharedPointer<Actor>>& actors);

    void
        console();

    void
        drawSelectedOutline(sf::RenderWindow* renderWindow,
            const std::vector<EngineUtilities::TSharedPointer<Actor>>& actors);

    void
        fileManagerPanel(std::vector<EngineUtilities::TSharedPointer<Actor>>& actors);


    int selectedActorIndex = -1;
};