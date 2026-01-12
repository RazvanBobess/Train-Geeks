#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"

#define YELLOW_COLOR glm::vec3(1.0f, 1.f, 0.f)
#define GREEN_COLOR glm::vec3(0.f, 1.f, 0.f)
#define DARK_GREEN_COLOR glm::vec3(0.f, 0.5f, 0.f)
#define BLUE_COLOR glm::vec3(0.f, 0.f, 1.f)
#define PURPLE_COLOR glm::vec3(1.f, 0.f, 1.f)
#define RED_COLOR glm::vec3(1.f, 0.f, 0.f)
#define GRAY_COLOR glm::vec3(0.75f, 0.75f, 0.75f)
#define DARK_GRAY_COLOR glm::vec3(0.2f, 0.2f, 0.2f)
#define WHITE_COLOR glm::vec3(1.f, 1.f, 1.f)
#define ORANGE_COLOR glm::vec3(0.8f, 0.5f, 0.2f)
#define BROWN_COLOR glm::vec3(0.6f, 0.3f, 0.1f)

namespace object3D
{
    Mesh* CreateLocomotive(const std::string &name, glm::vec3 position);

    Mesh* CreateCarriage(const std::string &name, glm::vec3 position);

    Mesh* CreateRail(const std::string &name, glm::vec3 position);

    Mesh* CreateBridgeRail(const std::string &name, glm::vec3 position);

	Mesh* CreateTunnelRail(const std::string& name, glm::vec3 position);

    Mesh* CreateTerrain(const std::string &name, glm::vec3 position);

    Mesh* CreateSphere(const std::string &name, glm::vec3 position);

    Mesh* CreateStation1(const std::string &name, glm::vec3 position);

    Mesh* CreateStation2(const std::string &name, glm::vec3 position);
    
    Mesh* CreateStation3(const std::string &name, glm::vec3 position);

	Mesh* RenderCube(const std::string& name, glm::vec3 position);

	Mesh* CreateLog(const std::string& name, glm::vec3 position);

	Mesh* CreatePad(const std::string& name, glm::vec3 position);

	Mesh* CreateMountain(const std::string& name, glm::vec3 position);
}