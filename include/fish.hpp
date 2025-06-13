#ifndef FISH_HPP
#define FISH_HPP

#include "entity.hpp"
#include <memory>

class Fish : public Entity {
private:
    struct Impl; // Предварительное объявление реализации

public:
    Fish(Position position, const sf::Texture& texture, int hunger, int age, int type);
    ~Fish();
    
    // Удаляем копирующие операции
    Fish(const Fish&) = delete;
    Fish& operator=(const Fish&) = delete;
    
    // Перемещающие операции
    Fish(Fish&& other) noexcept;
    Fish& operator=(Fish&& other) noexcept;

    // Интерфейс класса
    int getAge() const;
    int getHunger() const;
    Position getPosition() const;
    void setPosition(Position new_pos);
    void decreaseHunger();
    void decreaseAge();
    int getType() const;
    const sf::Texture& getTexture() const;
    sf::Sprite& getSprite();
    void setHunger(int h);
    void setAge(int a);

private:
    std::unique_ptr<Impl> pImpl;
};

#endif