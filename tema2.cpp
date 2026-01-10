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

    firstAvailableDirection = -1;

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

        Mesh* carriage1 = object3D::CreateCarriage("carriage1", glm::vec3(0.f, 0.f, 0.f));
        AddMeshToList(carriage1);

        Mesh* locomotive = object3D::CreateLocomotive("locomotive", glm::vec3(0.f, 0.f, 0.f));
        AddMeshToList(locomotive);

        Mesh* station1 = object3D::CreateStation1("station1", glm::vec3(0.f, 0.f, 0.f));
        AddMeshToList(station1);

        Mesh* station2 = object3D::CreateStation2("station2", glm::vec3(0.f, 0.f, 0.f));
        AddMeshToList(station2);
    }

    {
        gridWidth = 50;
        gridHeight = 50;
        railsGrid = RailGrid(gridWidth, gridHeight);
        
        railsGrid.createBridge({25, 27}, Direction::NORTH);

        for (int y = 26; y > 8; y--) {
            railsGrid.createSimpleRail(25, y, false);
        }

        for (int x = 27; x < 40; x++) {
            railsGrid.createSimpleRail(x, 8, true);
        }

        for (int x = 25; x > 8; x--) {
            railsGrid.createSimpleRail(x, 8, true);
        }

        railsGrid.linkRails();
    }

    direction = 0;

    {
        Train train;
        Train carriage;

        train.currentRail = railsGrid.cell(27, 8).rail;
        train.type = TrainType::TRAIN;
        train.trainDir = Direction::EAST;
        train.progress = 0.f;
        train.speed = 2.f;
        trains.push_back(train);

        // carriage.currentRail = railsGrid.cell(4, 10).rail;
        // carriage.type = TrainType::CARRIAGE;
        // carriage.trainDir = Direction::EAST;
        // carriage.progress = -2.1f;
        // carriage.speed = 0.5f;
        // trains.push_back(carriage);
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
    // Per-frame update code here

    glm::mat4 modelMatrix;
    glm::mat4 aux_mat;

    {
        // queue<Rail*> railsToRender;
        // railsToRender.push(railRoad);
        
        // for (Rail* r : rails) {
        //     modelMatrix = glm::mat4(1);

        //     int currentDirection = r->direction;

        //     switch (r->type) {
        //         case RAIL:
        //             for (float i = 0; i < r->length; i++) {
        //                 switch (currentDirection) {
        //                     case NORTH:
        //                         aux_mat = glm::translate(modelMatrix, r->startPos - glm::vec3(0.f, 0.f, i));
        //                         aux_mat = glm::rotate(aux_mat, glm::radians(0.0f), glm::vec3(0.f, 1.f, 0.f));
        //                         break;
        //                     case EAST:
        //                         aux_mat = glm::translate(modelMatrix, r->startPos + glm::vec3(i, 0.f, 0.f));
        //                         aux_mat = glm::rotate(aux_mat, glm::radians(-90.0f), glm::vec3(0.f, 1.f, 0.f));
        //                         break;
        //                     case SOUTH:
        //                         aux_mat = glm::translate(modelMatrix, r->startPos + glm::vec3(0.f, 0.f, i));
        //                         aux_mat = glm::rotate(aux_mat, glm::radians(180.0f), glm::vec3(0.f, 1.f, 0.f));
        //                         break;
        //                     case WEST:
        //                         aux_mat = glm::translate(modelMatrix, r->startPos - glm::vec3(i, 0.f, 0.f));
        //                         aux_mat = glm::rotate(aux_mat, glm::radians(90.0f), glm::vec3(0.f, 1.f, 0.f));
        //                         break;
        //                 }
        //             RenderMesh(meshes["rail"], shaders["VC"], aux_mat);
        //             }
        //             break;

        //         case BRIDGE_RAIL:
        //             aux_mat = glm::translate(modelMatrix, r->startPos);

        //             switch (currentDirection) {
        //                 case NORTH:
        //                     aux_mat = glm::rotate(aux_mat, glm::radians(0.0f), glm::vec3(0.f, 1.f, 0.f));
        //                     break;
        //                 case EAST:
        //                     aux_mat = glm::rotate(aux_mat, glm::radians(-90.0f), glm::vec3(0.f, 1.f, 0.f));
        //                     break;
        //                 case SOUTH:
        //                     aux_mat = glm::rotate(aux_mat, glm::radians(180.0f), glm::vec3(0.f, 1.f, 0.f));
        //                     break;
        //                 case WEST:
        //                     aux_mat = glm::rotate(aux_mat, glm::radians(90.0f), glm::vec3(0.f, 1.f, 0.f));
        //                     break;
        //             }
        //             RenderMesh(meshes["bridgeRail"], shaders["VC"], aux_mat);
        //             break;

        //         case TUNNEL_RAIL:
        //             // Implement tunnel rail rendering if needed
        //             break;
        //     }
        // }
    }

    // Terrain

    {
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(1.f, 0.2f, 0.457f));

        aux_mat = glm::translate(modelMatrix, glm::vec3(-25.f, 0.f, 54.f));
        RenderMesh(meshes["terrain"], shaders["VC"], aux_mat);

        aux_mat = glm::translate(modelMatrix, glm::vec3(-25.f, 0.f, -6.f));
        RenderMesh(meshes["terrain"], shaders["VC"], aux_mat);

        modelMatrix = glm::mat4(1);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f));

        aux_mat = modelMatrix;

        aux_mat = glm::translate(modelMatrix, glm::vec3(-15.f, 4.f, -37.f));
        RenderMesh(meshes["station1"], shaders["VC"], aux_mat);

        aux_mat = glm::translate(modelMatrix, glm::vec3(35.f, 4.f, 27.f));
        RenderMesh(meshes["station2"], shaders["VC"], aux_mat);

        modelMatrix = glm::mat4(1);
        modelMatrix = glm::rotate(modelMatrix, glm::radians(90.0f), glm::vec3(0.f, 1.f, 0.f));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(4.6f, 0.1f, 50));
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0.1f, 0.5f, 0.f));
        RenderMesh(meshes["water"], shaders["VC"], modelMatrix, mapTextures["water_m"]);
    }

    {
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(1.f, 3.f, 4.f));
        RenderMesh(meshes["sphere"], shaders["VC"], modelMatrix);
    }

    if (renderCameraTarget)
    {
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
        RenderMesh(meshes["sphere"], shaders["VC"], modelMatrix);
    }

    UpdateTrain(trains[0], deltaTimeSeconds);
    DrawTrain(trains[0]);

    RenderRails(railsGrid);

    MinimapRender();
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

