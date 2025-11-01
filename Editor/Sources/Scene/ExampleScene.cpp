#include "ExampleScene.hpp"

MCEditor::ExampleScene::ExampleScene() : MCEngine::Scene("ExampleScene")
{
    SetMainCamera(AddCamera("MainCamera",
                            MCEngine::TransformComponent(glm::vec3(0.0f, 5.0f, 8.0f), glm::vec3(-30.0f, 0.0f, 0.0f)),
                            MCEngine::CameraComponent(MCEngine::CameraType::Perspective)));

    AddLight("DirectionalLight",
             MCEngine::TransformComponent(glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(-45.0f, -45.0f, 0.0f)),
             MCEngine::LightComponent(MCEngine::LightType::Directional));

    AddSkybox("Skybox", MCEngine::SkyboxComponent());

    WelcomeToMCEngine();
}

void MCEditor::ExampleScene::WelcomeToMCEngine()
{
    ENGINE_PROFILE_FUNCTION();

    struct Position
    {
        int x, y, z;

        bool operator<(const Position &other) const
        {
            if (x != other.x)
                return x < other.x;
            if (y != other.y)
                return y < other.y;
            return z < other.z;
        }
    };

    std::set<Position> positions = {
        {0, 0, 2}, {0, 0, 3}, {0, 0, 4}, {0, 0, 5}, {0, 0, 6},                       // M
        {0, 1, 2}, {0, 1, 3}, {0, 1, 4}, {0, 1, 5}, {0, 1, 6},                       // M
        {0, 2, 2}, {0, 2, 3}, {0, 2, 4}, {0, 2, 5}, {0, 2, 6},                       // M
        {0, 3, 2}, {0, 3, 3}, {0, 3, 5}, {0, 3, 6},                                  // M
        {0, 4, 2}, {0, 4, 6},                                                        // M
        {0, 5, 4},                                                                   // M
        {0, 6, 3}, {0, 6, 4}, {0, 6, 5},                                             // M
        {0, 7, 2}, {0, 7, 3}, {0, 7, 4}, {0, 7, 5}, {0, 7, 6},                       // M
        {0, 8, 1}, {0, 8, 2}, {0, 8, 3}, {0, 8, 4}, {0, 8, 5}, {0, 8, 6}, {0, 8, 7}, // M

        {1, 0, 2}, {1, 0, 3}, {1, 0, 4}, {1, 0, 5}, {1, 0, 6},                       // M
        {1, 1, 2}, {1, 1, 3}, {1, 1, 4}, {1, 1, 5}, {1, 1, 6},                       // M
        {1, 2, 2}, {1, 2, 3}, {1, 2, 4}, {1, 2, 5}, {1, 2, 6},                       // M
        {1, 3, 2}, {1, 3, 3}, {1, 3, 5}, {1, 3, 6},                                  // M
        {1, 4, 2}, {1, 4, 6},                                                        // M
        {1, 5, 4},                                                                   // M
        {1, 6, 3}, {1, 6, 4}, {1, 6, 5},                                             // M
        {1, 7, 2}, {1, 7, 3}, {1, 7, 4}, {1, 7, 5}, {1, 7, 6},                       // M
        {1, 8, 1}, {1, 8, 2}, {1, 8, 3}, {1, 8, 4}, {1, 8, 5}, {1, 8, 6}, {1, 8, 7}, // M

        {2, 8, 0}, {3, 8, 0}, {8, 8, 0},                       // C
        {2, 8, 1},                                             // C
        {4, 8, 2}, {5, 8, 2}, {6, 8, 2}, {7, 8, 2},            // C
        {4, 8, 3}, {5, 8, 3}, {6, 8, 3}, {7, 8, 3}, {8, 8, 3}, // C
        {4, 8, 4}, {5, 8, 4}, {6, 8, 4}, {7, 8, 4}, {8, 8, 4}, // C
        {4, 8, 5}, {5, 8, 5}, {6, 8, 5}, {7, 8, 5}, {8, 8, 5}, // C
        {4, 8, 6}, {5, 8, 6}, {6, 8, 6}, {7, 8, 6},            // C
        {2, 8, 7},                                             // C
        {8, 8, 8},                                             // C {2, 8, 8}, {3, 8, 8},

        {2, 7, 0}, {3, 7, 0}, {8, 7, 0},                       // C
        {2, 7, 1},                                             // C
        {4, 7, 2}, {5, 7, 2}, {6, 7, 2}, {7, 7, 2},            // C
        {4, 7, 3}, {5, 7, 3}, {6, 7, 3}, {7, 7, 3}, {8, 7, 3}, // C
        {4, 7, 4}, {5, 7, 4}, {6, 7, 4}, {7, 7, 4}, {8, 7, 4}, // C
        {4, 7, 5}, {5, 7, 5}, {6, 7, 5}, {7, 7, 5}, {8, 7, 5}, // C
        {4, 7, 6}, {5, 7, 6}, {6, 7, 6}, {7, 7, 6},            // C
        {2, 7, 7},                                             // C
        {8, 7, 8},                                             // C {2, 7, 8}, {3, 7, 8},

        {8, 0, 8},                                                                   // E
        {8, 1, 8},                                                                   // E
        {2, 2, 8}, {3, 2, 8}, {4, 2, 8}, {5, 2, 8}, {6, 2, 8}, {7, 2, 8}, {8, 2, 8}, // E
        {2, 3, 8}, {3, 3, 8}, {4, 3, 8}, {5, 3, 8}, {6, 3, 8}, {7, 3, 8}, {8, 3, 8}, // E
        {8, 4, 8},                                                                   // E
        {2, 5, 8}, {3, 5, 8}, {4, 5, 8}, {5, 5, 8}, {6, 5, 8}, {7, 5, 8}, {8, 5, 8}, // E
        {2, 6, 8}, {3, 6, 8}, {4, 6, 8}, {5, 6, 8}, {6, 6, 8}, {7, 6, 8}, {8, 6, 8}, // E

        {8, 0, 7},                                                                   // E
        {8, 1, 7},                                                                   // E
        {2, 2, 7}, {3, 2, 7}, {4, 2, 7}, {5, 2, 7}, {6, 2, 7}, {7, 2, 7}, {8, 2, 7}, // E
        {2, 3, 7}, {3, 3, 7}, {4, 3, 7}, {5, 3, 7}, {6, 3, 7}, {7, 3, 7}, {8, 3, 7}, // E
        {8, 4, 7},                                                                   // E
        {2, 5, 7}, {3, 5, 7}, {4, 5, 7}, {5, 5, 7}, {6, 5, 7}, {7, 5, 7}, {8, 5, 7}, // E
        {2, 6, 7}, {3, 6, 7}, {4, 6, 7}, {5, 6, 7}, {6, 6, 7}, {7, 6, 7}, {8, 6, 7}, // E
    };

    int size = 9;
    float center = size / 2.0f;
    MCEngine::Entity cubes = AddEmptyEntity(
        "Cubes", MCEngine::TransformComponent(glm::vec3(0.0f), glm::vec3(0, 50.0f, 0.0f), glm::vec3(1.0f)));
    for (int x = 0; x < size; x++)
    {
        for (int y = 0; y < size; y++)
        {
            for (int z = 0; z < size; z++)
            {
                if (positions.find({x, y, z}) != positions.end())
                    continue;

                MCEngine::Entity cube =
                    Add3DObject("Cube" + std::to_string(x) + std::to_string(y) + std::to_string(z),
                                MCEngine::TransformComponent(glm::vec3(x - center, y - center, z - center)));
                cube.GetComponent<MCEngine::RelationshipComponent>()->Parent = cubes;
                cubes.GetComponent<MCEngine::RelationshipComponent>()->AddChild(cube);
            }
        }
    }
}