#ifndef ALGAE_HPP
#define ALGAE_HPP

#include "entity.hpp"

class Algae : public Entity{
    public:
        Algae(Position position, sf:: Texture texture);
};

#endif