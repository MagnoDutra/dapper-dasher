#include "raylib.h"

struct AnimData
{
    Rectangle imageSourceRect;
    Rectangle imageRect;
    int frame;
    float updateTime;
    float runningTime;
    int textureFrameCount;
};


int main(){
    // window dimensions
    const int windowWidth{512};
    const int windowHeight{380};

    InitWindow(windowWidth, windowHeight, "Dapper Dasher");

    // acceleration due to gravity (pixel/s)/s
    const int gravity{1'500};

    // player texture
    Texture2D playerText = LoadTexture("textures/player-run.png");

    // Player data
    AnimData playerData{ 
        {0, 0, playerText.width / 6, playerText.height}, 
        {windowWidth/2 - playerText.width/2, windowHeight - playerText.height, playerText.width/6 * 3, playerText.height * 3}, 
        0, 
        1.0f / 12.0f, 
        0.0, 
        6
    };    

    Vector2 worldPivotPoint;
    worldPivotPoint.x = 0;
    worldPivotPoint.y = 0;

    // player variables
    int velocity{0};
    bool isGrounded{true};
    const int jumpVelocity{-750};

    // enemy texture
    Texture2D enemyTexture = LoadTexture("textures/fly-eye.png");

    // Enemy data
    AnimData enemyData{ 
        {0, 0, enemyTexture.width/4, enemyTexture.height},                                                      // imageSourceRect -> Size of the rect in the sprite png
        {windowWidth, windowHeight - enemyTexture.height * 3, enemyTexture.width/4 * 3, enemyTexture.height * 3}, // imageRect -> final size after scaling and position
        0,                                                                                                      // frame -> current frame of the animation
        1.0f / 12.0f,                                                                                           // updatetime -> number of anim update per second 
        0.0,                                                                                                    // runningTime -> time elapsed for the current animation
        4                                                                                                       // textureFrameCount -> numbers of frames in the spritsheet
    };


    // enemy variables
    int enemyVelocity{-250};

    
    
    SetTargetFPS(60);
    while(!WindowShouldClose()){
        // time delta
        float dT{GetFrameTime()};
        playerData.runningTime += dT;
        enemyData.runningTime += dT;


        // start drawing
        BeginDrawing();
        ClearBackground(WHITE);


        // ground check
        if(playerData.imageRect.y + playerData.imageRect.height <= windowHeight){
            // applying gravity
            velocity += gravity * dT;
        } else {
            velocity = 0;
            playerData.imageRect.y = windowHeight - playerData.imageRect.height;
            isGrounded = true;
        }

        // Jump
        if(IsKeyPressed(KEY_SPACE) && isGrounded){
            velocity += jumpVelocity;
            isGrounded = false;
        }

        // update player position
        playerData.imageRect.y += velocity * dT;

        // update enemy position
        enemyData.imageRect.x += enemyVelocity * dT;

        // update player animation
        if(playerData.runningTime >= playerData.updateTime){
            if(isGrounded){
                playerData.imageSourceRect.x = playerData.frame * playerData.imageSourceRect.width;
                playerData.frame = (playerData.frame + 1) % playerData.textureFrameCount;
            }

            // reset time to update
            playerData.runningTime = 0.0f;
        }

        // update enemy animation
        if(enemyData.runningTime >= enemyData.updateTime){
            enemyData.imageSourceRect.x = enemyData.frame * enemyData.imageSourceRect.width;
            enemyData.frame = (enemyData.frame + 1) % enemyData.textureFrameCount;

            // reset time to update
            enemyData.runningTime = 0.0f;
        }

        // draw player
        DrawTexturePro(playerText, playerData.imageSourceRect, playerData.imageRect, worldPivotPoint, 0, WHITE);

        // draw enemy
        DrawTexturePro(enemyTexture, enemyData.imageSourceRect, enemyData.imageRect, worldPivotPoint, 0, WHITE);

        // stop drawing
        EndDrawing();
    }
    UnloadTexture(playerText);
    UnloadTexture(enemyTexture);
    CloseWindow();
}
