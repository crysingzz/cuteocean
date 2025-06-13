#ifndef HERBIVORE_HPP
#define HERBIVORE_HPP

#include "fish.hpp"
class Herbivore : public Fish{
        
    public:
        Herbivore(Position position, sf::Texture texture, int hunger,int age, int type);
        // virtual ~Herbivore();
    
        
};

#endif