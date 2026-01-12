#include "lab_m1/tema2/tema2.h"
#include "lab_m1/tema2/object.h"
#include "lab_m1/tema2/camera.h"
#include "lab_m1/tema2/rails.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace t2;

Tema2::Tema2() {
}

Tema2::~Tema2() {
}

void Tema2::Init() {
    // Initialization code here
    camera = new camera::Camera();
    camera->Set(glm::vec3(0, 2, 5), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
    renderCameraTarget = false;

    minimapCam = new camera::Camera();
    minimapCam->Set(glm::vec3(0, 30, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, -1));

    const string sourceTextureDir = PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "textures");

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "water_m.png").c_str(), GL_REPEAT);
        mapTextures["water_m"] = texture;
    }

    projectionMatrix = glm::perspective(RADIANS(60.0f), window->props.aspectRatio, 0.01f, 200.0f);
    minimapProjMatrix = glm::ortho(-30.f, 30.f, -30.f, 30.f, -100.f, 100.f);

    {
        Shader* shader = new Shader("VC");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;

        mesh = new Mesh("water");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;

        Mesh* terrain = object3D::CreateTerrain("terrain", glm::vec3(0.f, 0.f, 0.f));
        AddMeshToList(terrain);

        Mesh* sphere = object3D::CreateSphere("sphere", glm::vec3(0.f, 0.f, 0.f));
        AddMeshToList(sphere);

        Mesh* bridgeRail = object3D::CreateBridgeRail("bridgeRail", glm::vec3(0.f, 0.f, 0.f));
        AddMeshToList(bridgeRail);

        Mesh* rail = object3D::CreateRail("rail", glm::vec3(0.f, 0.f, 0.f));
        AddMeshToList(rail);

		Mesh* tunnelRail = object3D::CreateTunnelRail("tunnelRail", glm::vec3(0.f, 0.f, 0.f));
		AddMeshToList(tunnelRail);

        Mesh* carriage1 = object3D::CreateCarriage("carriage1", glm::vec3(0.f, 0.f, 0.f));
        AddMeshToList(carriage1);

        Mesh* locomotive = object3D::CreateLocomotive("locomotive", glm::vec3(0.f, 0.f, 0.f));
        AddMeshToList(locomotive);

        Mesh* station1 = object3D::CreateStation1("station1", glm::vec3(0.f, 0.f, 0.f));
        AddMeshToList(station1);

        Mesh* station2 = object3D::CreateStation2("station2", glm::vec3(0.f, 0.f, 0.f));
        AddMeshToList(station2);

        Mesh* station3 = object3D::CreateStation3("station3", glm::vec3(0.f, 0.f, 0.f));
        AddMeshToList(station3);

        Mesh* station4 = object3D::CreateStation4("station4", glm::vec3(0.f, 0.f, 0.f));
        AddMeshToList(station4);

        Mesh* cube = object3D::RenderCube("cube", glm::vec3(0.f, 0.f, 0.f));
        AddMeshToList(cube);

		Mesh* log = object3D::CreateLog("log", glm::vec3(0.f, 0.f, 0.f));
		AddMeshToList(log);

        Mesh* barrel = object3D::CreateBarrel("barrel", glm::vec3(0.f, 0.f, 0.f));
        AddMeshToList(barrel);

        Mesh* powder = object3D::CreatePowder("powder", glm::vec3(0.f, 0.f, 0.f));
        AddMeshToList(powder);

		Mesh* pad = object3D::CreatePad("pad", glm::vec3(0.f, 0.f, 0.f));
		AddMeshToList(pad);

		Mesh* mountain = object3D::CreateMountain("mountain", glm::vec3(0.f, 0.f, 0.f));
		AddMeshToList(mountain);
    }

    {
        stationPositions.push_back(glm::vec2(-3.f, -7.4f));
        stationPositions.push_back(glm::vec2(18.5f, 13.f));
        stationPositions.push_back(glm::vec2(-16.f, 17.f));
        stationPositions.push_back(glm::vec2(-16.f, -14.f));
    }

    {
        padsPositions.push_back(glm::vec2(0.5f, -7.f));
        padsPositions.push_back(glm::vec2(21.5f, 14.f));
        padsPositions.push_back(glm::vec2(-18.5f, 18.f));
        padsPositions.push_back(glm::vec2(-18.5f, -12.5f));
    }

    {
		railsGrid.setStraightRail(25, 27, Direction::NORTH, RailType::BRIDGE_RAIL);
        
		railsGrid.connectCells(25, 7, 25, 27, RailType::RAIL);
		railsGrid.connectCells(25, 28, 25, 46, RailType::RAIL);
		railsGrid.setCellNeighbor(railsGrid.getCell(25, 27), Direction::NORTH, railsGrid.getCell(25, 26));
        railsGrid.setCellNeighbor(railsGrid.getCell(25, 27), Direction::SOUTH, railsGrid.getCell(25, 28));

		Cell& sw1 = railsGrid.getCell(25, 7);
		
        railsGrid.connectSwich(sw1, {
            {Direction::WEST, &railsGrid.getCell(25, 7)},
            {Direction::EAST, &railsGrid.getCell(25, 7)},
            {Direction::SOUTH, &railsGrid.getCell(25, 7)}
			});

		railsGrid.connectCells(24, 7, 6, 7, RailType::RAIL);
        railsGrid.connectCells(6, 7, 6, 27, RailType::RAIL);

		Cell& sw2 = railsGrid.getCell(6, 7);
        railsGrid.connectSwich(sw2, {
           {Direction::EAST, &railsGrid.getCell(6, 7)},
           {Direction::SOUTH, &railsGrid.getCell(6, 7)}
            });

		railsGrid.setStraightRail(6, 27, Direction::NORTH, RailType::BRIDGE_RAIL);

        railsGrid.connectCells(6, 28, 6, 36, RailType::RAIL);

        Cell& sw3 = railsGrid.getCell(6, 27);
        sw3.connections.fill(false);
        sw3.neighbors.fill(nullptr);

        railsGrid.setCellNeighbor(sw3, Direction::NORTH, railsGrid.getCell(6, 27), true);
        railsGrid.setCellNeighbor(sw3, Direction::SOUTH, railsGrid.getCell(6, 27), true);

		railsGrid.connectCells(7, 35, 26, 35, RailType::RAIL);
		railsGrid.connectCells(6, 36, 6, 47, RailType::RAIL);

        Cell& sw4 = railsGrid.getCell(6, 35);
        railsGrid.connectSwich(sw4, {
           {Direction::WEST, &railsGrid.getCell(6, 35)},
           {Direction::SOUTH, &railsGrid.getCell(6, 35)},
           {Direction::NORTH, &railsGrid.getCell(6, 35)}
            });
        railsGrid.setCellNeighbor(railsGrid.getCell(7, 35), Direction::WEST, sw4);

        railsGrid.connectCells(6, 46, 26, 46, RailType::RAIL);

		Cell& sw6 = railsGrid.getCell(6, 46);
        railsGrid.connectSwich(sw6, {
           {Direction::EAST, &railsGrid.getCell(6, 46)},
           {Direction::NORTH, &railsGrid.getCell(6, 46)}
            });

		Cell& sw7 = railsGrid.getCell(25, 46);
        railsGrid.connectSwich(sw7, {
           {Direction::WEST, &railsGrid.getCell(25, 46)},
           {Direction::NORTH, &railsGrid.getCell(25, 46)},
           {Direction::EAST, &railsGrid.getCell(25, 46)}
            });

        Cell& sw5 = railsGrid.getCell(25, 35);
        railsGrid.connectSwich(sw5, {
           {Direction::WEST, &railsGrid.getCell(25, 34)},
           {Direction::SOUTH, &railsGrid.getCell(26, 35)},
           {Direction::NORTH, &railsGrid.getCell(24, 35)},
           {Direction::EAST, &railsGrid.getCell(25, 36)}
            });

        railsGrid.setCellNeighbor(railsGrid.getCell(25, 36), Direction::WEST, sw5);
        railsGrid.setCellNeighbor(railsGrid.getCell(25, 34), Direction::WEST, sw5);
        railsGrid.setCellNeighbor(railsGrid.getCell(24, 35), Direction::NORTH, sw5);
        railsGrid.setCellNeighbor(railsGrid.getCell(26, 35), Direction::SOUTH, sw5);

		railsGrid.connectCells(26, 35, 39, 35, RailType::RAIL);
		railsGrid.connectCells(26, 46, 39, 46, RailType::RAIL);

        railsGrid.connectCells(26, 7, 39, 7, RailType::RAIL);
		railsGrid.connectCells(38, 8, 38, 19, RailType::TUNNEL_RAIL);
        railsGrid.connectCells(39, 7, 46, 7, RailType::RAIL);

        Cell& sw8 = railsGrid.getCell(38, 7);
        railsGrid.connectSwich(sw8, {
           {Direction::WEST, &railsGrid.getCell(38, 7)},
           {Direction::SOUTH, &railsGrid.getCell(38, 7)},
           {Direction::EAST, &railsGrid.getCell(38, 7)}
            });

		railsGrid.connectCells(45, 8, 45, 20, RailType::RAIL);
        Cell& sw9 = railsGrid.getCell(45, 7);
        railsGrid.connectSwich(sw9, {
           {Direction::WEST, &railsGrid.getCell(45, 7)},
           {Direction::SOUTH, &railsGrid.getCell(45, 7)}
            });

		railsGrid.connectCells(44, 19, 37, 19, RailType::RAIL);
        Cell& sw10 = railsGrid.getCell(45, 19);
        railsGrid.connectSwich(sw10, {
           {Direction::WEST, &railsGrid.getCell(45, 19)},
           {Direction::NORTH, &railsGrid.getCell(45, 19)}
            });

        railsGrid.connectCells(38, 20, 38, 27, RailType::RAIL);
        Cell& sw11 = railsGrid.getCell(38, 19);
        railsGrid.connectSwich(sw11, {
           {Direction::EAST, &railsGrid.getCell(38, 19)},
           {Direction::NORTH, &railsGrid.getCell(38, 19)},
           {Direction::SOUTH,& railsGrid.getCell(38, 19)}
            });

        railsGrid.setStraightRail(38, 27, Direction::NORTH, RailType::BRIDGE_RAIL);
        railsGrid.connectCells(38, 28, 38, 36, RailType::RAIL);

        Cell& sw12 = railsGrid.getCell(38, 27);
        sw12.connections.fill(false);
        sw12.neighbors.fill(nullptr);

        railsGrid.setCellNeighbor(sw12, Direction::NORTH, railsGrid.getCell(38, 27), true);
        railsGrid.setCellNeighbor(sw12, Direction::SOUTH, railsGrid.getCell(38, 27), true);

        railsGrid.connectCells(39, 35, 47, 35, RailType::RAIL);
		railsGrid.connectCells(38, 36, 38, 47, RailType::RAIL);
        railsGrid.connectCells(39, 46, 47, 46, RailType::RAIL);

        Cell& sw13 = railsGrid.getCell(38, 35);
        railsGrid.connectSwich(sw13, {
           {Direction::EAST, &railsGrid.getCell(38, 35)},
           {Direction::NORTH, &railsGrid.getCell(38, 35)},
           {Direction::SOUTH,&railsGrid.getCell(38, 35)},
           {Direction::WEST,&railsGrid.getCell(38, 35)}
            });

        Cell& sw14 = railsGrid.getCell(38, 46);
        railsGrid.connectSwich(sw14, {
           {Direction::EAST, &railsGrid.getCell(38, 46)},
           {Direction::NORTH, &railsGrid.getCell(38, 46)},
           {Direction::WEST,&railsGrid.getCell(38, 46)}
            });

		railsGrid.connectCells(46, 36, 46, 46, RailType::RAIL);
        Cell& sw15 = railsGrid.getCell(46, 35);
        railsGrid.connectSwich(sw15, {
           {Direction::SOUTH, &railsGrid.getCell(46, 35)},
           {Direction::WEST,&railsGrid.getCell(46, 35)}
            });

        Cell& sw16 = railsGrid.getCell(46, 46);
        railsGrid.connectSwich(sw16, {
           {Direction::NORTH, &railsGrid.getCell(46, 46)},
           {Direction::WEST,&railsGrid.getCell(46, 46)}
            });

		railsGrid.buildDefaultNeighbors();
    }

    direction = 0;
	trainWaiting = true;

    {
        Train train;
        Train carriage;

        train.type = TrainType::TRAIN;
        train.trainDir = Direction::NORTH;
        train.gridPos = glm::ivec2(25, 20);
		train.nextDir = Direction::NORTH;
		train.request = false;
        train.progress = 0.f;
        train.speed = 3.f;
        trains.push_back(train);
    }
}

