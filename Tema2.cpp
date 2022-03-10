#include "lab_m1/Tema2/Tema2.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema2::Tema2()
{
}


Tema2::~Tema2()
{
}


void Tema2::Init()
{
    renderCAMTarget = false;
    
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            maze[i][j] = 1;
        }
    }
    Tema2::GenerateMaze(maze, size, 0, 0);

    while (!goodPosition)
    {
        int x = 10 + rand() % 5;
        int z = 10 + rand() % 5;

        if (maze[x][z] == 0 && maze[x][z - 1] == 0)
        {
            goodPosition = true;
            playerX = x + 0.5f;
            playerZ = -z - 0.5f;
        }
    }

    CAM = new implemented::CAM();
    CAM->Set(glm::vec3(playerX, 1.5f, playerZ + 1.5f), glm::vec3(playerX, 1.5f, playerZ), glm::vec3(0, 1.5f, 0));
    CAM->distanceToTarget = cameraField;


    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);

    {
        Shader* shader = new Shader("Skin");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("Hair");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentShaderHair.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("Pants");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentShaderPants.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("Shirt");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentShaderShirt.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("Floor");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentShaderFloor.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("Walls");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentShaderWalls.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("Projectile");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentShaderProjectile.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("Gate");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentShaderGate.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

}

void Tema2::GenerateMaze(int maze[20][20], int size, int i, int j)
{
    int direct[][2] = { {0,1}, {0,-1}, {-1,0}, {1,0} };
    int visitOrder[] = { 0,1,2,3 };

    if (i < 0 || j < 0 || i >= size || j >= size)
        return;
    
    if (maze[i][j] == 0)
        return;

    if (Tema2::CountVisited(maze, size, i, j) > 1)
        return;

    maze[i][j] = 0;
    Tema2::Shuffle(visitOrder, 4);

    for (int k = 0; k < 4; ++k)
    {
        int nexti = i + direct[visitOrder[k]][0];
        int nextj = j + direct[visitOrder[k]][1];
        Tema2::GenerateMaze(maze, size, nexti, nextj);
    }
}

int Tema2::CountVisited(int maze[20][20], int size, int i, int j)
{
    int direct[][2] = { {1,0}, {-1,0}, {0,1}, {0,-1} };
    int count = 0;

    for (int index = 0; index < 4; index++)
    {
        int nexti = i + direct[index][0];
        int nextj = j + direct[index][1];

        if (nexti < 0 || nexti >= size || nextj < 0 || nextj >= size)
            continue;

        if (maze[nexti][nextj] == 0)
            count++;
        
    }
    return count;
}


void Tema2::Shuffle(int maze[], int size)
{
    for (int i = 0; i < size; ++i)
    {
        int randomIndex = rand() % size;
        int aux = maze[i];
        maze[i] = maze[randomIndex];
        maze[randomIndex] = aux;
       
    }
}