RailSegment* Tema2::bridgeExit(RailSegment* bridgeRoot, Direction dir) {
    RailSegment* r = bridgeRoot;

    while (r->nextSegments[(int)dir] &&
           r->nextSegments[(int)dir]->type == RailType::BRIDGE_RAIL)
    {
        r = r->nextSegments[(int)dir];
    }

    return r;
}

void Tema2::UpdateTrain(Train& train, float deltaTime) {
    train.progress += train.speed * deltaTime;

    while (train.progress >= 1.0f) {
        train.progress -= 1.0f;

        RailSegment* next = train.currentRail->nextSegments[(int)train.trainDir];
        if (!next) {
            // end of track
            train.speed = 0.0f;
            return;
        }
        
        train.currentRail = next;
    }
}

glm::vec3 Tema2::getRailPos(const Cell& cell) {
    return gridToWorld(cell.position);
}

float Tema2::getRailYaw(const RailSegment& rail) {
    bool horizontal = rail.connections[(int)Direction::EAST] && rail.connections[(int)Direction::WEST];

    return horizontal ? glm::radians(90.0f) : glm::radians(0.0f);
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

glm::vec3 Tema2::getTrainPos(const Train& train) {
    glm::vec3 base = gridToWorld(train.currentRail->cell);

    glm::vec3 dir  = directionToWorld(train.trainDir);

    return {
        base.x + dir.x * train.progress * CELL_SIZE,
        base.y + 0.45f,
        base.z + dir.z * train.progress * CELL_SIZE
    };
}

RenderTransform Tema2::buildRailTransform(const Cell& cell) {
    const RailSegment& rail = *cell.rail;

    glm::vec3 pos = gridToWorld(cell.position);

    RenderTransform t;
    t.pos = pos;
    t.yaw = getRailYaw(rail);

    return t;
}

glm::mat4 Tema2::buildModelMatrix(const RenderTransform& rt) {
    glm::mat4 model(1.0f);
    model = glm::translate(model, glm::vec3(rt.pos.x, rt.pos.y, rt.pos.z));
    model = glm::rotate(model, rt.yaw, glm::vec3(0,1,0));
    return model;
}

void Tema2::DrawTrain(const Train& train) {
    glm::vec3 pos = getTrainPos(train);

    glm::mat4 modelMatrix(1.0f);
    modelMatrix = glm::translate(modelMatrix, pos);
    modelMatrix = glm::rotate(modelMatrix, getRailYaw(*train.currentRail), glm::vec3(0,1,0));

    switch (train.type) {
        case TRAIN:
            RenderMesh(meshes["locomotive"], shaders["VC"], modelMatrix);
            break;
        case CARRIAGE:
            RenderMesh(meshes["carriage1"], shaders["VC"], modelMatrix);
            break;
    }
}

void Tema2::RenderRails(const RailGrid& grid) {
    for (int x = 0; x < gridWidth; x++) {
        for (int y = 0; y < gridHeight; y++) {
            const Cell& cell = grid.cell(x, y);
            
            if (!cell.rail) {
                continue;
            }

            RenderTransform rt = buildRailTransform(cell);
            glm::mat4 modelMatrix = buildModelMatrix(rt);
            switch (cell.rail->type) {
                case RailType::RAIL:
                    RenderMesh(meshes["rail"], shaders["VC"], modelMatrix);
                    break;
                case RailType::BRIDGE_RAIL:
                    RenderMesh(meshes["bridgeRail"], shaders["VC"], modelMatrix);
                    break;
                case RailType::TUNNEL_RAIL:
                    // Implement tunnel rail rendering if needed
                    break;
            }
        }
    }
}

void Tema2::RenderMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix, Texture2D *texture) {
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

    if (meshes["water"] == mesh) {
        GLint isWater = glGetUniformLocation(shader->program, "isWater");
        glUniform1i(isWater, 1);

        float time = Engine::GetElapsedTime();
        GLint timeLocation = glGetUniformLocation(shader->program, "Time");
        glUniform1f(timeLocation, time);

    } else {
        GLint isWater = glGetUniformLocation(shader->program, "isWater");
        glUniform1i(isWater, 0);
    }

    GLint useTexture = glGetUniformLocation(shader->program, "useTexture");

    if (texture)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture->GetTextureID());
        glUniform1i(glGetUniformLocation(shader->program, "texture1"), 0);
        glUniform1i(useTexture, 1);
    } else {
        glUniform1i(useTexture, 0);
    }

    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}

