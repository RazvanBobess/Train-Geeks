#pragma once

#include <string>
#include <vector>
#include <queue>

#include "components/simple_scene.h"
#include "components/transform.h"

#include "lab_m1/tema2/camera.h"
#include "lab_m1/tema2/object.h"
#include "lab_m1/tema2/rails.h"

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
                RailSegment* currentRail = nullptr;
                TrainType type;
                Direction trainDir;
                float progress;
                float speed;
            };

            Tema2();
            ~Tema2();

            void Init() override;

        private:
            void FrameStart() override;
            void Update(float deltaTimeSeconds) override;
            void FrameEnd() override;

            glm::vec3 lerp(glm::vec3 start, glm::vec3 end, float t);

            RailSegment* bridgeExit(RailSegment* bridgeRoot, Direction dir);
            void UpdateTrain(Train& train, float deltaTime);

            inline glm::vec3 gridToWorld(const Vec2i& gridPos) {
                float offsetx = (gridWidth * CELL_SIZE) / 2.f;
                float offsety = (gridHeight * CELL_SIZE) / 2.f;

                return glm::vec3(gridPos.x * CELL_SIZE - offsetx, 0.8f, gridPos.y * CELL_SIZE - offsety);
            }

            glm::vec3 getRailPos(const Cell& cell);
            float getRailYaw(const RailSegment& rail);
            glm::vec3 directionToWorld(Direction dir);
            glm::vec3 getTrainPos(const Train& train);

            RenderTransform buildRailTransform(const Cell& cell);
            glm::mat4 buildModelMatrix(const RenderTransform& rt);

            void DrawTrain(const Train& train);
            void RenderRails(const RailGrid& grid);

            void RenderMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix, Texture2D *texture=NULL);
            void RenderMeshMini(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix, Texture2D *texture = NULL);
            void MinimapRender();
            glm::vec3 directionToVector(Direction dir);

            Direction turnLeft(Direction dir);
            Direction turnRight(Direction dir);
            Direction goBack(Direction dir);

            Direction intToDirection(int dirInt);
            int directionToInt(Direction dir);

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

            RailGrid railsGrid;
            float CELL_SIZE = 1.f;
            int gridWidth;
            int gridHeight;

            int direction;
            int availableDirections[4];
            int firstAvailableDirection;
            bool railChanged = false;

            camera::Camera *camera;
            camera::Camera *minimapCam;

            glm::mat4 minimapProjMatrix;
            glm::mat4 projectionMatrix;
            bool renderCameraTarget;

            std::unordered_map<std::string, Texture2D *> mapTextures;
    };
}