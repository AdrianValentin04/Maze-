#pragma once

#include "components/simple_scene.h"
#include "lab_m1/Tema2/lab_camera.h"
#include "lab_m1/Tema2/transform3D.h"


namespace m1
{
    class Tema2 : public gfxc::SimpleScene
    {
     public:
        Tema2();
        ~Tema2();

        void Init() override;

     private:
        void FrameStart() override;
        //void GenerateMaze(int size, int maze[23][23]);
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;
        void GenerateMaze(int maze[20][20], int size, int i, int j);
        int  CountVisited(int maze[20][20], int size, int i, int j);
        void Shuffle(int maze[], int size);
        void RenderMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix) override;
        void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix);
        void UpdateProjectile(float deltaTimeSeconds);
        void CheckPlayerWallsCollision();
        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

     protected:
        implemented::CAM *CAM;
        glm::mat4 projectionMatrix;
        
        bool renderCAMTarget = false;
        bool projectionType = true;
        float r = 10.f;
        float l = 0.01f;
        float bottom = 0.01f;
        float top = 10.f;
        float FoV = 30.f;
        float Z_FAR = 100.f;
        float Z_NEAR = 0.01f;
        bool  goodPosition = false;
        bool  isPressed = false;

        float playerAngle = M_PI_2;
        float newPozX;
        float newPozY;
        float newPozZ;

        float centerPosX;
        float centerPosY;
        float centerPosZ;
        float cameraField = 1.5f;
        bool  TP = true;
        
        float playerX = 0;
        float playerY = 0;
        float playerZ = 0;
        float projectileX = 0;
        float projectileZ = 0;
        float height = 1.f;

        float minXPlayer = 0;
        float maxXPlayer = 0;
        float minZPlayer = 0;
        float maxZPlayer = 0;

        float minXObstacle = 0;
        float maxXObstacle = 0;
        float minZObstacle = 0;
        float maxZObstacle = 0;
        
        float headDimension = 0.25f;
        float projectileDimension = 0.1f;
        bool  trigger = false;
        float projectileSpeed = 3;
        float projectileAngle = playerAngle;
        float projectileDistance = 3;

        float hairHeight = 0.03f;
        float bodyHeight = 0.6f;
        float bodyWidth = 0.5f;
        float bodyY = height - headDimension / 2 - bodyHeight / 2;

        float legWidth = 0.2f;
        float legHeight = 0.25f;
        float legY = height - headDimension / 2 - bodyHeight - legHeight / 2;
        float legX = bodyWidth / 2 - legWidth / 2;

        float armHeight = 0.3f;
        float armWidth = 0.1f;
        float armX = bodyWidth / 2 + armWidth / 2;
        float armY = height - headDimension / 2 - armHeight / 2;

        float handHeight = 0.1f;
        float handWidth = armWidth;
        float handX = bodyWidth / 2 + armWidth / 2;
        float handY = height - headDimension / 2 - armHeight - handHeight / 2;

        float CAMSpeed = 2.0f;
        float mapSize = 40;
        float mapHeight = 0.5f;
        int maze[20][20];
        int size = 20;
        int noCells = 10;
        float wallHeight = 2;
        float miniFloorX = size;
        float miniFloorZ = 5;

        bool collideLeft = false;
        bool collideRight = false;
        bool collideForward = false;
        bool collideBackward = false;


    };
}   // namespace m1