glm::vec3 Tema2::directionToVector(Direction dir) {
    switch (dir) {
    case Direction::NORTH:
        return glm::vec3(0.f, 0.f, -1.f);
    case Direction::EAST:
        return glm::vec3(1.f, 0.f, 0.f);
    case Direction::SOUTH:
        return glm::vec3(0.f, 0.f, 1.f);
    case Direction::WEST:
        return glm::vec3(-1.f, 0.f, 0.f);
    }
    return glm::vec3(0.f, 0.f, 0.f);
}

void Tema2::FrameStart() {
    // Code to execute at the start of each frame
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}

void Tema2::Update(float deltaTimeSeconds) {
	GameOn(deltaTimeSeconds);
}

void Tema2::GameOn(float deltaTime) {
    glm::mat4 modelMatrix;
    glm::mat4 aux_mat;

    {
        glm::vec3 p1 = gridToWorld(25, 7);
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(p1.x, -0.1f, p1.z));
        RenderMesh(meshes["cube"], shaders["VC"], modelMatrix);

        glm::vec3 p2 = gridToWorld(6, 7);
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(p2.x, -0.1f, p2.z));
        RenderMesh(meshes["cube"], shaders["VC"], modelMatrix);

        glm::vec3 p4 = gridToWorld(6, 35);
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(p4.x, -0.1f, p4.z));
        RenderMesh(meshes["cube"], shaders["VC"], modelMatrix);

        glm::vec3 p5 = gridToWorld(25, 35);
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(p5.x, -0.1f, p5.z));
        RenderMesh(meshes["cube"], shaders["VC"], modelMatrix);

        glm::vec3 p6 = gridToWorld(6, 46);
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(p6.x, -0.1f, p6.z));
        RenderMesh(meshes["cube"], shaders["VC"], modelMatrix);

        glm::vec3 p7 = gridToWorld(25, 46);
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(p7.x, -0.1f, p7.z));
        RenderMesh(meshes["cube"], shaders["VC"], modelMatrix);

        glm::vec3 p8 = gridToWorld(38, 7);
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(p8.x, -0.1f, p8.z));
        RenderMesh(meshes["cube"], shaders["VC"], modelMatrix);

        glm::vec3 p9 = gridToWorld(45, 7);
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(p9.x, -0.1f, p9.z));
        RenderMesh(meshes["cube"], shaders["VC"], modelMatrix);

        glm::vec3 p10 = gridToWorld(45, 19);
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(p10.x, -0.1f, p10.z));
        RenderMesh(meshes["cube"], shaders["VC"], modelMatrix);

        glm::vec3 p11 = gridToWorld(38, 19);
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(p11.x, -0.1f, p11.z));
        RenderMesh(meshes["cube"], shaders["VC"], modelMatrix);

        glm::vec3 p13 = gridToWorld(38, 35);
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(p13.x, -0.1f, p13.z));
        RenderMesh(meshes["cube"], shaders["VC"], modelMatrix);

        glm::vec3 p14 = gridToWorld(38, 46);
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(p14.x, -0.1f, p14.z));
        RenderMesh(meshes["cube"], shaders["VC"], modelMatrix);

        glm::vec3 p15 = gridToWorld(46, 35);
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(p15.x, -0.1f, p15.z));
        RenderMesh(meshes["cube"], shaders["VC"], modelMatrix);

        glm::vec3 p16 = gridToWorld(46, 46);
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(p16.x, -0.1f, p16.z));
        RenderMesh(meshes["cube"], shaders["VC"], modelMatrix);
    }

    {
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::rotate(modelMatrix, glm::radians(90.0f), glm::vec3(0.f, 1.f, 0.f));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(4.6f, 0.1f, 50));
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0.1f, 0.5f, 0.f));
        RenderMesh(meshes["water"], shaders["VC"], modelMatrix, mapTextures["water_m"]);

        modelMatrix = glm::mat4(1);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(1.f, 0.2f, 0.457f));

        aux_mat = glm::translate(modelMatrix, glm::vec3(-25.f, 0.f, 54.f));
        RenderMesh(meshes["terrain"], shaders["VC"], aux_mat);

        aux_mat = glm::translate(modelMatrix, glm::vec3(-25.f, 0.f, -6.f));
        RenderMesh(meshes["terrain"], shaders["VC"], aux_mat);

		modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(14.f, 0.8f, -2.f));
		RenderMesh(meshes["mountain"], shaders["VC"], modelMatrix);
    }

    if (renderCameraTarget)
    {
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
        RenderMesh(meshes["sphere"], shaders["VC"], modelMatrix);
    }

    UpdateTrain(trains[0], deltaTime, railsGrid);
    DrawTrain(trains[0], railsGrid);
    DrawStations();
    RenderRails();

    DrawPads();

    MinimapRender();
}

