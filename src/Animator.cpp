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

void Animator::Draw(Rectangle position, bool flipX, float rotation) {
    if (currentState == "")
        return;

    Animation &anim = frames[currentState];

    Vector2 pos = anim.frames[currentFrame];

    Rectangle sourceRec = {pos.x * anim.frameSize.x, pos.y * anim.frameSize.y,
                           (float)anim.frameSize.x, (float)anim.frameSize.y};

    if (flipX)
        sourceRec.width = -sourceRec.width;

    Vector2 origin = {position.width / 2.0f, position.height / 2.0f};

    DrawTexturePro(anim.texture, sourceRec, position, origin, rotation, WHITE);
}

void Animator::SetState(std::string state) {
    if (state == currentState)
        return;
    currentState = state;
    currentFrame = 0;
}

void Animator::AddAnimation(std::string state, std::string texPath,
                            Vector2 frameSize, float fps,
                            std::vector<Vector2> framesPos, bool loop) {
    Animation anim;
    anim.texture = LoadTexture(texPath.c_str());
    anim.frameSize = frameSize;
    anim.fps = fps;
    anim.isLooping = loop;
    anim.frames = framesPos;
    frames[state] = anim;
}

void Animator::ResetAnimation() { currentFrame = 0; }
