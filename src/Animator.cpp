#include "Animator.h"
#include "raylib.h"

Animator::Animator() { frames = {}; }

Animator::~Animator() {
    for (const auto &[state, anim] : frames) {
        UnloadTexture(anim.texture);
    }
}

void Animator::Update() {
    if (currentState == "")
        return;

    Animation &anim = frames[currentState];
    frameTimer += GetFrameTime();
    float timePerFrame = 1.0f / anim.fps;

    if (frameTimer >= timePerFrame) {
        frameTimer = 0.0f;
        currentFrame++;

        if (currentFrame >= anim.frames.size()) {
            if (anim.isLooping) {
                currentFrame = 0;
            } else {
                currentFrame = anim.frames.size() - 1;
            }
        }
    }
}

void Animator::Draw(Rectangle position, bool flipX) {
    if (currentState == "")
        return;

    Animation &anim = frames[currentState];

    Vector2 pos = anim.frames[currentFrame];

    Rectangle sourceRec = {pos.x * anim.tileSize, pos.y * anim.tileSize,
                           (float)anim.tileSize, (float)anim.tileSize};

    if (flipX)
        sourceRec.width = -sourceRec.width;

    DrawTexturePro(anim.texture, sourceRec, position, {0.0f, 0.0f}, 0, WHITE);
}

void Animator::SetState(std::string state) {
    if(state == currentState) return;
    currentState = state;
    currentFrame = 0;
}

void Animator::AddAnimation(std::string state, std::string texPath,
                            int tileSize, float fps,
                            std::vector<Vector2> framesPos, bool loop) {
    Animation anim;
    anim.texture = LoadTexture(texPath.c_str());
    anim.tileSize = tileSize;
    anim.fps = fps;
    anim.isLooping = loop;
    anim.frames = framesPos;
    frames[state] = anim;
}