void Tema2::UpdateGame(float deltaTime) {
    // Code for updating game state
}

void Tema2::MainMenu(float deltaTime) {
    // Code for main menu rendering
}

void Tema2::FrameEnd() {
    // Code to execute at the end of each frame
}

glm::vec3 Tema2::lerp(glm::vec3 start, glm::vec3 end, float t) {
    return start + t * (end - start);
}

Direction Tema2::intToDirection(int dirInt) {
    switch (dirInt) {
    case 0:
        return Direction::NORTH;
    case 1:
        return Direction::EAST;
    case 3:
        return Direction::SOUTH;
    case 2:
        return Direction::WEST;
    }
    return Direction::NORTH; // Default case
}

float Tema2::yawFromDir(Direction dir) {
    switch (dir) {
    case Direction::NORTH:
        return glm::radians(0.0f);
    case Direction::EAST:
        return glm::radians(-90.0f);
    case Direction::SOUTH:
        return glm::radians(180.0f);
    case Direction::WEST:
        return glm::radians(90.0f);
    }
    return glm::radians(0.0f);
}

glm::vec3 Tema2::directionToWorld(Direction dir) {
    switch (dir) {
    case Direction::NORTH:
        return glm::vec3(0.f, 0.f, -1.f);
    case Direction::EAST:
        return glm::vec3(1.f, 0.f, 0.f);
    case Direction::SOUTH:
        return glm::vec3(0.f, 0.f, 1.f);
    case Direction::WEST:
        return glm::vec3(-1.f, 0.f, 0.f);
    }
    return glm::vec3(0.f, 0.f, 0.f);
}

