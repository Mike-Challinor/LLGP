#include "WaveManager.h"

WaveManager::WaveManager()
{
}

void WaveManager::StartWave()
{
	m_isWaveActive = true;
	m_currentWave++;
	m_enemiesThisRound = GetEnemiesForWave(m_currentWave); // Set the number of enemies for this wave
	m_enemiesRemaining = m_enemiesThisRound; // Set the number of enemies still alive

	// Start the delay, before spawning enemies
	m_waveDelayTimer.Start();
}

void WaveManager::EndWave()
{
	m_isWaveActive = false;
	m_enemiesSpawned = 0;

	// Start the next wave
	StartWave();
}

int WaveManager::GetEnemiesForWave(int waveNumber)
{
	int baseEnemies = 3;   // Starting number of enemies
	int scalingFactor = 2; // Rate at which enemies increase each wave

	// Increase the number of enemies based on the wave number
	return baseEnemies + (waveNumber - 1) * scalingFactor;
}

bool WaveManager::GetIsWaveComplete() const
{
	if (m_enemiesRemaining <= 0)
		return true;

	else
		return false;
}

bool WaveManager::GetCanSpawnEnemy() const
{
	if (m_enemySpawnTimer.IsActive() && !m_enemySpawnTimer.IsFinished())
		return false;
	if (m_waveDelayTimer.IsActive() && !m_waveDelayTimer.IsFinished())
		return false;
	if (m_enemiesSpawned < m_maxEnemies && m_enemiesSpawned < m_enemiesThisRound)
		return true;
	else
		return false;		
}

int WaveManager::GetRemainingEnemies() const
{
	return m_enemiesRemaining;
}

int WaveManager::GetCurrentWave() const
{
	return m_currentWave;
}

void WaveManager::EnemySpawned()
{
	// Increment the amount of enemies spawned
	m_enemiesSpawned++;

	// Start the enemy spawn timer
	m_enemySpawnTimer.Start();
}

void WaveManager::EnemyDefeated()
{
	// Decrement the amount of enemies remain
	m_enemiesRemaining--;
}

void WaveManager::Update(float deltaTime)
{
	// --- Update timers if they are active
	if (m_waveDelayTimer.IsActive())
		m_waveDelayTimer.Update(deltaTime);

	if (m_enemySpawnTimer.IsActive())
		m_enemySpawnTimer.Update(deltaTime);
}
