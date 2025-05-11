#pragma once

#include <SFML/Graphics.hpp>
#include "Timer.h"

class WaveManager {
private:
    // Ints
    int m_currentWave = 0;
    int m_enemiesRemaining = 0;
    int m_enemiesSpawned = 0;
    int m_enemiesThisRound = 0;
    const int m_maxEnemies = 7;

    // Floats
    float m_waveDelayTime = 0.2f;
    float m_enemySpawnTime = 0.2f;

    // Bools
    bool m_isWaveActive = false;

    // Vectors
    sf::Vector2f m_waveTextPosition;

    // Timers
    Timer m_waveDelayTimer { m_waveDelayTime };
    Timer m_enemySpawnTimer { m_enemySpawnTime };

    // Functions
    int GetEnemiesForWave(int waveNumber);

public:
    WaveManager();


    void Update(float deltaTime);
    void StartWave();
    void EndWave();
    bool GetIsWaveComplete() const;
    bool GetCanSpawnEnemy() const;
    int GetRemainingEnemies() const;
    int GetCurrentWave() const;
    void EnemySpawned();
    void EnemyDefeated();

};