glm::vec3 Tema2::getCarriagePos(int index) {
    int sample = int(index * CARRIAGE_DISTANCE / CELL_SIZE * 10);

    if (sample >= trainPath.size()) {
        sample = trainPath.size() - 1;
    }

    return trainPath[sample];
}

glm::vec3 Tema2::gridToWorld(int x, int y) {
    float wx = (x - gridWidth / 2.f) * CELL_SIZE;
    float wz = (y - gridHeight / 2.f) * CELL_SIZE;

    return glm::vec3(wx, 0.8f, wz);
}

glm::ivec2 Tema2::worldToGrid(const glm::vec3& pos) {
    float halfW = gridWidth * CELL_SIZE / 2.f;
    float halfH = gridHeight * CELL_SIZE / 2.f;

    int x = static_cast<int>((pos.x + halfW) / CELL_SIZE);
    int y = static_cast<int>((pos.z + halfH) / CELL_SIZE);

    return glm::ivec2(x, y);
}

Direction Tema2::opposite(Direction dir) {
    switch (dir) {
    case Direction::NORTH:
        return Direction::SOUTH;
    case Direction::EAST:
        return Direction::WEST;
    case Direction::SOUTH:
        return Direction::NORTH;
    case Direction::WEST:
        return Direction::EAST;
    }
    return Direction::NORTH; // Default case
}