void Tema2::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema2::Update(float deltaTimeSeconds)
{
    // Head
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(playerX, playerY + height, playerZ);
        modelMatrix *= transform3D::Scale(-headDimension, -headDimension, -headDimension);
        modelMatrix *= transform3D::RotateOY(playerAngle);
        RenderMesh(meshes["box"], shaders["Skin"], modelMatrix);
    }

    // Hair
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(playerX, playerY + height  + headDimension / 2 + hairHeight / 2, playerZ);
        modelMatrix *= transform3D::Scale(-headDimension, -hairHeight, -headDimension);
        modelMatrix *= transform3D::RotateOY(playerAngle);
        RenderMesh(meshes["box"], shaders["Hair"], modelMatrix);
    }

    // Projectile
    if(trigger)
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(projectileX, playerY + height - headDimension / 2, projectileZ);
        modelMatrix *= transform3D::Scale(-projectileDimension, -projectileDimension, -projectileDimension);
        modelMatrix *= transform3D::RotateOY(playerAngle);
        RenderMesh(meshes["box"], shaders["Projectile"], modelMatrix);
    }

    Tema2::UpdateProjectile(deltaTimeSeconds);

    // Body
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(playerX, playerY + bodyY, playerZ);
        modelMatrix *= transform3D::RotateOY(playerAngle - M_PI_2);
        modelMatrix *= transform3D::Scale(-bodyWidth, -bodyHeight, -headDimension);
        
        RenderMesh(meshes["box"], shaders["Shirt"], modelMatrix);
    }

    // Legs
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(playerX, playerY, playerZ);
        modelMatrix *= transform3D::RotateOY(playerAngle - M_PI_2);
        modelMatrix *= transform3D::Translate(legX, legY , 0);
        modelMatrix *= transform3D::Scale(-legWidth, -legHeight, -headDimension);
        RenderMesh(meshes["box"], shaders["Pants"], modelMatrix);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(playerX, playerY, playerZ);
        modelMatrix *= transform3D::RotateOY(playerAngle - M_PI_2);
        modelMatrix *= transform3D::Translate(- legX, legY, 0);
        modelMatrix *= transform3D::Scale(-legWidth, -legHeight, -headDimension);
        RenderMesh(meshes["box"], shaders["Pants"], modelMatrix);
    }

    
    // Arms
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(playerX, playerY, playerZ);
        modelMatrix *= transform3D::RotateOY(playerAngle - M_PI_2);
        modelMatrix *= transform3D::Translate(armX, armY, 0);
        modelMatrix *= transform3D::Scale(-armWidth, -armHeight, -headDimension);
        RenderMesh(meshes["box"], shaders["Shirt"], modelMatrix);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(playerX, playerY, playerZ);
        modelMatrix *= transform3D::RotateOY(playerAngle - M_PI_2);
        modelMatrix *= transform3D::Translate(-armX, armY, 0);
        modelMatrix *= transform3D::Scale(-armWidth, -armHeight, -headDimension);
        RenderMesh(meshes["box"], shaders["Shirt"], modelMatrix);
    }

    // Hands
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(playerX, playerY, playerZ);
        modelMatrix *= transform3D::RotateOY(playerAngle - M_PI_2);
        modelMatrix *= transform3D::Translate(handX, handY, 0);
        modelMatrix *= transform3D::Scale(-handWidth, -handHeight, -headDimension);
        RenderMesh(meshes["box"], shaders["Skin"], modelMatrix);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(playerX, playerY, playerZ);
        modelMatrix *= transform3D::RotateOY(playerAngle - M_PI_2);
        modelMatrix *= transform3D::Translate(- handX, handY, 0);
        modelMatrix *= transform3D::Scale(-handWidth, -handHeight, -headDimension);
        RenderMesh(meshes["box"], shaders["Skin"], modelMatrix);
    }

    // Floor
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(mapSize / 4, -mapHeight, -mapSize / 4);
        modelMatrix *= transform3D::Scale(mapSize / 2, 1 , mapSize/ 2);
        RenderMesh(meshes["box"], shaders["Floor"], modelMatrix);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(mapSize / 4, -mapHeight, mapSize / 4);
        modelMatrix *= transform3D::Scale(mapSize / 2, 1, mapSize / 2);
        RenderMesh(meshes["box"], shaders["Floor"], modelMatrix);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(size / 2, -mapHeight, -size - miniFloorZ / 2);
        modelMatrix *= transform3D::Scale(miniFloorX, 1, miniFloorZ);
        RenderMesh(meshes["box"], shaders["Floor"], modelMatrix);
    }



    
    // Walls
    for (int i = 0; i < size; i ++) 
    {
        for (int j = 0; j < size; j++)
        {
            if (maze[i][j] == 1)
            {
                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix *= transform3D::Translate(i + 0.5f , wallHeight / 2 , -j - 0.5f);
                modelMatrix *= transform3D::Scale(1, wallHeight, 1);
                RenderMesh(meshes["box"], shaders["Walls"], modelMatrix);
            }
        }
    }

    // Side Walls
    for (int i = 0; i < size; i++)
    {
        
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(-0.5f, wallHeight / 2, -i - 0.5f);
        modelMatrix *= transform3D::Scale(1, wallHeight, 1);
        RenderMesh(meshes["box"], shaders["Walls"], modelMatrix);

        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(size + 0.5f, wallHeight / 2, -i - 0.5f);
        modelMatrix *= transform3D::Scale(1, wallHeight, 1);
        RenderMesh(meshes["box"], shaders["Walls"], modelMatrix);
    }
   
    // Lobby Castle
    for (int i = 0; i < 16; i++)
    {
        if (i % 3 == 0)
        {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix *= transform3D::Translate(-0.5f, (wallHeight + 1) / 2, i + 0.5f);
            modelMatrix *= transform3D::Scale(1, wallHeight + 1, 1);
            RenderMesh(meshes["box"], shaders["Walls"], modelMatrix);

            modelMatrix = glm::mat4(1);
            modelMatrix *= transform3D::Translate(size + 0.5f, (wallHeight + 1) / 2, i + 0.5f);
            modelMatrix *= transform3D::Scale(1, wallHeight + 1, 1);
            RenderMesh(meshes["box"], shaders["Walls"], modelMatrix);
        }
        else
        {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix *= transform3D::Translate(-0.5f, wallHeight / 2, i + 0.5f);
            modelMatrix *= transform3D::Scale(1, wallHeight, 1);
            RenderMesh(meshes["box"], shaders["Walls"], modelMatrix);

            modelMatrix = glm::mat4(1);
            modelMatrix *= transform3D::Translate(size + 0.5f, wallHeight / 2, i + 0.5f);
            modelMatrix *= transform3D::Scale(1, wallHeight, 1);
            RenderMesh(meshes["box"], shaders["Walls"], modelMatrix);
        }
    }
    
    // Walls
    for (int i = 1; i <= 5; i++)
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(-0.5f, wallHeight / 2, -i - size + 0.5f);
        modelMatrix *= transform3D::Scale(1, wallHeight, 1);
        RenderMesh(meshes["box"], shaders["Walls"], modelMatrix);

        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(size + 0.5f, wallHeight / 2, -i - size + 0.5f);
        modelMatrix *= transform3D::Scale(1, wallHeight, 1);
        RenderMesh(meshes["box"], shaders["Walls"], modelMatrix);

    } 

    // More walls
    for (int i = 0; i < size; i++)
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(i + 0.5f, wallHeight / 2, - 5 - size + 0.5f);
        modelMatrix *= transform3D::Scale(1, wallHeight, 1);
        RenderMesh(meshes["box"], shaders["Walls"], modelMatrix);
    }

    // Again Walls
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(7.5f, (wallHeight + 1) / 2, 15.5f);
        modelMatrix *= transform3D::Scale(1, wallHeight + 1, 1);
        RenderMesh(meshes["box"], shaders["Walls"], modelMatrix);
    }
    
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(12.5f, (wallHeight + 1) / 2, 15.5f);
        modelMatrix *= transform3D::Scale(1, wallHeight + 1, 1);
        RenderMesh(meshes["box"], shaders["Walls"], modelMatrix);
    }
    
    // Exterior walls and gate
    for(int i = 0; i < size; i ++)
    {
        if (i < 7.5f || i > 12.5f)
        {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix *= transform3D::Translate(i + 0.5f, wallHeight / 2, 15.5f);
            modelMatrix *= transform3D::Scale(1, wallHeight, 1);
            RenderMesh(meshes["box"], shaders["Walls"], modelMatrix);
        }
        else
        {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix *= transform3D::Translate(i + 0.5f, wallHeight / 2, 15.5f);
            modelMatrix *= transform3D::Scale(1, wallHeight, 1);
            RenderMesh(meshes["box"], shaders["Gate"], modelMatrix);
        }
    }
}

