#include "Pistol.h"
#include "raylib.h"
#include <stdio.h>

Pistol::Pistol() {
    std::vector<Vector2> shootFramePos = {{0, 0}};

    animator.AddAnimation("shoot", "../assets/pistol.png", {64, 32}, 1, 
                          shootFramePos, false);

    animator.SetState("shoot");
}

void Pistol::Update() { animator.Update(); }

void Pistol::Draw(Vector2 position, float rotation) {
    Rectangle newPos = {position.x, position.y, 60, 30};
    DrawCircleV(position, 10, BLUE);
    animator.Draw(newPos);
}
