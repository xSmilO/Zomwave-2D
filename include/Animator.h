#pragma once

#include "raylib.h"
#include <string>
#include <unordered_map>
#include <vector>

struct Animation {
    Texture2D *texture;
    Vector2 frameSize;
    std::vector<Vector2> frames;
    float fps;
    bool isLooping;
};

class Animator {
  private:
    bool animationFinished;
    double frameTimer;
    unsigned int currentFrame;
    std::string currentState;
    std::unordered_map<std::string, Animation> frames;

  public:
    Animator();

    void Update(float dt);
    void Draw(Rectangle position, bool flipX = false, float rotation = 0.0f);
    void SetState(std::string state);
    void AddAnimation(std::string state, Texture2D *texture, Vector2 frameSize,
                      float fps, std::vector<Vector2> framesPos, bool loop);
    void ResetAnimation();
    std::string GetState() const { return currentState; }
    bool IsAnimationFinished();
};