void Tema2::CheckPlayerWallsCollision()
{
    
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {

            if (maze[i][j] == 1)
            {
                if (i + 0.5f == playerX - bodyWidth / 2 - armWidth)
                {
                    collideLeft = true;
                }
                else if (i - 0.5f == playerX + bodyWidth / 2 + armWidth)
                {
                    collideRight = true;
                }
                else if (-j + 0.5f == playerZ - headDimension)
                {
                    collideForward = true;
                }
                else if (-j - 0.5f == playerZ + headDimension)
                {
                    collideBackward = true;
                }
            }
        }
    }

}


void Tema2::FrameEnd()
{
    DrawCoordinateSystem(CAM->GetViewMatrix(), projectionMatrix);
}


void Tema2::RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix)
{
    if (!mesh || !shader || !shader->program)
        return;

    // Render an object using the specified shader and the specified position
    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(CAM->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    mesh->Render();
}

void Tema2::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    int modelLocation = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    int viewLocation = glGetUniformLocation(shader->program, "View");
    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    int projectionLocation = glGetUniformLocation(shader->program, "Projection");
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));



    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}

void Tema2::UpdateProjectile(float deltaTimeSeconds)
{
    if (trigger && !TP)
    {
        projectileX += projectileSpeed * deltaTimeSeconds * sin(projectileAngle + M_PI_2);
        projectileZ += projectileSpeed * deltaTimeSeconds * cos(projectileAngle + M_PI_2);

        if (projectileX > playerX + projectileDistance || projectileZ > playerZ + projectileDistance
            || projectileX < playerX - projectileDistance || projectileZ < playerZ - projectileDistance)
        {
            trigger = false;
            projectileX = playerX;
            projectileZ = playerZ;
            projectileAngle = playerAngle;
            projectileSpeed = 0;
        }
    }
    else
    {
        projectileX = playerX;
        projectileZ = playerZ;
        projectileAngle = playerAngle;
    }

}


