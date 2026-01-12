#include "lab_m1/tema2/object.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"

void createCube(glm::vec3 pos,
    float w, float h, float d,
    glm::vec3 color,
    std::vector<VertexFormat>& vertices,
    std::vector<unsigned int>& indices) {
        unsigned int start_index = vertices.size();

        vertices.push_back(VertexFormat(glm::vec3(pos.x, pos.y, pos.z), color));
        vertices.push_back(VertexFormat(glm::vec3(pos.x + w, pos.y, pos.z), color));
        vertices.push_back(VertexFormat(glm::vec3(pos.x + w, pos.y + h, pos.z), color));
        vertices.push_back(VertexFormat(glm::vec3(pos.x, pos.y + h, pos.z), color));

        vertices.push_back(VertexFormat(glm::vec3(pos.x, pos.y, pos.z - d), color));
        vertices.push_back(VertexFormat(glm::vec3(pos.x + w, pos.y, pos.z - d), color));
        vertices.push_back(VertexFormat(glm::vec3(pos.x + w, pos.y + h, pos.z - d), color));
        vertices.push_back(VertexFormat(glm::vec3(pos.x, pos.y + h, pos.z - d), color));

        unsigned int cube_indices[] = {
            0,1,2, 0,2,3,
            4,6,5, 4,7,6,
            4,5,1, 4,1,0,
            3,2,6, 3,6,7,
            1,5,6, 1,6,2,
            4,0,3, 4,3,7
        };

        for (unsigned int i : cube_indices) {
            indices.push_back(start_index + i);
        }
    }

void createCylinder1(glm::vec3 pos, 
    float r, float d, float seg,
    glm::vec3 color,
    std::vector<VertexFormat>& vertices,
    std::vector<unsigned int>& indices) {
        unsigned int start_index = vertices.size();

        vertices.push_back(VertexFormat(glm::vec3(pos.x, pos.y, pos.z), color));
        vertices.push_back(VertexFormat(glm::vec3(pos.x, pos.y, pos.z - d), color));

        for (int i = 0; i <= seg; i++) {
            float theta = i * 2.f * M_PI / seg;
            float cx = pos.x + r * cos(theta);
            float cy = pos.y + r * sin(theta);

            vertices.push_back(VertexFormat(glm::vec3(cx, cy, pos.z), color));
            vertices.push_back(VertexFormat(glm::vec3(cx, cy, pos.z - d), color));
        }

        for (int i = 0; i < seg; i++) {
            unsigned int front1 = start_index + 2 + i * 2;
            unsigned int front2 = front1 + 2;
            unsigned int back1 = front1 + 1;
            unsigned int back2 = back1 + 2;

            indices.insert(indices.end(), {
                start_index, front1, front2,
                start_index + 1, back2, back1,
                front1, back1, back2,
                front1, back2, front2
            });
        }
    }

void createPyramid(glm::vec3 pos,
    float w, float h, float d,
    glm::vec3 color,
    std::vector<VertexFormat>& vertices,
    std::vector<unsigned int>& indices) {
        unsigned int start_index = vertices.size();

        vertices.push_back(VertexFormat(glm::vec3(pos.x, pos.y, pos.z), color));
        vertices.push_back(VertexFormat(glm::vec3(pos.x, pos.y + h, pos.z - d * 0.5f), color));
        vertices.push_back(VertexFormat(glm::vec3(pos.x, pos.y, pos.z - d), color));

        vertices.push_back(VertexFormat(glm::vec3(pos.x + w, pos.y, pos.z), color));
        vertices.push_back(VertexFormat(glm::vec3(pos.x + w, pos.y + h, pos.z - d * 0.5f), color));
        vertices.push_back(VertexFormat(glm::vec3(pos.x + w, pos.y, pos.z - d), color));
        unsigned int pyramid_indices[] = {
            0,1,2,
            3,5,4
        };

        for (unsigned int i : pyramid_indices) {
            indices.push_back(start_index + i);
        }
    }

