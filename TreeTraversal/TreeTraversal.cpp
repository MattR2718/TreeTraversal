// TreeTraversal.cpp : Defines the entry point for the application.
//

#include "TreeTraversal.h"

int insideNode(std::vector<std::tuple<int, int, int>>& nodes, sf::Vector2i& pos, float& r, bool draw) {
    for (auto& node : nodes) {
        float xDist = std::get<1>(node) - pos.x;
        float yDist = std::get<2>(node) - pos.y;
        float magDist = sqrt((xDist * xDist) + (yDist * yDist));
        if ((draw) && (magDist <= r * 2)) {
            return std::get<0>(node);
        }
        else if (magDist <= r) {
            return std::get<0>(node);
        }
    }
    return 0;
}

sf::Color makeColour(float colour[3]) {
    return (sf::Color((int)(colour[0] * 255), (int)(colour[1] * 255), (int)(colour[2] * 255)));
}

void cleanList(std::map<int, std::vector<int>>& adjacencyList, int& nodeIn, std::vector<std::tuple<int, int, int>> nodes) {
    adjacencyList.erase(nodeIn);
    for (auto& node : nodes) {
        if (adjacencyList[std::get<0>(node)][0] == nodeIn) {
            adjacencyList[std::get<0>(node)].erase(adjacencyList[std::get<0>(node)].begin());
        }
        else if ((adjacencyList[std::get<0>(node)].size() == 2) && (adjacencyList[std::get<0>(node)][1] == nodeIn)) {
            adjacencyList[std::get<0>(node)].erase(adjacencyList[std::get<0>(node)].begin() + 1);
        }
    }
}

void connectNodes(int connect[2], std::map<int, std::vector<int>>& adjacencyList) {
    // Make checks for more than two connections and being connected to by more than one
    adjacencyList[connect[0]].push_back(connect[1]);
    adjacencyList[connect[1]].push_back(connect[0]);
}

