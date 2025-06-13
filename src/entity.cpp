#include "entity.hpp"

Entity::Entity(Position position, sf::Texture texture) 
    : position(position), texture(texture), sprite(texture){
    sprite.setTexture(texture);
    sprite.setOrigin({0.01f, 0.01f});
    sprite.setScale({0.5f, 0.5f});
}

void Entity::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

sf::Sprite& Entity::getSprite() {
    return sprite;
}

void Entity::setActive(bool b) {
    active = b;
}

bool Entity::getActive() const {
    return active;
}

void Entity::setSprite(const sf::Sprite& spr) {
    sprite = spr;
}
void Entity::setScale(sf::Vector2f scale){
    sprite.setScale(scale);
}