void createCylinder2(glm::vec3 pos,
    float r, float d, float seg,
    glm::vec3 color,
    std::vector<VertexFormat>& vertices,
    std::vector<unsigned int>& indices) {
        unsigned int start_index = vertices.size();

        vertices.push_back(VertexFormat(glm::vec3(pos.x, pos.y, pos.z), color));
        vertices.push_back(VertexFormat(glm::vec3(pos.x - d, pos.y, pos.z), color));

        for (int i = 0; i <= seg; i++) {
            float theta = i * 2.f * M_PI / seg;
            float cy = pos.y + r * cos(theta);
            float cz = pos.z + r * sin(theta);

            vertices.push_back(VertexFormat(glm::vec3(pos.x, cy, cz), color));
            vertices.push_back(VertexFormat(glm::vec3(pos.x - d, cy, cz), color));
        }

        for (int i = 0; i < seg; i++) {
            unsigned int front1 = start_index + 2 + i * 2;
            unsigned int front2 = front1 + 2;
            unsigned int back1 = front1 + 1;
            unsigned int back2 = back1 + 2;

            indices.insert(indices.end(), {
                start_index, front1, front2,
                start_index + 1, back2, back1,
                front1, back1, back2,
                front1, back2, front2
            });
        }
    }

void createCylinder3(glm::vec3 pos,
    float r, float h, float seg,
    glm::vec3 color,
    std::vector<VertexFormat>& vertices,
    std::vector<unsigned int>& indices) {
        unsigned int start_index = vertices.size();

        vertices.push_back(VertexFormat(glm::vec3(pos.x, pos.y, pos.z), color));
        vertices.push_back(VertexFormat(glm::vec3(pos.x, pos.y + h, pos.z), color));

        for (int i = 0; i <= seg; i++) {
            float theta = i * 2.f * M_PI / seg;
            float cx = pos.x + r * cos(theta);
            float cz = pos.z + r * sin(theta);

            vertices.push_back(VertexFormat(glm::vec3(cx, pos.y, cz), color));
            vertices.push_back(VertexFormat(glm::vec3(cx, pos.y + h, cz), color));
        }

        for (int i = 0; i < seg; i++) {
            unsigned int front1 = start_index + 2 + i * 2;
            unsigned int front2 = front1 + 2;
            unsigned int back1 = front1 + 1;
            unsigned int back2 = back1 + 2;

            indices.insert(indices.end(), {
                start_index, front1, front2,
                start_index + 1, back2, back1,
                front1, back1, back2,
                front1, back2, front2
            });
        }        
    }

void createRoof(glm::vec3 pos,
    float w, float h, float d,
    glm::vec3 color,
    std::vector<VertexFormat>& vertices,
    std::vector<unsigned int>& indices) {
        unsigned int start_index = vertices.size();

		vertices.push_back(VertexFormat(glm::vec3(pos.x, pos.y, pos.z), color));
		vertices.push_back(VertexFormat(glm::vec3(pos.x + w, pos.y, pos.z), color));
        vertices.push_back(VertexFormat(glm::vec3(pos.x + w, pos.y + h, pos.z), color));
        vertices.push_back(VertexFormat(glm::vec3(pos.x, pos.y + h, pos.z), color));

        vertices.push_back(VertexFormat(glm::vec3(pos.x, pos.y, pos.z - d), color));
        vertices.push_back(VertexFormat(glm::vec3(pos.x + w, pos.y, pos.z - d), color));
        vertices.push_back(VertexFormat(glm::vec3(pos.x + w, pos.y + h, pos.z - d), color));
        vertices.push_back(VertexFormat(glm::vec3(pos.x, pos.y + h, pos.z - d), color));

        unsigned int roof_indices[] = {
            4,0,3, 3,7,4,
            3,2,6, 3,6,7,
			1,5,6, 1,6,2
		};

        for (unsigned int i : roof_indices) {
            indices.push_back(start_index + i);
		}
    }

void createEgypt(glm::vec3 pos,
    float w, float h, float d,
    glm::vec3 color,
    std::vector<VertexFormat>& vertices,
    std::vector<unsigned int>& indices) {

        unsigned int start_index = vertices.size();
        vertices.push_back(VertexFormat(glm::vec3(pos.x, pos.y, pos.z), color));
        vertices.push_back(VertexFormat(glm::vec3(pos.x + w, pos.y, pos.z), color));
        vertices.push_back(VertexFormat(glm::vec3(pos.x + w, pos.y, pos.z - d), color));
        vertices.push_back(VertexFormat(glm::vec3(pos.x, pos.y, pos.z - d), color));
        vertices.push_back(VertexFormat(glm::vec3(pos.x + w * 0.5f, pos.y + h, pos.z - d * 0.5f), color));

        unsigned int egypt_indices[] = {
            0,1,4,
            1,2,4,
            2,3,4,
            3,0,4,
            3,2,1,
            3,1,0
		};

        for (unsigned int i : egypt_indices) {
            indices.push_back(start_index + i);
		}
}