void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    
    Tema2::CheckPlayerWallsCollision();
    
    if (window->KeyHold(GLFW_KEY_W)) 
    {
        if (isPressed)
        {
            CAM->TranslateForward(deltaTime * CAMSpeed);
        }
        else
        {
            CAM->TranslateForward(deltaTime * CAMSpeed);
            playerX += deltaTime * CAMSpeed * cos(playerAngle);
            playerZ -= deltaTime * CAMSpeed * sin(playerAngle);
        }
    }
 
    if (collideForward)
    {
        CAM->TranslateForward(-deltaTime * CAMSpeed);
        playerX -= deltaTime * CAMSpeed * cos(playerAngle);
        playerZ += deltaTime * CAMSpeed * sin(playerAngle); 
        collideForward = false;
    }

    if (window->KeyHold(GLFW_KEY_A)) 
    {
        if (isPressed)
        {
            CAM->TranslateRight(-deltaTime * CAMSpeed);
        }
        else
        {
            CAM->TranslateRight(-deltaTime * CAMSpeed);
            playerX -= deltaTime * CAMSpeed * sin(playerAngle);
            playerZ -= deltaTime * CAMSpeed * cos(playerAngle);
        }
    }

    if (collideLeft)
    {
        CAM->TranslateRight(deltaTime * CAMSpeed);
        playerX += deltaTime * CAMSpeed * sin(playerAngle);
        playerZ += deltaTime * CAMSpeed * cos(playerAngle);
        collideLeft = false;
    }


    if (window->KeyHold(GLFW_KEY_S)) 
    {
        if (isPressed)
        {
            CAM->TranslateForward(-deltaTime * CAMSpeed);
        }
        else
        {
            CAM->TranslateForward(-deltaTime * CAMSpeed);
            playerX -= deltaTime * CAMSpeed * cos(playerAngle);
            playerZ += deltaTime * CAMSpeed * sin(playerAngle);
        }
    }

    if (collideBackward)
    {
        CAM->TranslateForward(deltaTime * CAMSpeed);
        playerX += deltaTime * CAMSpeed * cos(playerAngle);
        playerZ -= deltaTime * CAMSpeed * sin(playerAngle);
        collideBackward = false;
    }

    if (window->KeyHold(GLFW_KEY_D)) 
    {
        if (isPressed)
        {
            CAM->TranslateRight(deltaTime * CAMSpeed);
        }
        else
        {
            CAM->TranslateRight(deltaTime * CAMSpeed);
            playerX += deltaTime * CAMSpeed * sin(playerAngle);
            playerZ += deltaTime * CAMSpeed * cos(playerAngle);
        }
    }

    if (collideRight)
    {
        CAM->TranslateRight(-deltaTime * CAMSpeed);
        playerX -= deltaTime * CAMSpeed * sin(playerAngle);
        playerZ -= deltaTime * CAMSpeed * cos(playerAngle);
        collideRight = false;
    }


    if (window->KeyHold(GLFW_KEY_LEFT)) 
    {
        CAM->RotateThirdPerson_OY(deltaTime);
        playerAngle += deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_RIGHT)) 
    {   
        CAM->RotateThirdPerson_OY(-deltaTime);
        playerAngle -= deltaTime;
    }

}