void Tema2::RenderMeshMini(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix, Texture2D *texture) {
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

    } else {
        GLint isWater = glGetUniformLocation(shader->program, "isWater");
        glUniform1i(isWater, 0);
    }

    GLint useTexture = glGetUniformLocation(shader->program, "useTexture");

    if (texture)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture->GetTextureID());
        glUniform1i(glGetUniformLocation(shader->program, "texture1"), 0);
        glUniform1i(useTexture, 1);
    } else {
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
        // for (int i = 0; i < trains.size(); i++) {
        //     Train &train = trains[i];

        //     modelMatrix = glm::mat4(1);
        //     aux_mat = glm::translate(modelMatrix, train.position);

        //     switch (train.currentRail->direction) {
        //         case NORTH:
        //             aux_mat = glm::rotate(aux_mat, glm::radians(0.0f), glm::vec3(0.f, 1.f, 0.f));
        //             break;
        //         case EAST:
        //             aux_mat = glm::rotate(aux_mat, glm::radians(-90.0f), glm::vec3(0.f, 1.f, 0.f));
        //             break;
        //         case SOUTH:
        //             aux_mat = glm::rotate(aux_mat, glm::radians(180.0f), glm::vec3(0.f, 1.f, 0.f));
        //             break;
        //         case WEST:
        //             aux_mat = glm::rotate(aux_mat, glm::radians(90.0f), glm::vec3(0.f, 1.f, 0.f));
        //             break;
        //     }

        //     switch (train.type) {
        //         case TRAIN:
        //             RenderMeshMini(meshes["locomotive"], shaders["VC"], aux_mat);
        //             break;
        //         case CARRIAGE:
        //             RenderMeshMini(meshes["carriage1"], shaders["VC"], aux_mat);
        //             break;
        //     }
        // }
    }

    {
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(1.f, 0.2f, 0.457f));

        aux_mat = glm::translate(modelMatrix, glm::vec3(-25.f, 0.f, 54.f));
        RenderMeshMini(meshes["terrain"], shaders["VC"], aux_mat);

        aux_mat = glm::translate(modelMatrix, glm::vec3(-25.f, 0.f, -6.f));
        RenderMeshMini(meshes["terrain"], shaders["VC"], aux_mat);
    }

    {
        // queue<Rail*> railsToRender;
        // railsToRender.push(railRoad);
        
        // for (Rail* r : rails) {
        //     modelMatrix = glm::mat4(1);

        //     int currentDirection = r->direction;

        //     switch (r->type) {
        //         case RAIL:
        //             for (float i = 0; i < r->length; i++) {
        //                 switch (currentDirection) {
        //                     case NORTH:
        //                         aux_mat = glm::translate(modelMatrix, r->startPos - glm::vec3(0.f, 0.f, i));
        //                         aux_mat = glm::rotate(aux_mat, glm::radians(0.0f), glm::vec3(0.f, 1.f, 0.f));
        //                         break;
        //                     case EAST:
        //                         aux_mat = glm::translate(modelMatrix, r->startPos + glm::vec3(i, 0.f, 0.f));
        //                         aux_mat = glm::rotate(aux_mat, glm::radians(-90.0f), glm::vec3(0.f, 1.f, 0.f));
        //                         break;
        //                     case SOUTH:
        //                         aux_mat = glm::translate(modelMatrix, r->startPos + glm::vec3(0.f, 0.f, i));
        //                         aux_mat = glm::rotate(aux_mat, glm::radians(180.0f), glm::vec3(0.f, 1.f, 0.f));
        //                         break;
        //                     case WEST:
        //                         aux_mat = glm::translate(modelMatrix, r->startPos - glm::vec3(i, 0.f, 0.f));
        //                         aux_mat = glm::rotate(aux_mat, glm::radians(90.0f), glm::vec3(0.f, 1.f, 0.f));
        //                         break;
        //                 }
        //             RenderMeshMini(meshes["rail"], shaders["VC"], aux_mat);
        //             }
        //             break;

        //         case BRIDGE_RAIL:
        //             aux_mat = glm::translate(modelMatrix, r->startPos);

        //             switch (currentDirection) {
        //                 case NORTH:
        //                     aux_mat = glm::rotate(aux_mat, glm::radians(0.0f), glm::vec3(0.f, 1.f, 0.f));
        //                     break;
        //                 case EAST:
        //                     aux_mat = glm::rotate(aux_mat, glm::radians(-90.0f), glm::vec3(0.f, 1.f, 0.f));
        //                     break;
        //                 case SOUTH:
        //                     aux_mat = glm::rotate(aux_mat, glm::radians(180.0f), glm::vec3(0.f, 1.f, 0.f));
        //                     break;
        //                 case WEST:
        //                     aux_mat = glm::rotate(aux_mat, glm::radians(90.0f), glm::vec3(0.f, 1.f, 0.f));
        //                     break;
        //             }
        //             RenderMeshMini(meshes["bridgeRail"], shaders["VC"], aux_mat);
        //             break;

        //         case TUNNEL_RAIL:
        //             // Implement tunnel rail rendering if needed
        //             break;
        //     }
        // }
    }

    {
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f));

        aux_mat = glm::translate(modelMatrix, glm::vec3(-15.f, 4.f, -37.f));
        RenderMeshMini(meshes["station1"], shaders["VC"], aux_mat);

        aux_mat = glm::translate(modelMatrix, glm::vec3(35.f, 4.f, 27.f));
        RenderMeshMini(meshes["station2"], shaders["VC"], aux_mat);

        modelMatrix = glm::mat4(1);
        modelMatrix = glm::rotate(modelMatrix, glm::radians(90.0f), glm::vec3(0.f, 1.f, 0.f));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(4.6f, 0.1f, 50));
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0.1f, 0.5f, 0.0f));
        RenderMeshMini(meshes["water"], shaders["VC"], modelMatrix, mapTextures["water_m"]);
    }

    {
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(1.f, 3.f, 4.f));
        RenderMeshMini(meshes["sphere"], shaders["VC"], modelMatrix);
    }

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

    if (key == GLFW_KEY_W) {
        direction = directionToInt(Direction::NORTH);
    }

    if (key == GLFW_KEY_D) {
        direction = directionToInt(Direction::EAST);
    }

    if (key == GLFW_KEY_A) {
        direction = directionToInt(Direction::WEST);
    }

    if (key == GLFW_KEY_S) {
        direction = directionToInt(Direction::SOUTH);
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