Mesh* object3D::RenderCube (const std::string& name,
    glm::vec3 position)
{
    glm::vec3 corner = position + glm::vec3(-0.5f, 0.f, 0.5f);

    std::vector<VertexFormat> vertices = {
        VertexFormat(corner, WHITE_COLOR),
		VertexFormat(corner + glm::vec3(1.f, 0.f, 0.f), WHITE_COLOR),
        VertexFormat(corner + glm::vec3(1.f, 1.f, 0.f), WHITE_COLOR),
        VertexFormat(corner + glm::vec3(0.f, 1.f, 0.f), WHITE_COLOR),
        VertexFormat(corner + glm::vec3(0.f, 0.f, -1.f), WHITE_COLOR),
        VertexFormat(corner + glm::vec3(1.f, 0.f, -1.f), WHITE_COLOR),
        VertexFormat(corner + glm::vec3(1.f, 1.f, -1.f), WHITE_COLOR),
        VertexFormat(corner + glm::vec3(0.f, 1.f, -1.f), WHITE_COLOR),
    };

    std::vector<unsigned int> indices = {
        0,1,2, 0,2,3,
        4,6,5, 4,7,6,
        4,5,1, 4,1,0,
        3,2,6, 3,6,7,
        1,5,6, 1,6,2,
        4,0,3, 4,3,7
	};

	Mesh* cube = new Mesh(name);
	cube->InitFromData(vertices, indices);

    return cube;
}

Mesh* object3D::CreateLog(const std::string& name, glm::vec3 position) 
{
	std::vector<VertexFormat> vertices;
	std::vector<unsigned int> indices;
	glm::vec3 center = position;

	createCylinder3(center, 0.1f, 0.5f, 16, BROWN_COLOR, vertices, indices);
	Mesh* log = new Mesh(name);
	log->InitFromData(vertices, indices);

	return log;
}

Mesh* object3D::CreateMountain(const std::string& name, glm::vec3 position)
{
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;
    glm::vec3 corner = position + glm::vec3(-4.f, 0.f, -5.f);

	createEgypt(corner, 8.f, 10.f, 10.f, BROWN_COLOR, vertices, indices);
    Mesh* mountain = new Mesh(name);
    mountain->InitFromData(vertices, indices);
    return mountain;
}

Mesh* object3D::CreatePad(const std::string& name, glm::vec3 position)
{
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;
    glm::vec3 corner = position + glm::vec3(-0.5f, 0.f, -0.5f);

    createCylinder3(corner, 0.5f, 0.5f, 16, YELLOW_COLOR, vertices, indices);
    Mesh* pad = new Mesh(name);
    pad->InitFromData(vertices, indices);
	return pad;
}

