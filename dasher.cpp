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

bool isOnAir(AnimData data, int windowHeight){
   return data.imageRect.y + data.imageRect.height <= windowHeight;
}

AnimData updateAnimData(AnimData data){
    data.imageSourceRect.x = data.frame * data.imageSourceRect.width;
    data.frame = (data.frame + 1) % data.textureFrameCount;

    // reset time to update
    data.runningTime = 0.0f;

    return data;
}

int main(){
    // window dimensions
    int windowDimensions[2]{512, 380};
    
    InitWindow(windowDimensions[0], windowDimensions[1], "Dapper Dasher");

    // acceleration due to gravity (pixel/s)/s
    const int gravity{1'500};

    // background image
    Texture2D background = LoadTexture("textures/far-buildings.png");
    Texture2D midground = LoadTexture("textures/back-buildings.png");
    Texture2D foreground = LoadTexture("textures/foreground.png");

    // player texture
    Texture2D playerText = LoadTexture("textures/player-run.png");

    // Player data
    AnimData playerData{ 
        {0, 0, playerText.width / 6, playerText.height}, 
        {windowDimensions[0]/2 - playerText.width/2, windowDimensions[1] - playerText.height, playerText.width/6 * 3, playerText.height * 3}, 
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
    float bgX{0};
    float mgX{0};
    float fgX{0};
    // Enemy data
    const int ENEMY_COUNT = 3;
    AnimData enemys[ENEMY_COUNT]{};

    for (int i = 0; i < ENEMY_COUNT; i++)
    {
        enemys[i].imageSourceRect.x = 0;
        enemys[i].imageSourceRect.y = 0;
        enemys[i].imageSourceRect.width = enemyTexture.width/4;
        enemys[i].imageSourceRect.height = enemyTexture.height;

        enemys[i].imageRect.x = windowDimensions[0] + i * 300;
        enemys[i].imageRect.y = windowDimensions[1] - enemyTexture.height * 3;
        enemys[i].imageRect.width = enemyTexture.width/4 * 3;
        enemys[i].imageRect.height = enemyTexture.height * 3;

        enemys[i].frame = 0;
        enemys[i].textureFrameCount = 4;
        enemys[i].runningTime = 0.0;
        enemys[i].updateTime = 1.0f / 12.0f;
    }

    // enemy variables
    int enemyVelocity{-250};

    bool collision{false};
    float finishLine{ enemys[ENEMY_COUNT-1].imageRect.x };
    
    SetTargetFPS(60);
    while(!WindowShouldClose()){
        // time delta
        float dT{GetFrameTime()};
        playerData.runningTime += dT;

        for (int i = 0; i < ENEMY_COUNT; i++)
        {
            enemys[i].runningTime += dT;
        }

        // start drawing
        BeginDrawing();
        ClearBackground(WHITE);

        bgX -= 20 * dT;
        mgX -= 40 * dT;
        fgX -= 80 * dT;

        if(bgX <= -background.width*2){
            bgX = 0.0;
        }

        if(mgX <= -background.width*2){
            mgX = 0.0;
        }

        if(fgX <= -background.width*2){
            fgX = 0.0;
        }

        // draw background
        Vector2 bg1Pos{bgX, 0.0};
        Vector2 bg2Pos{bgX + background.width * 2, 0.0};
        Vector2 mg1Pos{mgX, 0.0};
        Vector2 mg2Pos{mgX + background.width * 2, 0.0};
        Vector2 fg1Pos{fgX, 0.0};
        Vector2 fg2Pos{fgX + background.width * 2, 0.0};

        DrawTextureEx(background, bg1Pos, 0.0, 2.0, WHITE);
        DrawTextureEx(background, bg2Pos, 0.0, 2.0, WHITE);
        DrawTextureEx(midground, mg1Pos, 0.0, 2.0, WHITE);
        DrawTextureEx(midground, mg2Pos, 0.0, 2.0, WHITE);
        DrawTextureEx(foreground, fg1Pos, 0.0, 2.0, WHITE);
        DrawTextureEx(foreground, fg2Pos, 0.0, 2.0, WHITE);


        // ground check
        if(isOnAir(playerData, windowDimensions[1])){
            // applying gravity
            velocity += gravity * dT;
        } else {
            velocity = 0;
            playerData.imageRect.y = windowDimensions[1] - playerData.imageRect.height;
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
        for (int i = 0; i < 3; i++)
        {
            enemys[i].imageRect.x += enemyVelocity * dT;
        }

        finishLine += enemyVelocity * dT;        

        // update player animation
        if(playerData.runningTime >= playerData.updateTime && isGrounded){
            playerData = updateAnimData(playerData);
        }

        // update enemy animation
        for (int i = 0; i < ENEMY_COUNT; i++)
        {
            if(enemys[i].runningTime >= enemys[i].updateTime){
                enemys[i] = updateAnimData(enemys[i]);
            }
        }



        for(AnimData enemy : enemys){
            if(CheckCollisionRecs(enemy.imageRect, playerData.imageRect)){
                collision = true;
            }
        }

        if(collision){
            DrawText("You lose", windowDimensions[0]/4, windowDimensions[1]/4, 50, RED);
        }else if(playerData.imageRect.x >= finishLine){
            DrawText("You WON", windowDimensions[0]/2, windowDimensions[1]/4, 50, GREEN);       
        }else{
            // draw player
            DrawTexturePro(playerText, playerData.imageSourceRect, playerData.imageRect, worldPivotPoint, 0, WHITE);

            // draw enemy
            for (int i = 0; i < ENEMY_COUNT; i++)
            {
                DrawTexturePro(enemyTexture, enemys[i].imageSourceRect, enemys[i].imageRect, worldPivotPoint, 0, WHITE);
            }
        }
       

        // stop drawing
        EndDrawing();
    }
    UnloadTexture(playerText);
    UnloadTexture(enemyTexture);
    UnloadTexture(background);
    UnloadTexture(midground);
    UnloadTexture(foreground);
    CloseWindow();
}