glm::vec3 Tema2::getTrainPos(const Train& train, const RailGrid& grid) {

    glm::vec3 pos0 = gridToWorld(train.gridPos.x, train.gridPos.y);

    glm::ivec2 offset = grid.dirOffsetConst(train.trainDir);

    glm::vec3 pos1 = gridToWorld(train.gridPos.x + offset.x, train.gridPos.y + offset.y);

    return glm::mix(pos0, pos1, train.progress);
}

void Tema2::UpdateTrain(Train& train, float dt, RailGrid& grid) {
    train.progress += train.speed * dt;

    while (train.progress >= 1.f) {
        train.progress -= 1.f;

        int nx, ny;
        Cell& curCell = grid.getCell(train.gridPos.x, train.gridPos.y);

        if (curCell.isSwitch) {
            if (train.request) {
                int reqDir = directionToInt(train.nextDir);
                if (curCell.connections[reqDir]) {
                    train.trainDir = train.nextDir;
                    train.request = false;
                    trainWaiting = false;
                }
                else {

                    trainWaiting = true;
                    train.progress = 1.f;
                    return;
                }
            }
            else {
                trainWaiting = true;
                train.progress = 1.f;
                return;
            }
        }

        if (!grid.getNextCell(train.gridPos.x, train.gridPos.y, train.trainDir, nx, ny)) {

            train.progress = 1.f;
            return;
        }

        train.gridPos = { nx, ny };

        Cell& newCell = grid.getCell(nx, ny);
        if (newCell.isSwitch) {
            trainWaiting = true;
            return;
        }
    }
}

void Tema2::DrawTrain(const Train& train, const RailGrid& grid) {
 
	glm::vec3 pos = getTrainPos(train, grid);
    float yaw = yawFromDir(train.trainDir);

    glm::mat4 modelMatrix(1.0f);
    modelMatrix = glm::translate(modelMatrix, pos);
    modelMatrix = glm::rotate(modelMatrix, yaw, glm::vec3(0, 1, 0));
    RenderMesh(meshes["locomotive"], shaders["VC"], modelMatrix);
}

void Tema2::DrawTrainMini(const Train& train, const RailGrid& grid) {

    glm::vec3 pos = getTrainPos(train, grid);
    float yaw = yawFromDir(train.trainDir);

    glm::mat4 modelMatrix(1.0f);
    modelMatrix = glm::translate(modelMatrix, pos);
    modelMatrix = glm::rotate(modelMatrix, yaw, glm::vec3(0, 1, 0));
    RenderMeshMini(meshes["cube"], shaders["VC"], modelMatrix);
}

void Tema2::DrawCarriages() {
    for (int i = 0; i < numberOfCarriages; i++) {
        glm::vec3 pos = getCarriagePos(i + 1);

        glm::mat4 modelMatrix(1.0f);
        modelMatrix = glm::translate(modelMatrix, pos);

        RenderMesh(meshes["carriage1"], shaders["VC"], modelMatrix);
    }
}

void Tema2::RenderRails() {
    for (int y = 0; y < gridHeight; y++) {
        for (int x = 0; x < gridWidth; x++) {
            if (!railsGrid.hasRail(x, y)) continue;

            Cell c = railsGrid.getCell(x, y);
            glm::vec3 pos = gridToWorld(x, y);

			float yaw = yawFromDir(c.dir);

            glm::mat4 modelMatrix(1.0f);
            modelMatrix = glm::translate(modelMatrix, pos);
            modelMatrix = glm::rotate(modelMatrix, yaw, glm::vec3(0, 1, 0));

            switch (railsGrid.getRailType(x, y)) {
                case RailType::RAIL:
                    RenderMesh(meshes["rail"], shaders["VC"], modelMatrix);
                    break;
                case RailType::BRIDGE_RAIL:
                    RenderMesh(meshes["bridgeRail"], shaders["VC"], modelMatrix);
                    break;
                case RailType::TUNNEL_RAIL:
					RenderMesh(meshes["tunnelRail"], shaders["VC"], modelMatrix);
                    break;
                default:
                    break;
			}

        }
    }
}

void Tema2::RenderRailsMini() {
    for (int y = 0; y < gridHeight; y++) {
        for (int x = 0; x < gridWidth; x++) {
            if (!railsGrid.hasRail(x, y)) continue;

            Cell c = railsGrid.getCell(x, y);
            glm::vec3 pos = gridToWorld(x, y);
            
            float yaw = yawFromDir(c.dir);

            glm::mat4 modelMatrix(1.0f);
            modelMatrix = glm::translate(modelMatrix, pos);
            modelMatrix = glm::rotate(modelMatrix, yaw, glm::vec3(0, 1, 0));

            switch (railsGrid.getRailType(x, y)) {
            case RailType::RAIL:
                RenderMeshMini(meshes["rail"], shaders["VC"], modelMatrix);
                break;
            case RailType::BRIDGE_RAIL:
                RenderMeshMini(meshes["bridgeRail"], shaders["VC"], modelMatrix);
                break;
            case RailType::TUNNEL_RAIL:
				RenderMeshMini(meshes["tunnelRail"], shaders["VC"], modelMatrix);
                break;
            default:
                break;
            }

        }
    }
}

