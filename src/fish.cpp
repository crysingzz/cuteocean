#include "fish.hpp"
#include <SFML/Graphics.hpp>

// Реализация скрытой структуры
struct Fish::Impl {
    int hunger;
    int age;
    int type;

    Impl(int h, int a, int t) : hunger(h), age(a), type(t) {}
};

Fish::Fish(Position position, const sf::Texture& texture, int hunger, int age, int type)
    : Entity(position, texture), 
      pImpl(std::make_unique<Impl>(hunger, age, type)) 
{
    sprite.setTexture(this->texture);
}

Fish::~Fish() = default;

// Перемещающий конструктор
Fish::Fish(Fish&& other) noexcept
    : Entity(std::move(other)),
      pImpl(std::move(other.pImpl)) 
{}

// Перемещающее присваивание
Fish& Fish::operator=(Fish&& other) noexcept {
    if (this != &other) {
        Entity::operator=(std::move(other));
        pImpl = std::move(other.pImpl);
    }
    return *this;
}

// Реализации методов
int Fish::getAge() const {
    return pImpl->age;
}

int Fish::getHunger() const {
    return pImpl->hunger;
}

Position Fish::getPosition() const {
    return position;
}

void Fish::setPosition(Position new_pos) {
    position = new_pos;
    sprite.setPosition({position.x, position.y});
}

void Fish::decreaseHunger() {
    --(pImpl->hunger);
}

void Fish::decreaseAge() {
    --(pImpl->age);
}

int Fish::getType() const {
    return pImpl->type;
}

const sf::Texture& Fish::getTexture() const {
    return texture;
}

sf::Sprite& Fish::getSprite() {
    return sprite;
}

void Fish::setHunger(int h) {
    pImpl->hunger = h;
}

void Fish::setAge(int a) {
    pImpl->age = a;
}