Mesh* object3D::CreateLocomotive
    (const std::string &name,
    glm::vec3 position) 
{
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    glm::vec3 corner = position + glm::vec3(-0.5f, 0.45f, 1.f);
    createCube(corner, 1.f, 0.2f, 2.f, YELLOW_COLOR, vertices, indices);

    glm::vec3 cabinCorner = corner + glm::vec3(0.f, 0.2f, 0.f);
    createCube(cabinCorner, 1.f, 0.8f, 0.8f, GREEN_COLOR, vertices, indices);

    glm::vec3 bodyCenter = cabinCorner + glm::vec3(0.5f, 0.3f, -0.8f);
    createCylinder1(bodyCenter, 0.3f, 1.1f, 16, BLUE_COLOR, vertices, indices);

    glm::vec3 frontCenter = bodyCenter + glm::vec3(0.f, 0.f, -1.1f);
    createCylinder1(frontCenter, 0.1f, 0.1f, 16, PURPLE_COLOR, vertices, indices);

    glm::vec3 firstWheel = corner + glm::vec3(0.2f, -0.2f, -0.2f);
    createCylinder2(firstWheel, 0.2f, 0.2f, 16, RED_COLOR, vertices, indices);
    createCylinder2(firstWheel + glm::vec3(0.f, 0.f, -0.4f), 0.2f, 0.2f, 16, RED_COLOR, vertices, indices);
    createCylinder2(firstWheel + glm::vec3(0.f, 0.f, -0.8f), 0.2f, 0.2f, 16, RED_COLOR, vertices, indices);
    createCylinder2(firstWheel + glm::vec3(0.f, 0.f, -1.2f), 0.2f, 0.2f, 16, RED_COLOR, vertices, indices);
    createCylinder2(firstWheel + glm::vec3(0.f, 0.f, -1.6f), 0.2f, 0.2f, 16, RED_COLOR, vertices, indices);
    glm::vec3 otherSideWheel = firstWheel + glm::vec3(0.8f, 0.f, 0.f);

    createCylinder2(otherSideWheel, 0.2f, 0.2f, 16, RED_COLOR, vertices, indices);
    createCylinder2(otherSideWheel + glm::vec3(0.f, 0.f, -0.4f), 0.2f, 0.2f, 16, RED_COLOR, vertices, indices);
    createCylinder2(otherSideWheel + glm::vec3(0.f, 0.f, -0.8f), 0.2f, 0.2f, 16, RED_COLOR, vertices, indices);
    createCylinder2(otherSideWheel + glm::vec3(0.f, 0.f, -1.2f), 0.2f, 0.2f, 16, RED_COLOR, vertices, indices);
    createCylinder2(otherSideWheel + glm::vec3(0.f, 0.f, -1.6f), 0.2f, 0.2f, 16, RED_COLOR, vertices, indices);
    Mesh* locomotive = new Mesh(name);

    locomotive->InitFromData(vertices, indices);

    return locomotive;
}

Mesh* object3D::CreateCarriage
    (const std::string &name,
    glm::vec3 position) 
{
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    glm::vec3 corner = position + glm::vec3(-0.5f, 0.45f, 1.f);
    createCube(corner, 1.f, 0.2f, 2.f, YELLOW_COLOR, vertices, indices);

    glm::vec3 cabinCorner = corner + glm::vec3(0.f, 0.2f, 0.f);
    createCube(cabinCorner, 1.f, 0.8f, 2.f, GREEN_COLOR, vertices, indices);

    glm::vec3 firstWheel = corner + glm::vec3(0.2f, -0.2f, -0.2f);
    createCylinder2(firstWheel, 0.2f, 0.2f, 16, RED_COLOR, vertices, indices);
    createCylinder2(firstWheel + glm::vec3(0.f, 0.f, -0.4f), 0.2f, 0.2f, 16, RED_COLOR, vertices, indices);
    createCylinder2(firstWheel + glm::vec3(0.f, 0.f, -0.8f), 0.2f, 0.2f, 16, RED_COLOR, vertices, indices);
    createCylinder2(firstWheel + glm::vec3(0.f, 0.f, -1.2f), 0.2f, 0.2f, 16, RED_COLOR, vertices, indices);
    createCylinder2(firstWheel + glm::vec3(0.f, 0.f, -1.6f), 0.2f, 0.2f, 16, RED_COLOR, vertices, indices);

    glm::vec3 otherSideWheel = firstWheel + glm::vec3(0.8f, 0.f, 0.f);
    createCylinder2(otherSideWheel, 0.2f, 0.2f, 16, RED_COLOR, vertices, indices);
    createCylinder2(otherSideWheel + glm::vec3(0.f, 0.f, -0.4f), 0.2f, 0.2f, 16, RED_COLOR, vertices, indices);
    createCylinder2(otherSideWheel + glm::vec3(0.f, 0.f, -0.8f), 0.2f, 0.2f, 16, RED_COLOR, vertices, indices);
    createCylinder2(otherSideWheel + glm::vec3(0.f, 0.f, -1.2f), 0.2f, 0.2f, 16, RED_COLOR, vertices, indices);
    createCylinder2(otherSideWheel + glm::vec3(0.f, 0.f, -1.6f), 0.2f, 0.2f, 16, RED_COLOR, vertices, indices);
    Mesh* carriage = new Mesh(name);

    carriage->InitFromData(vertices, indices);

    return carriage;
}

Mesh* object3D::CreateRail
    (const std::string &name,
    glm::vec3 position) 
{
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    glm::vec3 corner = position + glm::vec3(-0.5f, 0.f, 0.5f);
    createCube(corner, 0.2f, 0.04f, 1.f, GRAY_COLOR, vertices, indices);
    createCube(corner + glm::vec3(0.8f, 0.f, 0.f), 0.2f, 0.04f, 1.f, GRAY_COLOR, vertices, indices);

    Mesh* rail = new Mesh(name);

    rail->InitFromData(vertices, indices);

    return rail;
}

