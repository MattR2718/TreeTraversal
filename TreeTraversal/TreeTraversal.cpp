// TreeTraversal.cpp : Defines the entry point for the application.
//

#include "TreeTraversal.h"

bool insideNode(std::vector<std::tuple<int, int, int>>& nodes, sf::Vector2i& pos, float& r) {
    for (auto& node : nodes) {
        float xDist = std::get<1>(node) - pos.x;
        float yDist = std::get<2>(node) - pos.y;
        float magDist = sqrt((xDist * xDist) + (yDist * yDist));
        if (magDist <= r) {
            return true;
        }
    }
    return false;
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 800), "Tree Traversal");
    ImGui::SFML::Init(window);

    std::vector<std::tuple<int, int, int>> nodes;

    bool showNodes = true;
    float nodeRadius = 50.0f;
    int circleSegments = 100;
    float nodeColour[3] = { (float)204 / 255, (float)77 / 255, (float)5 / 255 };

    sf::Clock deltaClock;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
                sf::Vector2i pos = sf::Mouse::getPosition(window);
                if (!insideNode(nodes, pos, nodeRadius)) {
                    std::tuple<int, int, int> temp = { nodes.size(), pos.x, pos.y};
                    nodes.push_back(temp);
                }
            }
        }
        ImGui::SFML::Update(window, deltaClock.restart());

        ImGui::Begin("Nodes");
        ImGui::Text("%i", nodes.size());
        ImGui::Checkbox("Show Nodes", &showNodes);
        ImGui::SliderFloat("Node Size", &nodeRadius, 0.0f, 100.0f);
        ImGui::SliderInt("Sides", &circleSegments, 3, 150);
        ImGui::ColorEdit3("Node Colour", nodeColour);
        if (ImGui::Button("Clear All")) {
            nodes = std::vector<std::tuple<int, int, int>>{};
        }
        ImGui::End();

        window.clear(sf::Color(18, 33, 43));

        sf::CircleShape shape;

        for (auto& node : nodes) {
            shape.setRadius(nodeRadius);
            shape.setOrigin(nodeRadius, nodeRadius);
            shape.setPosition(std::get<1>(node), std::get<2>(node));
            shape.setPointCount(circleSegments);
            shape.setFillColor(sf::Color
            (
                (int)(nodeColour[0] * 255),
                (int)(nodeColour[1] * 255),
                (int)(nodeColour[2] * 255)
            ));
            if (showNodes) {
                window.draw(shape);
            }
        }
        
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}
