#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "global.hpp"
#include <SFML/Graphics.hpp>

class Entity {
protected:
    Position position;
    sf::Sprite sprite;
    bool active = false;
    sf::Texture texture;

public:
    Entity(Position position, sf::Texture texture);
    
    virtual ~Entity() = default;
    Entity(const Entity& other) = default;
    Entity& operator=(const Entity& other) = default;
    Entity(Entity&& other) = default;
    Entity& operator=(Entity&& other) = default;

    void draw(sf::RenderWindow& window);
    void setActive(bool b);
    bool getActive() const;
    sf::Sprite& getSprite();
    void setSprite(const sf::Sprite& spr);
    void setScale(sf::Vector2f scale);
};

#endif