Mesh* object3D::CreateTunnelRail(const std::string& name, glm::vec3 position)
{
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    glm::vec3 corner = position + glm::vec3(-0.5f, 0.f, 0.5f);
    createCube(corner, 0.2f, 0.04f, 1.f, GRAY_COLOR, vertices, indices);
    createCube(corner + glm::vec3(0.8f, 0.f, 0.f), 0.2f, 0.04f, 1.f, GRAY_COLOR, vertices, indices);

    glm::vec3 c1 = corner + glm::vec3(-0.1f, 0.f, 0.f);
	createRoof(c1, 1.2f, 2.4f, 1.f, DARK_GRAY_COLOR, vertices, indices);

	Mesh* rail = new Mesh(name);
	rail->InitFromData(vertices, indices);

	return rail;
}

Mesh* object3D::CreateBridgeRail
    (const std::string &name, 
        glm::vec3 position) 
{
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    glm::vec3 corner = position + glm::vec3(-0.5f, 0.f, 0.5f);

    createCube(corner, 0.2f, 0.04f, 5.f, GRAY_COLOR, vertices, indices);
    createCube(corner + glm::vec3(0.8f, 0.f, 0.f), 0.2f, 0.04f, 5.f, GRAY_COLOR, vertices, indices);
    createPyramid(corner + glm::vec3(-0.02f, 0.f, 0.f), 1.04f, 1.3f, 5.f, glm::vec3(0.03f, 0.03f, 0.2f), vertices, indices);

    Mesh* rail = new Mesh(name);

    rail->InitFromData(vertices, indices);

    return rail;
}

Mesh* object3D::CreateTerrain
    (const std::string &name,
    glm::vec3 position) 
{
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    glm::vec3 corner = position;

    createCube(corner, 50.f, 2.f, 50.f, glm::vec3(0.6f, 0.4f, 0.2f), vertices, indices);
    createCube(corner + glm::vec3(0.f, 2.f, 0.f), 50.f, 1.f, 50.f, glm::vec3(0.3f, 0.8f, 0.3f), vertices, indices);
    createCube(corner + glm::vec3(0.f, 3.f, 0.f), 50.f, 1.f, 50.f, glm::vec3(0.1f, 0.9f, 0.1f), vertices, indices);

    Mesh* terrain = new Mesh(name);

    terrain->InitFromData(vertices, indices);

    return terrain;
}