std::pair<int, int> getCoords(std::vector<std::tuple<int, int, int>>& nodes, int& nodeNum) {
    for (auto& node : nodes) {
        if (std::get<0>(node) == nodeNum) {
            return std::make_pair(std::get<1>(node), std::get<2>(node));
        }
    }
    return std::make_pair(-1, -1);
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 800), "Tree Traversal");
    ImGui::SFML::Init(window);

    std::vector<std::tuple<int, int, int>> nodes;
    int nodeIndex = 1;
    std::map<int, std::vector<int>> adjacencyList;
    std::vector<int> visited;

    bool showNodes = true;
    float nodeRadius = 50.0f;
    int circleSegments = 100;
    float nodeColour[3] = { (float)204 / 255, (float)77 / 255, (float)5 / 255 };
    float bgColour[3] = { (float)18 / 255, (float)33 / 255, (float)43 / 255 };
    float edgeColour[3] = { (float)255 / 255, (float)255 / 255, (float)255 / 255 };
    float selectColour[3] = { (float)50 / 255, (float)255 / 255, (float)50 / 255 };
    float visitColour[3] = { (float)255 / 255, (float)50 / 255, (float)50 / 255 };
    

    int coolDown = 0;
    int upto = 0;
    int at = 0;
    bool running = false;
    int connect[2] = { -1, -1 };
    std::pair<int, int> nullCoords = { -1, -1 };

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
            else if ((!running) && (coolDown > 100) && (sf::Mouse::isButtonPressed(sf::Mouse::Right))) {
                sf::Vector2i pos = sf::Mouse::getPosition(window);
                if (!insideNode(nodes, pos, nodeRadius, true)) {
                    std::tuple<int, int, int> temp = { nodeIndex, pos.x, pos.y};
                    nodeIndex++;
                    nodes.push_back(temp);
                }
                else {
                    int nodeIn = insideNode(nodes, pos, nodeRadius, false);
                    if (nodeIn > 0) {
                        for (int i = 0; i < nodes.size(); i++) {
                            if (nodeIn == std::get<0>(nodes[i])) {
                                nodes.erase(nodes.begin() + i);
                                cleanList(adjacencyList, nodeIn, nodes);
                                nodeIndex = 1;
                                upto = 0;
                                at = 0;
                            }
                        }
                    }
                }
                coolDown = 0;
            }
            else if ((!running) && (sf::Mouse::isButtonPressed(sf::Mouse::Left))) {
                sf::Vector2i pos = sf::Mouse::getPosition(window);
                int nodeIn = insideNode(nodes, pos, nodeRadius, false);
                if (connect[0] == -1) {
                    connect[0] = nodeIn;
                }
                else if (connect[1] == -1) {
                    connect[1] = nodeIn;
                    connectNodes(connect, adjacencyList);
                    connect[0] = -1;
                    connect[1] = -1;
                }
            }
        }

        coolDown++;

        ImGui::SFML::Update(window, deltaClock.restart());

        ImGui::Begin("Customise");
        ImGui::Text("%i", nodes.size());
        ImGui::Checkbox("Show Nodes", &showNodes);
        ImGui::SliderFloat("Node Size", &nodeRadius, 0.0f, 100.0f);
        ImGui::SliderInt("Sides", &circleSegments, 3, 150);
        ImGui::ColorEdit3("Node Colour", nodeColour);
        ImGui::ColorEdit3("Edge Colour", edgeColour);
        ImGui::ColorEdit3("Selected Colour", selectColour);
        ImGui::ColorEdit3("Background Colour", bgColour);
        ImGui::ColorEdit3("Visiting Colour", visitColour);
        if (ImGui::Button("Clear All Nodes")) {
            nodes = std::vector<std::tuple<int, int, int>>{};
            adjacencyList = std::map<int, std::vector<int>>{};
            running = false;
            nodeIndex = 1;
        }
        ImGui::End();

        ImGui::Begin("Algorithms");
        const char* items[] = { "Depth First", "Breadth First" };
        static int item_current = 0;
        ImGui::ListBox("listbox", &item_current, items, IM_ARRAYSIZE(items), (int)(sizeof(items) / sizeof(*items)));
        if (ImGui::Button("Traverse")) {
            running = true;
            switch (item_current) {
            case 0:
                visited = depthFirst(adjacencyList, 1);
                for (auto& f : visited) {
                    std::cout << f << ' ';
                }
                std::cout << '\n';
                break;
            case 1:
                break;
            }
        }
        ImGui::End();

        window.clear(makeColour(bgColour));

        sf::CircleShape shape;

        //Draw Visiting
        if (running) {
            //std::cout << "RUNNING\n";
            at++;
            if (at == 100) {
                at = 0;
                upto++;
            }
            std::cout << visited.size() <<' '<<upto << '\n';
            if (upto == visited.size()) { upto = 0; }
            float r = (float)nodeRadius * 1.2;
            shape.setFillColor(makeColour(visitColour));
            shape.setOrigin(r, r);
            shape.setRadius(r);
            for (int i = 0; i < upto + 1; i++) {
                for (auto& node : nodes) {
                    if (std::get<0>(node) == visited[i]) {
                        shape.setPosition(std::get<1>(node), std::get<2>(node));
                        window.draw(shape);
                    }
                }
            }
            
        }

        //Draw Selected
        if (!running) {
            if (connect[0] != -1) {
                float r = (float)nodeRadius * 1.2;
                shape.setFillColor(makeColour(selectColour));
                shape.setOrigin(r, r);
                shape.setRadius(r);
                for (auto& node : nodes) {
                    if (std::get<0>(node) == connect[0]) {
                        shape.setPosition(std::get<1>(node), std::get<2>(node));
                    }
                }
                window.draw(shape);
            }
            if (connect[1] != -1) {
                for (auto& node : nodes) {
                    if (std::get<0>(node) == connect[1]) {
                        shape.setPosition(std::get<1>(node), std::get<2>(node));
                    }
                }
                window.draw(shape);
            }
        }

        //Draw connections
        for (auto& node : nodes) {
            int nodeNum = std::get<0>(node);
            std::pair<int, int> nodeCoords = getCoords(nodes, nodeNum);
            for (auto& n : adjacencyList[nodeNum]) {
                std::pair<int, int> node2Coords = getCoords(nodes, n);
                if (node2Coords != nullCoords) {
                    sf::Vertex line[] =
                    {
                        sf::Vertex(sf::Vector2f(nodeCoords.first, nodeCoords.second)),
                        sf::Vertex(sf::Vector2f(node2Coords.first, node2Coords.second))
                    };
                    line[0].color = makeColour(edgeColour);
                    line[1].color = makeColour(edgeColour);

                    window.draw(line, 2, sf::Lines);
                }
            }
        }

        //Draw Nodes
        for (auto& node : nodes) {
            shape.setRadius(nodeRadius);
            shape.setOrigin(nodeRadius, nodeRadius);
            shape.setPosition(std::get<1>(node), std::get<2>(node));
            shape.setPointCount(circleSegments);
            shape.setFillColor(makeColour(nodeColour));
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
