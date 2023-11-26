#include "raylib.h"

int main(){
    // window dimensions
    const int windowWidth{512};
    const int windowHeight{380};

    InitWindow(windowWidth, windowHeight, "Dapper Dasher");

    // acceleration due to gravity (pixel/s)/s
    const int gravity{1'500};

    // player texture
    Texture2D playerText = LoadTexture("textures/player-run.png");
    int textureLength{6};
    Rectangle textureSourceRect {0, 0, playerText.width / textureLength, playerText.height};
    Rectangle playerSpriteScaled;
    playerSpriteScaled.width = textureSourceRect.width * 3;
    playerSpriteScaled.height = textureSourceRect.height * 3;
    playerSpriteScaled.x = windowWidth/2 - playerSpriteScaled.width/2;
    playerSpriteScaled.y = windowHeight - playerSpriteScaled.height;
    Vector2 worldPivotPoint;
    worldPivotPoint.x = 0;
    worldPivotPoint.y = 0;

    // player variables
    int velocity{0};
    bool isGrounded{true};
    const int jumpVelocity{-750};

    // enemy texture
    Texture2D enemyTexture = LoadTexture("textures/fly-eye.png");
    int enemyTextureLength{4};
    Rectangle enemyTextSourceRect{0, 0, enemyTexture.width/enemyTextureLength, enemyTexture.height};
    Rectangle enemySpriteScaled{windowWidth, windowHeight - enemyTextSourceRect.height * 3, enemyTextSourceRect.width * 3, enemyTextSourceRect.height * 3};

    // enemy variables
    int enemyVelocity{-250};

    // anim
    int textureFrame{0};
    int enemyTextureFrame{0};
    const float updateTime{1.0f / 12.0f}; // amount of time before we update the animation frame
    float runningTime{0.0f};
    
    
    SetTargetFPS(60);
    while(!WindowShouldClose()){
        // time delta
        float dT{GetFrameTime()};
        runningTime += dT;

        // start drawing
        BeginDrawing();
        ClearBackground(WHITE);


        // ground check
        if(playerSpriteScaled.y + playerSpriteScaled.height <= windowHeight){
            // applying gravity
            velocity += gravity * dT;
        } else {
            velocity = 0;
            playerSpriteScaled.y = windowHeight - playerSpriteScaled.height;
            isGrounded = true;
        }

        // Jump
        if(IsKeyPressed(KEY_SPACE) && isGrounded){
            velocity += jumpVelocity;
            isGrounded = false;
        }

        // update player position
        playerSpriteScaled.y += velocity * dT;

        // update enemy position
        enemySpriteScaled.x += enemyVelocity * dT;

        // update animation
        if(runningTime >= updateTime){
            // update player frame
            if(isGrounded){
                textureSourceRect.x = textureFrame * textureSourceRect.width;
                textureFrame = (textureFrame + 1) % textureLength;
            }

            // update enemy frame
            enemyTextSourceRect.x = enemyTextureFrame * enemyTextSourceRect.width;
            enemyTextureFrame = (enemyTextureFrame + 1) % enemyTextureLength;

            // reset time to update
            runningTime = 0.0f;
        }

        // draw player
        DrawTexturePro(playerText, textureSourceRect, playerSpriteScaled, worldPivotPoint, 0, WHITE);

        // draw enemy
        DrawTexturePro(enemyTexture, enemyTextSourceRect, enemySpriteScaled, worldPivotPoint, 0, WHITE);

        // stop drawing
        EndDrawing();
    }
    UnloadTexture(playerText);
    UnloadTexture(enemyTexture);
    CloseWindow();
}
