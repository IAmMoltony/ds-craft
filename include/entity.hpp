/**
 * @file entity.hpp
 * @brief Entity classes and functions
*/

#pragma once

#include "camera.h"
#include "defines.h"
#include "block.hpp"
#include "animsprite.hpp"
#include "facing.hpp"

class Entity;

typedef std::vector<std::unique_ptr<Entity>> EntityList;

/**
 * @brief Class representing an entity
*/
class Entity
{
public:
    /**
     * @brief X position of the entity
    */
    s16 x;

    /**
     * @brief Y position of the entity
    */
    s16 y;

    /**
     * @brief X velocity of the entity
    */
    float velX;

    /**
     * @brief Y velocity of the entity
    */
    float velY;

    /**
     * @brief Whether the entity is falling or not
    */
    bool falling;

    /**
    * @brief Whether the entity is jumping or not
    */
    bool jumping;

    /**
     * @brief The entity's health
    */
    s16 health;

    /**
     * @brief Load entity textures
    */
    static void loadTextures(void);

    /**
     * @brief Unload entity textures
    */
    static void unloadTextures(void);

    /**
     * @brief Load entity sounds
    */
    static void loadSounds(void);

    /**
     * @brief Unload entity sounds
    */
    static void unloadSounds(void);

    /**
     * @brief Entity constructor
     * @param x X position of the entity
     * @param y Y position of the entity
    */
    Entity(s16 x, s16 y);

    /**
     * @brief Entity destructor
    */
    virtual ~Entity() = default;

    /**
     * @brief Draw the entity
     * @param camera camera to use
    */
    virtual void draw(Camera camera) = 0; // TODO const reference camera

    /**
     * @brief Update the entity
     * @param blocks list of blocks
     * @param camera camera (used for culling)
     * @param frames frame counter
    */
    virtual void update(BlockList &blocks, Camera camera, u16 frames) = 0; // TODO const reference camera; frames can be pulled from Game

    /**
     * @brief Get bottom hitbox of the entity
    */
    virtual Rect getRectBottom(void) = 0;

    /**
     * @brief Get top hitbox of the entity
    */
    virtual Rect getRectTop(void) = 0;

    /**
     * @brief Get left hitbox of the entity
    */
    virtual Rect getRectLeft(void) = 0;

    /**
     * @brief Get right hitbox of the entity
    */
    virtual Rect getRectRight(void) = 0;

    /**
     * @brief Get overall hitbox of the entity
    */
    virtual Rect getRect(void) = 0;

    /**
     * @brief Get entity's ID
     * @todo why is entity id a string?
    */
    virtual std::string id(void) = 0;

    /**
     * @brief This function is called when the entity gets damaged
    */
    virtual void onDealDamage(void);

    /**
     * @brief This function is called when the entity dies
     * @param entities list of entities
    */
    virtual void onDeath(EntityList &entities);

    /**
     * @brief Get entity's X position
     * @todo Remove this because x is public
    */
    s16 getX(void);

    /**
     * @brief Get entity's Y position
     * @todo Remove this because y is public
    */
    s16 getY(void);

    /**
     * @brief Check if the entity is dead
    */
    bool dead(void);

    /**
     * @brief Deal damage to the entity
     * @param amount the amount of damage to deal
    */
    void damage(u8 amount);
};

/**
 * @brief Class representing a pig entity
*/
class PigEntity : public Entity
{
private:
    /**
     * @brief Where the pig is facing
    */
    Facing facing;

    /**
     * @brief Whether the pig is moving or not
    */
    bool moving;

    /**
     * @brief Timer for damage overlay
    */
    u8 damageOverlayTimer;

    /**
     * @brief Timer for panic mode
    */
    u16 panicModeTimer;

public:
    PigEntity(s16 x, s16 y);
    void draw(Camera camera) override; // TODO const refernce
    void update(BlockList &blocks, Camera camera, u16 frames) override; // TODO const reference and getFrameCounter()
    Rect getRectBottom(void) override;
    Rect getRectTop(void) override;
    Rect getRectLeft(void) override;
    Rect getRectRight(void) override;
    Rect getRect(void) override;
    std::string id(void) override;
    void onDealDamage(void) override;
    void onDeath(EntityList &entities) override;
};

class DropEntity : public Entity
{
private:
    /**
     * @brief The drop's item ID
    */
    InventoryItem::ID itemid;

    /**
     * @brief Spin counter
     * @todo think of better description for this thing
    */
    u8 spin;

    /**
     * @brief If true, incerment spin counter, otherwise, decrement it
    */
    bool increment;

    /**
     * @brief If true, the drop will be slightly a bit more up
     * @todo uhh also think of better description
    */
    bool upOffset;

public:
    /**
     * @brief Drop entity constructor
     * @param itemid drop's item ID
    */
    DropEntity(s16 x, s16 y, InventoryItem::ID itemid);
    void draw(Camera camera) override;
    void update(BlockList &blocks, Camera camera, u16 frames) override;
    Rect getRectBottom(void) override;
    Rect getRectTop(void) override;
    Rect getRectLeft(void) override;
    Rect getRectRight(void) override;
    Rect getRect(void) override;
    std::string id(void) override;

    /**
     * @brief Get the drop's item ID
    */
    InventoryItem::ID getItemID(void);
};
