#pragma once

#include "raylib.h"
#include <string>
#include <unordered_map>
#include <vector>

struct Animation {
    Texture2D texture;
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
    ~Animator();

    void Update();
    void Draw(Rectangle position, bool flipX = false);
    void SetState(std::string state);
    void AddAnimation(std::string state, std::string texPath, Vector2 frameSize,
                      float fps, std::vector<Vector2> framesPos, bool loop);
    void ResetAnimation();
    bool IsAnimationFinished();
};