void Tema2::DrawStation(glm::vec3 position, Mesh* stationMesh) {
    glm::mat4 modelMatrix(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
    RenderMesh(stationMesh, shaders["VC"], modelMatrix);
}

void Tema2::DrawStations() {
    glm::mat4 model;
    for (int i = 0; i < stationPositions.size(); i++) {
        glm::vec3 pos = glm::vec3(stationPositions[i].x, 0.8f, stationPositions[i].y);

        switch (i) {
            case 0:
                DrawStation(pos, meshes["station1"]);
                break;
            case 1:
                DrawStation(pos, meshes["station2"]);

                model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(pos.x, 3.f, pos.z));
                model = glm::rotate(model, glm::radians(45.0f), glm::vec3(1.f, 0.f, 0.f));
                model = glm::scale(model, glm::vec3(2.f, 4.f, 3.f));
                RenderMesh(meshes["log"], shaders["VC"], model);

                break;
            case 2:
                DrawStation(pos, meshes["station3"]);

                model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(pos.x - 0.2f, 5.f, pos.z + 0.3f));
                model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(1.f, 0.f, 0.f));
                model = glm::scale(model, glm::vec3(1.5f));
                RenderMesh(meshes["barrel"], shaders["VC"], model);

                break;
            case 3:
                DrawStation(pos, meshes["station4"]);

                model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(pos.x + 0.6f, 4.f, pos.z + 1.1f));
                model = glm::rotate(model, glm::radians(35.0f), glm::vec3(1.f, 0.f, 0.f));
                model = glm::scale(model, glm::vec3(4.f));
                RenderMesh(meshes["powder"], shaders["VC"], model);

                break;
        }
    }
}

void Tema2::DrawStationMini(glm::vec3 position, Mesh* stationMesh) {
    glm::mat4 modelMatrix(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
    RenderMeshMini(stationMesh, shaders["VC"], modelMatrix);
}

void Tema2::DrawStationsMini() {
    for (int i = 0; i < stationPositions.size(); i++) {
        glm::vec3 pos = glm::vec3(stationPositions[i].x, 0.4f, stationPositions[i].y);

        switch (i) {
            case 0:
                DrawStationMini(pos, meshes["station1"]);
                break;
            case 1:
                DrawStationMini(pos, meshes["station2"]);
                break;
            case 2:
                DrawStationMini(pos, meshes["station3"]);
                break;
            case 3:
                DrawStationMini(pos, meshes["station4"]);
                break;
        }
    }
}

void Tema2::DrawPads() {
    for (int i = 0; i < padsPositions.size(); i++) {
        glm::vec3 pos = glm::vec3(padsPositions[i].x, 0.4f, padsPositions[i].y);

        glm::mat4 modelMatrix(1.0f);
        modelMatrix = glm::translate(modelMatrix, pos);
        RenderMesh(meshes["pad"], shaders["VC"], modelMatrix);
    }
}

void Tema2::DrawPadMini() {
    for (int i = 0; i < padsPositions.size(); i++) {
        glm::vec3 pos = glm::vec3(padsPositions[i].x, 0.8f, padsPositions[i].y);

        glm::mat4 modelMatrix(1.0f);
        modelMatrix = glm::translate(modelMatrix, pos);
        RenderMeshMini(meshes["pad"], shaders["VC"], modelMatrix);
    }
}

bool Tema2::isInStationProximity(const glm::vec3& trainPos, glm::vec2& padPos) {
    if (trainPos.x >= padPos.x - TRAIN_PROXIMITY && trainPos.x <= padPos.x + TRAIN_PROXIMITY &&
        trainPos.z >= padPos.y - TRAIN_PROXIMITY && trainPos.z <= padPos.y + TRAIN_PROXIMITY) {
        return true;
    }
    return false;
}

void Tema2::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture) {
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    glUseProgram(shader->program);

    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    glm::mat4 viewMatrix = camera->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    glm::mat4 projectionMatrix = this->projectionMatrix;
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    if (mesh == meshes["water"]) {
        GLint isWater = glGetUniformLocation(shader->program, "isWater");
        glUniform1i(isWater, 1);

        float time = Engine::GetElapsedTime();
        GLint timeLocation = glGetUniformLocation(shader->program, "Time");
        glUniform1f(timeLocation, time);

    }
    else {
        GLint isWater = glGetUniformLocation(shader->program, "isWater");
        glUniform1i(isWater, 0);
    }

    if (mesh == meshes["mountain"]) {
        GLint isMountain = glGetUniformLocation(shader->program, "isMountain");
        glUniform1i(isMountain, 1);
    }
    else {
        GLint isMountain = glGetUniformLocation(shader->program, "isMountain");
        glUniform1i(isMountain, 0);
	}

    GLint useTexture = glGetUniformLocation(shader->program, "useTexture");

    if (texture)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture->GetTextureID());
        glUniform1i(glGetUniformLocation(shader->program, "texture1"), 0);
        glUniform1i(useTexture, 1);
    }
    else {
        glUniform1i(useTexture, 0);
    }

    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}

