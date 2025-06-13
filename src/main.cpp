
#include "global.hpp"
#include "herbivore.hpp"
#include "fish.hpp"
#include "algae.hpp"
#include "predator.hpp"
#include <cstdlib>
#include <time.h>
#include <cmath>
#include <sstream>
#include <iomanip>
#include "logger.hpp"

int main(){
    Logger::getInstance().configure(
        LogLevel::DEBUG,   // Уровень логирования
        true,              // Логировать в консоль
        true,              // Логировать в файл
        "ocean_log.txt"    // Имя файла
    );
    Logger::getInstance().log(LogLevel::INFO, "Starting ocean simulation");
    Logger::getInstance().log(LogLevel::DEBUG, "Initialized with window size", WIDTH, "x", HEIGHT);
    try
    {
        sf::Texture herbivore_texture("/home/crysingzz/Desktop/main_programm_folder/hse_vibes/ocean/sprites/fish_harbivore.jpg");
        sf::Texture predator_texture("/home/crysingzz/Desktop/main_programm_folder/hse_vibes/ocean/sprites/fish_predator.jpg");
        sf::Texture algae_texture("/home/crysingzz/Desktop/main_programm_folder/hse_vibes/ocean/sprites/algae.jpg");
        sf::Texture sand_texture("/home/crysingzz/Desktop/main_programm_folder/hse_vibes/ocean/sprites/sand.jpeg");
        sf::Texture water_texture("/home/crysingzz/Desktop/main_programm_folder/hse_vibes/ocean/sprites/water.jpeg");
        sf::Sprite water_sprite(water_texture);
        water_sprite.setPosition({0.0f,0.0f});
        water_sprite.setScale({0.5f,0.5f});
        
        // sf::Sprite algae_sprite(algae_texture);
        Position default_position = {0.0f,0.0f};

        Herbivore first_herb(default_position,herbivore_texture,hunger,age, herbivore_type);
        sf::Sprite herbivore_sprite(herbivore_texture);
        herbivore_sprite.setScale({0.2f,0.2f});
        first_herb.setSprite(herbivore_sprite);

        Predator first_pred(default_position,predator_texture,hunger,age,predator_type);
        sf::Sprite predator_sprite(predator_texture);
        predator_sprite.setScale({0.2f,0.2f});
        first_pred.setSprite(predator_sprite);


        sf::FloatRect bounds = water_sprite.getGlobalBounds();
        float waterHeight = HEIGHT - SAND_HEIGHT;

        

        Algae first_algae(default_position,algae_texture);
        sf::Sprite algae_sprite(algae_texture);
        first_algae.setSprite(algae_sprite);


        float spriteWidth = bounds.size.x;
        float spriteHeight = bounds.size.y;
        int numX = (WIDTH / spriteWidth) + 1;
        int numY = (waterHeight / spriteHeight) +1;
        std::vector<sf::Sprite> ocean;
        ocean.reserve(numX * numY);
        std::vector<sf::Sprite> algae_vector;

        
        


        for (int y = 0; y < numY; ++y) {
            for (int x = 0; x < numX; ++x) {
                sf::Sprite newSprite = water_sprite;
                newSprite.setPosition({x * spriteWidth, y * spriteHeight});
                ocean.push_back(newSprite);

            }
        }

    

        int sandNumY = (SAND_HEIGHT / spriteHeight) + 1;
        std::vector<sf::Sprite> sand;
        sand.reserve(numX * sandNumY);
        sf::Sprite sand_sprite(sand_texture);
        for (int y = 0; y < sandNumY; ++y) {
            for (int x = 0; x < numX; ++x) {
                sf::Sprite newSprite = sand_sprite;
                // Позиция по Y: начало песка (waterHeight) + текущее смещение
                newSprite.setPosition({x * spriteWidth, waterHeight + y * spriteHeight});
                sand.push_back(newSprite);
            }
        }

        

        const int ALGAE_COUNT = sand.size() / 2;
        algae_vector.reserve(ALGAE_COUNT);

        for (int i = 0; i < ALGAE_COUNT; ++i) {
            sf::Sprite new_algae = first_algae.getSprite();
            
            float x_pos = sand[i].getPosition().x;
            float y_pos = sand[i].getPosition().y - 225;
            
            new_algae.setPosition({x_pos, y_pos});
            new_algae.setScale({0.5f, 0.5f});
            algae_vector.push_back(new_algae);
        }

    

    
        srand(static_cast<unsigned>(time(0)));
        float algae_spawn_zone = HEIGHT - SAND_HEIGHT - 250;

        // Зоны спавна
        float predator_zone_top = 0;
        float predator_zone_bottom = waterHeight / 2;
        float herbivore_zone_top = predator_zone_bottom;
        float herbivore_zone_bottom = algae_spawn_zone;

        // Проверка валидности зон
        if (herbivore_zone_bottom <= herbivore_zone_top) {
            herbivore_zone_bottom = herbivore_zone_top + 100.f;
        }

        // Создаем векторы для хранения скоростей рыб
        std::vector<sf::Vector2f> herbivore_velocities;
        std::vector<sf::Vector2f> predator_velocities;

        // Спавн травоядных рыб
        const int HERBIVORE_COUNT = 20;

        std::vector<Herbivore> herbivores;
        herbivores.reserve(HERBIVORE_COUNT);

        sf::Sprite temp_sprite(herbivore_texture);
        temp_sprite.setScale({0.1f, 0.1f});
        sf::FloatRect herbivoreBounds = temp_sprite.getGlobalBounds();

        for (int i = 0; i < HERBIVORE_COUNT; ++i) {
            bool positionValid = false;
            sf::Sprite sprite(herbivore_texture);
            sprite.setScale({0.1f,0.1f});
            sf::FloatRect bounds;

            sf::Vector2f newPos;
            int attempts = 0;
            const int maxAttempts = 100;

            while (!positionValid && attempts < maxAttempts) {
                newPos.x = static_cast<float>(std::rand() % WIDTH);
                newPos.y = herbivore_zone_top + static_cast<float>(std::rand() % 
                            static_cast<int>(herbivore_zone_bottom - herbivore_zone_top - herbivoreBounds.size.y));
                
                sprite.setPosition(newPos);
                bounds = sprite.getGlobalBounds();

                positionValid = true;
                
                for (auto& other : herbivores) {
                    if (bounds.findIntersection(other.getSprite().getGlobalBounds())) {
                        positionValid = false;
                        break;
                    }
                }
                
                if (positionValid) {
                    for (auto& algae_block : algae_vector) {
                        if (bounds.findIntersection(algae_block.getGlobalBounds())) {
                            positionValid = false;
                            break;
                        }
                    }
                }
                
                attempts++;
            }

            if (positionValid) {
                Position p = {newPos.x, newPos.y};
                Herbivore h(p, herbivore_texture, hunger, age, herbivore_type);
                h.setSprite(sprite);
                herbivores.push_back(std::move(h));
                
                // Уменьшаем скорость в 10 раз
                float speed = 150.0f + static_cast<float>(std::rand() % 3);
                float angle = static_cast<float>(std::rand() % 360) * 3.14159f / 180.f;
                herbivore_velocities.push_back(sf::Vector2f(
                    std::cos(angle) * speed,
                    std::sin(angle) * speed
                ));
            } else {
                std::cerr << "Failed to place herbivore after " << maxAttempts << " attempts.\n";
            }
        }
        Logger::getInstance().log(LogLevel::DEBUG, "Spawned herbivores:", herbivores.size());

        // Спавн хищников
        const int PREDATOR_COUNT = 7;
        std::vector<Predator> predators;
        predators.reserve(PREDATOR_COUNT);

        for (int i = 0; i < PREDATOR_COUNT; ++i) {
            bool positionValid = false;
            sf::Sprite sprite(predator_texture);
            sprite.setScale({0.3f, 0.3f});
            sf::Vector2f newPos;
            int attempts = 0;
            const int maxAttempts = 100;

            while (!positionValid && attempts < maxAttempts) {
                newPos.x = static_cast<float>(std::rand() % WIDTH);
                newPos.y = predator_zone_top + static_cast<float>(std::rand() % 
                            static_cast<int>(predator_zone_bottom - predator_zone_top));
                
                sprite.setPosition(newPos);

                positionValid = true;
                
                for (auto& other : predators) {
                    if (sprite.getGlobalBounds().findIntersection(other.getSprite().getGlobalBounds())) {
                        positionValid = false;
                        break;
                    }
                }
                
                attempts++;
            }

            if (positionValid) {
                Position p = {newPos.x, newPos.y};
                Predator pred(p, predator_texture, hunger, age, predator_type);
                pred.setSprite(sprite);
                predators.push_back(std::move(pred));
                
                // Увеличиваем скорость хищников
                float speed = 70.0f + static_cast<float>(std::rand() % 20); // 70-90
                float angle = static_cast<float>(std::rand() % 360) * 3.14159f / 180.f;
                predator_velocities.push_back(sf::Vector2f(
                    std::cos(angle) * speed,
                    std::sin(angle) * speed
                ));
            }
        
        }
        Logger::getInstance().log(LogLevel::DEBUG, "Spawned predators:", predators.size());


        sf::RenderWindow window = sf::RenderWindow(sf::VideoMode({WIDTH,HEIGHT}),"Ocean");
        window.setFramerateLimit(60);
        
        // Часы для расчета дельты времени
        sf::Clock clock;
        
        // Загрузка шрифта для отображения тика
        sf::Font font("/home/crysingzz/Desktop/main_programm_folder/hse_vibes/ocean/sprites/Monocraft.ttc");
    
        // Текст для отображения тика
        sf::Text tickText(font);
        tickText.setCharacterSize(40);
        tickText.setFillColor(sf::Color::White);
        tickText.setPosition({10, 10});
        
        // Переменные для управления тиками
        unsigned long currentTick = 0;
        unsigned long slowTick = 0; // Медленные тики
        const float TICK_RATE = 1.0f / 1.0f; // 6 тиков в секунду (в 10 раз медленнее)
        float accumulatedTime = 0.0f;
        
        while (window.isOpen())
        {
            float deltaTime = clock.restart().asSeconds();
            accumulatedTime += deltaTime;
            
            // Обработка событий
            while (const std::optional event = window.pollEvent())
            {
                if (event->is<sf::Event::Closed>())
                {
                    window.close();
                }
                else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
                {
                    if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
                        window.close();
                }
            }
            
            // Обновление по тикам
            while (accumulatedTime >= TICK_RATE) {
                accumulatedTime -= TICK_RATE;
                currentTick++;
                slowTick++;
                // Спавн новых водорослей каждые 5 тиков
                if (slowTick % 5 == 0) {
                    algae_vector.clear();
                    for (int i = 0; i < ALGAE_COUNT; ++i) {
                        sf::Sprite newAlgae(algae_texture);
                        newAlgae.setPosition({
                            sand[i].getPosition().x,
                            sand[i].getPosition().y - 225
                        });
                        newAlgae.setScale({0.5f, 0.5f});
                        algae_vector.push_back(newAlgae);
                    }
                }
                for (auto& herb : herbivores) {
                    herb.setHunger(herb.getHunger() - 1);
                    herb.setAge(herb.getAge() + 1);
                }
                for (auto& pred : predators) {
                    pred.setHunger(pred.getHunger() - 1);
                    pred.setAge(pred.getAge() + 1);
                }
                
                // Обновление позиций травоядных
                for (int i = 0; i < int(herbivores.size()); ++i) {
                    if (slowTick % 10 == 0 && std::rand() % 100 < 2) {
                        float angleChange = (std::rand() % 100 - 30) * 3.14159f / 180.f;
                        float currentAngle = std::atan2(herbivore_velocities[i].y, herbivore_velocities[i].x);
                        float newAngle = currentAngle + angleChange;
                        float speed = std::sqrt(
                            herbivore_velocities[i].x * herbivore_velocities[i].x + 
                            herbivore_velocities[i].y * herbivore_velocities[i].y
                        );
                        
                        herbivore_velocities[i] = sf::Vector2f(
                            std::cos(newAngle) * speed,
                            std::sin(newAngle) * speed
                        );
                    }
                    
                    // Применяем скорость
                    herbivores[i].getSprite().move(
                    { herbivore_velocities[i].x + 10.0f,
                        herbivore_velocities[i].y+10.0f}
                    );
                }
                
                // Обновление позиций хищников
                for (int i = 0; i < int(predators.size()); ++i) {
                    // Увеличиваем частоту изменения направления до 10%
                    if (std::rand() % 100) {
                        // Увеличиваем диапазон изменения угла
                        float angleChange = (std::rand() % 180 - 60); // -60° до +60°
                        
                        // Добавляем случайное изменение скорости
                        float speedChange = 1.0f + static_cast<float>(std::rand() % 20); // 1.0-3.0
                        
                        // Плавное изменение направления (инерция)
                        float currentAngle = std::atan2(predator_velocities[i].y, predator_velocities[i].x);
                        float newAngle = currentAngle + angleChange * 0.5f;
                        
                        // Текущая скорость с учетом изменения
                        float currentSpeed = std::sqrt(
                            predator_velocities[i].x * predator_velocities[i].x + 
                            predator_velocities[i].y * predator_velocities[i].y
                        );
                        
                        float newSpeed = currentSpeed * speedChange;
                        
                        // Ограничиваем максимальную скорость
                        if (newSpeed > 100.0f) newSpeed = 100.0f;
                        if (newSpeed < 40.0f) newSpeed = 40.0f;
                        
                        predator_velocities[i] = sf::Vector2f(
                            std::cos(newAngle) * newSpeed,
                            std::sin(newAngle) * newSpeed
                        );
                    }
                    
                    // Применяем скорость (убраны +5.0f)
                    predators[i].getSprite().move(
                        predator_velocities[i]
                    );
                }

                std::vector<std::pair<Herbivore, sf::Vector2f>> newHerbivores; // Для новых рыб
                for (int i = 0; i < int(herbivores.size()); ++i) {
                    for (int j = i + 1; j < int(herbivores.size()); ++j) {
                        if (herbivores[i].getSprite().getGlobalBounds().findIntersection(
                            herbivores[j].getSprite().getGlobalBounds())) {
                            
                            // С вероятностью 30% создаем новую рыбу при столкновении
                            if (std::rand() % 100 < 50) {
                                // Позиция новой рыбы - среднее между позициями двух рыб
                                sf::Vector2f pos1 = herbivores[i].getSprite().getPosition();
                                sf::Vector2f pos2 = herbivores[j].getSprite().getPosition();
                                sf::Vector2f newPos = (pos1 + pos2) / 2.0f;
                                
                                // Создаем новую рыбу
                                Herbivore newHerb({newPos.x, newPos.y}, herbivore_texture, hunger, age, herbivore_type);
                                sf::Sprite newSprite(herbivore_texture);
                                newSprite.setScale({0.1f, 0.1f});
                                newSprite.setPosition(newPos);
                                newHerb.setSprite(newSprite);

                                
                                // Инициализируем случайную скорость
                                float speed = 50.0f + static_cast<float>(std::rand() % 3);
                                float angle = static_cast<float>(std::rand() % 360) * 3.14159f / 180.f;
                                sf::Vector2f newVelocity = {
                                    std::cos(angle) * speed,
                                    std::sin(angle) * speed
                                };
                                
                                // Добавляем новую рыбу во временный вектор
                                newHerbivores.push_back({std::move(newHerb), newVelocity});
                            }
                        }
                    }
                }
                if (!newHerbivores.empty()) {
                    Logger::getInstance().log(LogLevel::DEBUG, "New herbivores born:", newHerbivores.size());
                }
                
                // Добавляем новых рыб в основные векторы
                for (auto& [herb, vel] : newHerbivores) {
                    if (newHerbivores.size() < 15)
                    {
                        herbivores.push_back(std::move(herb));
                        herbivore_velocities.push_back(vel);
                    }
                    
                    
                }
            

                // Обработка границ с отскоком для травоядных
                for (int i = 0; i < int(herbivores.size()); ++i) {
                    sf::FloatRect bounds = herbivores[i].getSprite().getGlobalBounds();
                    sf::Vector2f pos = herbivores[i].getSprite().getPosition();
                    bool hitWall = false;
                    
                    if (pos.x < 0) {
                        pos.x = 0;
                        herbivore_velocities[i].x = std::abs(herbivore_velocities[i].x);
                        hitWall = true;
                    } 
                    else if (pos.x + bounds.size.x > WIDTH) {
                        pos.x = WIDTH - bounds.size.x;
                        herbivore_velocities[i].x = -std::abs(herbivore_velocities[i].x);
                        hitWall = true;
                    }
                    
                    if (pos.y < 0) {
                        pos.y = 0;
                        herbivore_velocities[i].y = std::abs(herbivore_velocities[i].y);
                        hitWall = true;
                    } 
                    else if (pos.y + bounds.size.y > waterHeight) {
                        pos.y = waterHeight - bounds.size.y;
                        herbivore_velocities[i].y = -std::abs(herbivore_velocities[i].y);
                        hitWall = true;
                    }
                    
                    if (hitWall) {
                        herbivores[i].getSprite().setPosition(pos);
                    }
                }
                for (auto& herb : herbivores) {
                    herb.setHunger(herb.getHunger() - 1);
                    herb.setAge(herb.getAge() + 1);
                }
                for (auto& pred : predators) {
                    pred.setHunger(pred.getHunger() - 1);
                    pred.setAge(pred.getAge() + 1);
                }
                

                // Обработка границ с отскоком для хищников
                for (int i = 0; i < int(predators.size()); ++i) {
                    sf::FloatRect bounds = predators[i].getSprite().getGlobalBounds();
                    sf::Vector2f pos = predators[i].getSprite().getPosition();
                    bool hitWall = false;
                    
                    if (pos.x < 0) {
                        pos.x = 0;
                        predator_velocities[i].x = std::abs(predator_velocities[i].x);
                        hitWall = true;
                    } 
                    else if (pos.x + bounds.size.x > WIDTH) {
                        pos.x = WIDTH - bounds.size.x;
                        predator_velocities[i].x = -std::abs(predator_velocities[i].x);
                        hitWall = true;
                    }
                    
                    if (pos.y < 0) {
                        pos.y = 0;
                        predator_velocities[i].y = std::abs(predator_velocities[i].y);
                        hitWall = true;
                    } 
                    else if (pos.y + bounds.size.y > waterHeight) {
                        pos.y = waterHeight - bounds.size.y;
                        predator_velocities[i].y = -std::abs(predator_velocities[i].y);
                        hitWall = true;
                    }
                    
                    if (hitWall) {
                        predators[i].getSprite().setPosition(pos);
                    }
                }
                std::vector<std::pair<Predator, sf::Vector2f>> newPredators;
                for (int i = 0; i < int(predators.size()); ++i) {
                    for (int j = i + 1; j < int(predators.size()); ++j) {
                        if (predators[i].getSprite().getGlobalBounds().findIntersection(
                            predators[j].getSprite().getGlobalBounds())) {
                            
                            // 5% шанс создания нового хищника
                            if (std::rand() % 100 < 30) {
                                // Позиция нового хищника - средняя между родителями
                                sf::Vector2f pos1 = predators[i].getSprite().getPosition();
                                sf::Vector2f pos2 = predators[j].getSprite().getPosition();
                                sf::Vector2f newPos = (pos1 + pos2) / 2.0f;
                                
                                // Создаем нового хищника
                                Predator newPred({newPos.x, newPos.y}, predator_texture, hunger, age, predator_type);
                                sf::Sprite newSprite(predator_texture);
                                newSprite.setScale({0.3f, 0.3f});
                                newSprite.setPosition(newPos);
                                newPred.setSprite(newSprite);
                                
                                // Инициализируем случайную скорость
                                float speed = 70.0f + static_cast<float>(std::rand() % 20);
                                float angle = static_cast<float>(std::rand() % 360) * 3.14159f / 180.f;
                                sf::Vector2f newVelocity = {
                                    std::cos(angle) * speed,
                                    std::sin(angle) * speed
                                };
                                
                                
                                newPredators.push_back({std::move(newPred), newVelocity});
                            }
                        }
                    }

                }
                for (int i = herbivores.size() - 1; i >= 0; i--) {
                    if (herbivores[i].getHunger() <= 0 || herbivores[i].getAge() > 15) {
                        herbivores.erase(herbivores.begin() + i);
                        herbivore_velocities.erase(herbivore_velocities.begin() + i);
                    }
                }
                for (int i = predators.size() - 1; i >= 0; i--) {
                    if (predators[i].getHunger() <= 0 || predators[i].getAge() > 50) {
                        predators.erase(predators.begin() + i);
                        predator_velocities.erase(predator_velocities.begin() + i);
                    }
                }
                for (auto& [pred, vel] : newPredators) {
                    if (predators.size() < 15)
                    {
                        predators.push_back(std::move(pred));
                        predator_velocities.push_back(vel);
                    }
                    
                    
                }
                if (!newPredators.empty()) {
                    Logger::getInstance().log(LogLevel::DEBUG, "New predators born:", newPredators.size());
                }

                // Коллизии травоядных с водорослями (поедание) - каждый тик
                for (auto& herb : herbivores) {
                    for (auto it = algae_vector.begin(); it != algae_vector.end(); ) {
                        if (herb.getSprite().getGlobalBounds().findIntersection(it->getGlobalBounds())) {
                            it = algae_vector.erase(it);
                            herb.setHunger(5);
                            Logger::getInstance().log(LogLevel::DEBUG, "Herbivore ate algae. Hunger:", herb.getHunger());
                        } else {
                            ++it;
                        }
                    }
                }

                // Коллизии хищников с травоядными (поедание) - каждый тик
                for (auto& pred : predators) {
                    for (int i = 0; i < int(herbivores.size()); ) {
                        if (pred.getSprite().getGlobalBounds().findIntersection(herbivores[i].getSprite().getGlobalBounds())) {
                            herbivores.erase(herbivores.begin() + i);
                            herbivore_velocities.erase(herbivore_velocities.begin() + i);
                            pred.setHunger(5);
                            Logger::getInstance().log(LogLevel::DEBUG, "Predator ate herbivore. Hunger:", pred.getHunger());
                        } else {
                            ++i;
                        }
                    }
                }
                for (auto& [herb, vel] : newHerbivores) {
                    if (herbivores.size() < 20)
                    {
                        herbivores.push_back(std::move(herb));
                        herbivore_velocities.push_back(vel);
                    }
                    
                    
                }    
            }

            for (int i = herbivores.size() - 1; i >= 0; i--) {
                    if (herbivores[i].getHunger() <= 0 || herbivores[i].getAge() > 15) {
                        Logger::getInstance().log(LogLevel::DEBUG, "Herbivore died: hunger=", 
                        herbivores[i].getHunger(), " age=", herbivores[i].getAge());
                        herbivores.erase(herbivores.begin() + i);
                        herbivore_velocities.erase(herbivore_velocities.begin() + i);
                    }
                }
                
                // Удаление хищников
            for (int i = predators.size() - 1; i >= 0; i--) {
                if (predators[i].getHunger() <= 0 || predators[i].getAge() > 75) {
                    Logger::getInstance().log(LogLevel::DEBUG, "Predator died: hunger=", 
                    predators[i].getHunger(), " age=", predators[i].getAge());
                    predators.erase(predators.begin() + i);
                    predator_velocities.erase(predator_velocities.begin() + i);
                }
            }
 
            
            if (slowTick % 100 == 0) {
                    Logger::getInstance().log(LogLevel::DEBUG, 
                    "Tick:", currentTick,
                    "Herbivores:", herbivores.size(),
                    "Predators:", predators.size(),
                    "Algae:", algae_vector.size());
            }
    
                
            // Обновление текста с номером тика
            std::ostringstream tickStream;
            tickStream << "Tick: " << std::setw(8) << currentTick;
            tickText.setString(tickStream.str());
            // Отрисовка
            window.clear();
            for(auto water_block : ocean)
                window.draw(water_block);
            for(auto sand_block : sand)
                window.draw(sand_block);
            for(auto algae_block : algae_vector)
                window.draw(algae_block);
            for (auto& herb : herbivores) {
                window.draw(herb.getSprite());
            }
            for (auto& pred : predators) 
                window.draw(pred.getSprite());
                
            // Отображение номера тика
            window.draw(tickText);
                
            window.display();
        }
    }
    catch (const std::exception& e) {
        Logger::getInstance().log(LogLevel::ERROR, "Exception caught:", e.what());
    }

    Logger::getInstance().log(LogLevel::INFO, "Simulation finished");
    return 0;
    
    

    
    
}