void makeSphere(float radius, 
    int sectorCount, 
    int stackCount,
    glm::vec3 center,
    glm::vec3 color,
    std::vector<VertexFormat>& vertices,
    std::vector<unsigned int>& indices) 
{
    for (int i = 0; i <= stackCount; ++i) {
        float stackAngle = M_PI / 2 - i * M_PI / stackCount;
        float xy = center.x + radius * cosf(stackAngle);
        float z = center.z + radius * sinf(stackAngle);

        for (int j = 0; j <= sectorCount; ++j) {
            float sectorAngle = j * 2 * M_PI / sectorCount;
            float x = center.x + xy * cosf(sectorAngle);
            float y = center.y + xy * sinf(sectorAngle);
            vertices.push_back(VertexFormat(glm::vec3(x, y, z), color));
        }
    }

    for (int i = 0; i < stackCount; ++i) {
        int k1 = i * (sectorCount + 1);
        int k2 = k1 + sectorCount + 1;

        for (int j = 0; j < sectorCount; ++j, ++k1, ++k2) {
            if (i != 0) {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }

            if (i != (stackCount - 1)) {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }
}

Mesh* object3D::CreateSphere
    (const std::string &name,
    glm::vec3 position) 
{
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    glm::vec3 corner = position;

    makeSphere(1.f, 20, 20, corner, WHITE_COLOR, vertices, indices);

    Mesh* sphere = new Mesh(name);

    sphere->InitFromData(vertices, indices);

    return sphere;
}

Mesh* object3D::CreateStation1
    (const std::string &name,
    glm::vec3 position)
{
    glm::vec3 corner = position + glm::vec3(-1.5f, 0.f, 1.5f);

    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    createCube(corner, 3.f, 2.f, 3.f, glm::vec3(0.5f, 0.5f, 0.5f), vertices, indices);
    createCube(corner + glm::vec3(-0.2f, 0.f, 0.2f), 0.4f, 3.f, 0.4f, DARK_GRAY_COLOR, vertices, indices);
    createCube(corner + glm::vec3(3.f, 0.f, 0.2f), 0.4f, 3.f, 0.4f, DARK_GRAY_COLOR, vertices, indices);
    createCube(corner + glm::vec3(-0.2f, 0.f, -3.f), 0.4f, 3.f, 0.4f, DARK_GRAY_COLOR, vertices, indices);
    createCube(corner + glm::vec3(3.f, 0.f, -3.f), 0.4f, 3.f, 0.4f, DARK_GRAY_COLOR, vertices, indices);

    Mesh* station = new Mesh(name);
    station->InitFromData(vertices, indices);

    return station;
}

Mesh* object3D::CreateStation2
    (const std::string &name,
    glm::vec3 position)
{
    glm::vec3 corner = position + glm::vec3(-1.2f, 0.f, 1.2f);

    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    createCube(corner, 2.4f, 1.3f, 2.4f, glm::vec3(0.8f, 0.5f, 0.2f), vertices, indices);
    createCylinder3(corner, 0.2f, 2.f, 20, glm::vec3(0.6f, 0.3f, 0.1f), vertices, indices);
    createCylinder3(corner + glm::vec3(2.4f, 0.f, 0.f), 0.2f, 2.f, 20, glm::vec3(0.6f, 0.3f, 0.1f), vertices, indices);
    createCylinder3(corner + glm::vec3(0.f, 0.f, -2.4f), 0.2f, 2.f, 20, glm::vec3(0.6f, 0.3f, 0.1f), vertices, indices);
    createCylinder3(corner + glm::vec3(2.4f, 0.f, -2.4f), 0.2f, 2.f, 20, glm::vec3(0.6f, 0.3f, 0.1f), vertices, indices);

    Mesh* station = new Mesh(name);
    station->InitFromData(vertices, indices);

    return station;
}

Mesh* object3D::CreateStation3
    (const std::string &name, 
    glm::vec3 position) {

    glm::vec3 corner = position + glm::vec3(-1.1f, 0.f, 1.1f);
    glm::vec3 center = position + glm::vec3(0.f, 0.f, 0.f);
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    createCylinder3(center, 1.2f, 2.f, 20, PURPLE_COLOR, vertices, indices);
    createCylinder3(corner, 0.2f, 3.f, 20, DARK_GRAY_COLOR, vertices, indices);
    createCylinder3(corner + glm::vec3(2.2f, 0.f, 0.f), 0.2f, 3.f, 20, DARK_GRAY_COLOR, vertices, indices);
    createCylinder3(corner + glm::vec3(0.f, 0.f, -2.2f), 0.2f, 3.f, 20, DARK_GRAY_COLOR, vertices, indices);
    createCylinder3(corner + glm::vec3(2.2f, 0.f, -2.2f), 0.2f, 3.f, 20, DARK_GRAY_COLOR, vertices, indices);

    Mesh* station = new Mesh(name);
    station->InitFromData(vertices, indices);

    return station;
}

Mesh* object3D::CreateStation4
    (const std::string &name, 
    glm::vec3 position) {

    glm::vec3 corner = position + glm::vec3(-1.5f, 0.f, 1.5f);
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    createEgypt(corner, 3.f, 3.f, 3.f, ORANGE_COLOR, vertices, indices);
    createCube(corner, 3.f, 0.4f, 3.f, WHITE_COLOR, vertices, indices);
    createCube(corner, 0.4f, 2.f, 0.4f, WHITE_COLOR, vertices, indices);
    createCube(corner + glm::vec3(2.6f, 0.f, 0.f), 0.4f, 2.f, 0.4f, WHITE_COLOR, vertices, indices);
    createCube(corner + glm::vec3(0.f, 0.f, -2.6f), 0.4f, 2.f, 0.4f, WHITE_COLOR, vertices, indices);
    createCube(corner + glm::vec3(2.6f, 0.f, -2.6f), 0.4f, 2.f, 0.4f, WHITE_COLOR, vertices, indices);

    Mesh* station = new Mesh(name);
    station->InitFromData(vertices, indices);

    return station;
}