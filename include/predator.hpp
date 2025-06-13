#ifndef PREDATOR_HPP
#define HERBIVORE_HPP

#include "fish.hpp"
class Predator : public Fish{
    public:
        Predator(Position position, sf:: Texture texture, int hunger,int age, int type);
        // virtual ~Herbivore();
};

#endif