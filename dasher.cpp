#include "raylib.h"

int main(){
    // window dimensions
    const int windowWidth{512};
    const int windowHeight{380};

    InitWindow(windowWidth, windowHeight, "Dapper Dasher");

    // acceleration due to gravity (pixel/s)/s
    const int gravity{1'500};

    // texture
    Texture2D playerText = LoadTexture("textures/player-run.png");
    int textureLength{6};
    Rectangle textureSourceRect;
    textureSourceRect.width = playerText.width / textureLength;
    textureSourceRect.height = playerText.height;
    textureSourceRect.x = 0;
    textureSourceRect.y = 0;
    Rectangle playerSpriteScaled;
    playerSpriteScaled.width = textureSourceRect.width * 3;
    playerSpriteScaled.height = textureSourceRect.height * 3;
    playerSpriteScaled.x = windowWidth/2 - playerSpriteScaled.width/2;
    playerSpriteScaled.y = windowHeight - playerSpriteScaled.height;
    Vector2 worldPivotPoint;
    worldPivotPoint.x = 0;
    worldPivotPoint.y = 0;

    // anim
    int textureFrame{0};
    const float updateTime{1.0f / 12.0f}; // amount of time before we update the animation frame
    float runningTime{0.0f};

    int velocity{0};

    bool isGrounded{true};
    const int jumpVelocity{-750};
    
    
    SetTargetFPS(60);
    while(!WindowShouldClose()){
        // time delta
        float dT{GetFrameTime()};
        runningTime += dT;

        // start drawing
        BeginDrawing();
        ClearBackground(WHITE);


        // ground check
        if(playerSpriteScaled.y + playerSpriteScaled.height <= windowHeight - 3){
            // applying gravity
            velocity += gravity * dT;
        } else {
            velocity = 0;
            isGrounded = true;
        }



        // Jump
        if(IsKeyPressed(KEY_SPACE) && isGrounded){
            velocity += jumpVelocity;
            isGrounded = false;
        }

        // update position
        playerSpriteScaled.y += velocity * dT;

        // update animation
        if(runningTime >= updateTime){
            textureSourceRect.x = textureFrame * textureSourceRect.width;
            textureFrame = (textureFrame + 1) % textureLength;
            runningTime = 0.0f;
        }

        DrawTexturePro(playerText, textureSourceRect, playerSpriteScaled, worldPivotPoint, 0, WHITE);
        DrawCircle(playerSpriteScaled.x, playerSpriteScaled.y, 5, RED);

        // stop drawing
        EndDrawing();
    }
    UnloadTexture(playerText);
    CloseWindow();
}