void Tema2::RenderMeshMini(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture) {
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    glUseProgram(shader->program);

    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    glm::mat4 viewMatrix = minimapCam->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    glm::mat4 projectionMatrix = this->minimapProjMatrix;
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    if (meshes["water"] == mesh) {
        GLint isWater = glGetUniformLocation(shader->program, "isWater");
        glUniform1i(isWater, 1);

        float time = Engine::GetElapsedTime();
        GLint timeLocation = glGetUniformLocation(shader->program, "Time");
        glUniform1f(timeLocation, time);

    }
    else {
        GLint isWater = glGetUniformLocation(shader->program, "isWater");
        glUniform1i(isWater, 0);
    }

    if (mesh == meshes["mountain"]) {
        GLint isMountain = glGetUniformLocation(shader->program, "isMountain");
        glUniform1i(isMountain, 1);
    }
    else {
        GLint isMountain = glGetUniformLocation(shader->program, "isMountain");
        glUniform1i(isMountain, 0);
	}

    GLint useTexture = glGetUniformLocation(shader->program, "useTexture");

    if (texture)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture->GetTextureID());
        glUniform1i(glGetUniformLocation(shader->program, "texture1"), 0);
        glUniform1i(useTexture, 1);
    }
    else {
        glUniform1i(useTexture, 0);
    }

    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}

void Tema2::MinimapRender() {
    glm::ivec2 resolution = window->GetResolution();

    int miniViewPortSize = 200;
    glViewport(resolution.x - miniViewPortSize - 10, resolution.y - miniViewPortSize - 10, miniViewPortSize, miniViewPortSize);
    glClear(GL_DEPTH_BUFFER_BIT);

    glm::mat4 modelMatrix = glm::mat4(1);
    glm::mat4 aux_mat;

    {
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(1.f, 0.2f, 0.457f));

        aux_mat = glm::translate(modelMatrix, glm::vec3(-25.f, 0.f, 54.f));
        RenderMeshMini(meshes["terrain"], shaders["VC"], aux_mat);

        aux_mat = glm::translate(modelMatrix, glm::vec3(-25.f, 0.f, -6.f));
        RenderMeshMini(meshes["terrain"], shaders["VC"], aux_mat);

        glm::vec3 p1 = gridToWorld(25, 7);
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(p1.x, -0.1f, p1.z));
        RenderMeshMini(meshes["cube"], shaders["VC"], modelMatrix);

        glm::vec3 p2 = gridToWorld(6, 7);
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(p2.x, -0.1f, p2.z));
        RenderMeshMini(meshes["cube"], shaders["VC"], modelMatrix);

        glm::vec3 p4 = gridToWorld(6, 35);
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(p4.x, -0.1f, p4.z));
        RenderMeshMini(meshes["cube"], shaders["VC"], modelMatrix);

        glm::vec3 p5 = gridToWorld(25, 35);
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(p5.x, -0.1f, p5.z));
        RenderMeshMini(meshes["cube"], shaders["VC"], modelMatrix);

        glm::vec3 p6 = gridToWorld(6, 46);
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(p6.x, -0.1f, p6.z));
        RenderMeshMini(meshes["cube"], shaders["VC"], modelMatrix);

        glm::vec3 p7 = gridToWorld(25, 46);
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(p7.x, -0.1f, p7.z));
        RenderMeshMini(meshes["cube"], shaders["VC"], modelMatrix);

        glm::vec3 p8 = gridToWorld(38, 7);
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(p8.x, -0.1f, p8.z));
        RenderMeshMini(meshes["cube"], shaders["VC"], modelMatrix);

        glm::vec3 p9 = gridToWorld(45, 7);
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(p9.x, -0.1f, p9.z));
        RenderMeshMini(meshes["cube"], shaders["VC"], modelMatrix);

        glm::vec3 p10 = gridToWorld(45, 19);
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(p10.x, -0.1f, p10.z));
        RenderMeshMini(meshes["cube"], shaders["VC"], modelMatrix);

        glm::vec3 p11 = gridToWorld(38, 19);
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(p11.x, -0.1f, p11.z));
        RenderMeshMini(meshes["cube"], shaders["VC"], modelMatrix);

        glm::vec3 p13 = gridToWorld(38, 35);
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(p13.x, -0.1f, p13.z));
        RenderMeshMini(meshes["cube"], shaders["VC"], modelMatrix);

        glm::vec3 p14 = gridToWorld(38, 46);
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(p14.x, -0.1f, p14.z));
        RenderMeshMini(meshes["cube"], shaders["VC"], modelMatrix);

        glm::vec3 p15 = gridToWorld(46, 35);
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(p15.x, -0.1f, p15.z));
        RenderMeshMini(meshes["cube"], shaders["VC"], modelMatrix);

        glm::vec3 p16 = gridToWorld(46, 46);
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(p16.x, -0.1f, p16.z));
        RenderMeshMini(meshes["cube"], shaders["VC"], modelMatrix);
    }

    {
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::rotate(modelMatrix, glm::radians(90.0f), glm::vec3(0.f, 1.f, 0.f));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(4.6f, 0.1f, 50));
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0.1f, 0.5f, 0.0f));
        RenderMeshMini(meshes["water"], shaders["VC"], modelMatrix, mapTextures["water_m"]);

        modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(14.f, 0.8f, -2.f));
		RenderMeshMini(meshes["mountain"], shaders["VC"], modelMatrix);
    }

    RenderRailsMini();
	DrawTrainMini(trains[0], railsGrid);
    DrawStationsMini();

    DrawPadMini();

    glViewport(0, 0, resolution.x, resolution.y);
}

