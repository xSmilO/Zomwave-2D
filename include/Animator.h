#pragma once

#include "raylib.h"
#include <string>
#include <unordered_map>
#include <vector>

/**
 * @struct Animation
 * @brief Stores data for a single animation sequence (frames, timing, looping).
 */
struct Animation {
    Texture2D *texture;          ///< Pointer to the spritesheet texture.
    Vector2 frameSize;           ///< Size of a single frame in pixels.
    std::vector<Vector2> frames; ///< UV / source-rect positions of each frame.
    float fps;                   ///< Playback speed in frames per second.
    bool isLooping;              ///< Whether the animation loops after reaching the end.
};

/**
 * @class Animator
 * @brief Manages sprite animations with state-based switching.
 *
 * Supports multiple named animation states (e.g. "idle", "walk", "shoot")
 * and provides frame-by-frame playback with optional looping.
 */
class Animator {
  private:
    bool animationFinished;                            ///< Flag set when a non-looping animation completes.
    double frameTimer;                                 ///< Accumulated time for frame stepping.
    unsigned int currentFrame;                         ///< Index of the currently displayed frame.
    std::string currentState;                          ///< Name of the active animation state.
    std::unordered_map<std::string, Animation> frames; ///< Map of state name to Animation data.

  public:
    Animator();

    /**
     * @brief Advances the animation by a given time delta.
     * @param dt Delta time in seconds.
     */
    void Update(float dt);

    /**
     * @brief Draws the current animation frame at the specified position.
     * @param position Destination rectangle on screen.
     * @param flipX   If true, the sprite is flipped horizontally.
     * @param rotation Rotation angle in degrees.
     */
    void Draw(Rectangle position, bool flipX = false, float rotation = 0.0f);

    /**
     * @brief Switches to a named animation state (resets frame timer).
     * @param state Name of the animation state to play.
     */
    void SetState(std::string state);

    /**
     * @brief Registers a new animation state.
     * @param state     Unique name for this animation.
     * @param texture   Pointer to the spritesheet texture.
     * @param frameSize Size of a single frame.
     * @param fps       Frames per second.
     * @param framesPos List of frame source-rect positions.
     * @param loop      Whether the animation should loop.
     */
    void AddAnimation(std::string state, Texture2D *texture, Vector2 frameSize, float fps,
                      std::vector<Vector2> framesPos, bool loop);

    /// Resets the animation to the first frame.
    void ResetAnimation();

    /// @return The name of the currently active animation state.
    std::string GetState() const {
        return currentState;
    }

    /// @return True if a non-looping animation has finished playing.
    bool IsAnimationFinished();
};
