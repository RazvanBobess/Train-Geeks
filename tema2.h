#pragma once

#include <string>
#include <vector>
#include <queue>

#include "components/simple_scene.h"
#include "components/transform.h"
#include "components/text_renderer.h"

#include "lab_m1/tema2/camera.h"
#include "lab_m1/tema2/object.h"
#include "lab_m1/tema2/rails.h"

#define TRAIN_PROXIMITY 0.5f
#define NUMBER_OF_ORDERS 3

#define INITIAL_GAME_TIME 180.f
#define MIN_GAME_TIME 30.f
#define MAX_GAME_TIME 360.f

namespace t2
{
    class Tema2 : public gfxc::SimpleScene
    {
    public:

        enum TrainType {
            TRAIN = 0,
            CARRIAGE = 1
        };

        struct Train {
			glm::ivec2 gridPos;
            TrainType type;
            Direction trainDir;
            float progress;
            float speed;

            Direction nextDir;
            bool request;
        };

        Tema2();
        ~Tema2();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        glm::vec3 lerp(glm::vec3 start, glm::vec3 end, float t);

        void UpdateTrain(Train& train, float deltaTime, RailGrid& grid);

        Direction turnLeft(Direction dir);
        Direction turnRight(Direction dir);
        Direction goBack(Direction dir);

        glm::vec3 gridToWorld(int x, int y);
        glm::ivec2 worldToGrid(const glm::vec3& pos);

        glm::vec3 getTrainPos(const Train& train, const RailGrid& grid);
        glm::vec3 getCarriagePos(int index);

        float yawFromDir(Direction dir);

        void DrawTrain(const Train& train, const RailGrid& grid);
        void DrawCarriages();
        void RenderRails();
		void RenderRailsMini();
        void RenderOrders();
		void DrawTrainMini(const Train& train, const RailGrid& grid);
        void DrawStations();
        void DrawStation(glm::vec3 position, Mesh* stationMesh);
        void DrawStationsMini();
        void DrawStationMini(glm::vec3 position, Mesh* stationMesh);

        void DrawPads();
        void DrawPadMini();

        bool isInStationProximity(const glm::vec3& trainPos, glm::vec2& padPos);

		void GameOn(float deltaTime);
        void UpdateGame(float deltaTime);
		void MainMenu(float deltaTime);

        void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture = NULL);
        void RenderMeshMini(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture = NULL);
        void MinimapRender();

        glm::vec3 directionToVector(Direction dir);
        glm::vec3 directionToWorld(Direction dir);

        Direction intToDirection(int dirInt);
        int directionToInt(Direction dir);
		Direction opposite(Direction dir);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

    protected:
        std::vector<Train> trains;

        int numberOfTrains = 1;
        int numberOfCarriages = 1;

        RailGrid railsGrid{ 50, 50 };
        float CELL_SIZE = 1.f;
        int gridWidth = 50;
        int gridHeight = 50;

		glm::vec3 RAIL_PIVOT_OFF = glm::vec3(0.5f, 0.f, 0.5f);

        int input;
        int direction;
        bool trainWaiting;

        std::deque<glm::vec3> trainPath;
        const float CARRIAGE_DISTANCE = 1.5f;

        camera::Camera* camera;
        camera::Camera* minimapCam;

        glm::mat4 minimapProjMatrix;
        glm::mat4 projectionMatrix;
        bool renderCameraTarget;

		gfxc::TextRenderer* textRenderer;

        std::vector<glm::vec2> stationPositions;
        std::vector<glm::vec2> padsPositions;

        bool startGame;
        bool goBackMenu;
        float wantedSpeed;
        float maxSpeed = 7.f;
        float minSpeed = 2.f;

		int difficultyLevel;

        float gameTime;
        float timeElapsed;
        float lastPadCheck;

        float ordersCooldown;

        unsigned int totalOrders = 0;
		unsigned int devOrders = 0;
        unsigned int ordersType = 3;
		std::vector<unsigned int> currentOrders;
		std::vector<unsigned int> collectedOrders;

        std::unordered_map<std::string, Texture2D*> mapTextures;
    };
}