void Tema2::OnInputUpdate(float deltaTime, int mods) {

    // Camera movement

    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float cameraSpeed = 5.0f;

        if (window->KeyHold(GLFW_KEY_W)) {
            camera->MoveForward(cameraSpeed * deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_A)) {
            camera->TranslateRight(-cameraSpeed * deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_S)) {
            camera->MoveForward(-cameraSpeed * deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_D)) {
            camera->TranslateRight(cameraSpeed * deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_Q)) {
            camera->TranslateUpward(-cameraSpeed * deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_E)) {
            camera->TranslateUpward(cameraSpeed * deltaTime);
        }
    }
}

Direction Tema2::turnLeft(Direction dir) {
    switch (dir) {
    case Direction::NORTH:
        return Direction::WEST;
    case Direction::EAST:
        return Direction::NORTH;
    case Direction::SOUTH:
        return Direction::EAST;
    case Direction::WEST:
        return Direction::SOUTH;
    }
    return dir;
}

Direction Tema2::turnRight(Direction dir) {
    switch (dir) {
    case Direction::NORTH:
        return Direction::EAST;
    case Direction::EAST:
        return Direction::SOUTH;
    case Direction::SOUTH:
        return Direction::WEST;
    case Direction::WEST:
        return Direction::NORTH;
    }
    return dir;
}

Direction Tema2::goBack(Direction dir) {
    switch (dir) {
    case Direction::NORTH:
        return Direction::SOUTH;
    case Direction::EAST:
        return Direction::WEST;
    case Direction::SOUTH:
        return Direction::NORTH;
    case Direction::WEST:
        return Direction::EAST;
    }
    return dir;
}

int Tema2::directionToInt(Direction dir) {
    switch (dir) {
    case Direction::NORTH:
        return 0;
    case Direction::EAST:
        return 1;
    case Direction::SOUTH:
        return 3;
    case Direction::WEST:
        return 2;
    }
    return -1;
}

void Tema2::OnKeyPress(int key, int mods) {
    // Handle key press events here
    if (key == GLFW_KEY_T) {
        renderCameraTarget = !renderCameraTarget;
    }

    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {
        return;
	}

    int direction = -1;
	
    if (key == GLFW_KEY_W) direction = 0;
    if (key == GLFW_KEY_D) direction = 1;
    if (key == GLFW_KEY_S) direction = 2;
    if (key == GLFW_KEY_A) direction = 3;

	printf("Key pressed: %d\n", direction);

    if (direction != -1) {
        Direction dir = Direction::NORTH;
        
        switch (direction) {
        case 0:
			dir = trains[0].trainDir;
            break;
        case 1:
			dir = turnRight(trains[0].trainDir);
			break;
        case 2:
			dir = goBack(trains[0].trainDir);
			break;
		case 3:
			dir = turnLeft(trains[0].trainDir);
        }

        Cell& cur = railsGrid.getCell(trains[0].gridPos.x, trains[0].gridPos.y);

        if (cur.connections[direction]) {
            trains[0].nextDir = dir;
            trains[0].request = true;
        }
    }
}

void Tema2::OnKeyRelease(int key, int mods) {
    // Handle key release events here
}

void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) {
    // Handle mouse movement events here
    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {
        float sensivityOX = 0.001f;
        float sensivityOY = 0.001f;

        if (window->GetSpecialKeyState() == 0) {
            renderCameraTarget = false;
            camera->RotateFirstPerson_OY(-deltaX * sensivityOY);
            camera->RotateFirstPerson_OX(-deltaY * sensivityOX);
        }

        if (window->GetSpecialKeyState() & GLFW_MOD_CONTROL) {
            renderCameraTarget = true;
            camera->RotateThirdPerson_OX(-deltaY * sensivityOX);
            camera->RotateThirdPerson_OY(-deltaX * sensivityOY);
        }
    }
}

void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) {
    // Handle mouse button press events here
}

void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) {
    // Handle mouse button release events here
}

void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) {
    // Handle mouse scroll events here
}

void Tema2::OnWindowResize(int width, int height) {
    // Handle window resize events here
}