void Tema2::OnKeyPress(int key, int mods)
{
    // Add key press event
    
    if (key == GLFW_KEY_LEFT_CONTROL)
    {

        if (!TP)
        {
            newPozX = CAM->position[0];
            newPozY = CAM->position[1];
            newPozZ = CAM->position[2];

            newPozX = newPozX + (1.5f + headDimension / 2 + 0.001f) * sin(playerAngle - M_PI_2);
            newPozY = 1.5f;
            newPozZ = newPozZ + (1.5f + headDimension / 2 + 0.001f) * cos(playerAngle - M_PI_2);

            centerPosX = newPozX;
            centerPosY = newPozY;
            centerPosZ = newPozZ;

            centerPosX = centerPosX - 0.001f * sin(playerAngle - M_PI_2);
            centerPosZ = centerPosZ - 0.001f * cos(playerAngle - M_PI_2);
            CAM->distanceToTarget = 1.5f;

            CAM->Set(glm::vec3(newPozX, newPozY, newPozZ), glm::vec3(centerPosX, centerPosY, centerPosZ), glm::vec3(0, 1, 0));
            TP = true;
        }
        else
        {
            newPozX = CAM->position[0];
            newPozY = CAM->position[1];
            newPozZ = CAM->position[2];

            newPozX = newPozX - (1.5f + headDimension / 2 + 0.001f) * sin(playerAngle - M_PI_2);
            newPozY = height - hairHeight - headDimension / 2;
            newPozZ = newPozZ - (1.5f + headDimension / 2 + 0.001f) * cos(playerAngle - M_PI_2);

            CAM->distanceToTarget = -0.001f - headDimension / 2;
            centerPosX = newPozX;
            centerPosY = newPozY;
            centerPosZ = newPozZ;

            centerPosX = centerPosX - 0.001f * sin(playerAngle - M_PI_2);
            centerPosZ = centerPosZ - 0.001f * cos(playerAngle - M_PI_2);

            CAM->Set(glm::vec3(newPozX, newPozY, newPozZ), glm::vec3(centerPosX, centerPosY, centerPosZ), glm::vec3(0, 1, 0));
            TP = false;

        }
    }
    
    if (key == GLFW_KEY_SPACE)
    {
        CAM->Set(glm::vec3(playerX, 1.5f, playerZ + 1.5f), glm::vec3(playerX, 1.5f, playerZ), glm::vec3(0, 1, 0));
    }

    if (key == GLFW_KEY_LEFT_SHIFT)
    {
        renderCAMTarget = true;
    }
    else
    {
        renderCAMTarget = false;
    }

}


void Tema2::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event

    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float sensivityOX = 0.001f;
        float sensivityOY = 0.001f;
        isPressed = true;

        if (window->GetSpecialKeyState() == 0) {
            renderCAMTarget = false;
            CAM->RotateFirstPerson_OY(-sensivityOY * deltaX);
            CAM->RotateFirstPerson_OX(-sensivityOX * deltaY);
        }

        if (window->GetSpecialKeyState() & GLFW_MOD_CONTROL) {
            renderCAMTarget = true;
            CAM->RotateThirdPerson_OY(-sensivityOY * deltaX);
            CAM->RotateThirdPerson_OX(-sensivityOX * deltaY);  
        }
    }
    else
    {
        isPressed = false;
    }
    
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
    if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT) && !TP)
    {
        projectileSpeed += 3;
    }
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
    if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT) && !TP)
    {
        trigger = true;
    }
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema2::OnWindowResize(int width, int height)
{
}


/*
    Nota:
    Pentru labirint am folosit resurse de pe net, asa cum ne-a fost indicat.
*/