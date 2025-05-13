#include "WaveManager.h"

// --- Constructor ---
WaveManager::WaveManager()
{
}

// --- Function for starting the next wave ---
void WaveManager::StartWave()
{
	m_isWaveActive = true; // Set wave to be active
	m_currentWave++; // Increment the wave number
	m_enemiesThisRound = GetEnemiesForWave(m_currentWave); // Set the number of enemies for this wave
	m_enemiesRemaining = m_enemiesThisRound; // Set the number of enemies still alive

	// Start the delay, before spawning enemies
	m_waveDelayTimer.Start();
}

// --- Function for ending the current wave ---
void WaveManager::EndWave()
{
	m_isWaveActive = false; // Set the wave to be inactive
	m_enemiesSpawned = 0; // Reset the number of spawned enemies

	// Start the next wave
	StartWave();
}

// --- Function that returns the number of enemies for the current wave ---
int WaveManager::GetEnemiesForWave(int waveNumber)
{
	int baseEnemies = 3;   // Starting number of enemies
	int scalingFactor = 2; // Rate at which enemies increase each wave

	// Increase the number of enemies based on the wave number
	return baseEnemies + (waveNumber - 1) * scalingFactor;
}

// --- Function that returns whether the wave has completed ---
bool WaveManager::GetIsWaveComplete() const
{
	if (m_enemiesRemaining <= 0)
		return true;

	else
		return false;
}

// --- Function that returns whether an enemy can be spawned ---
bool WaveManager::GetCanSpawnEnemy() const
{
	// Dont spawn enemy if the spawn timer is active but hasn't finished
	if (m_enemySpawnTimer.IsActive() && !m_enemySpawnTimer.IsFinished()) 
		return false;
	// Dont spawn enemy if the wave delay timer is active but hasn't finished
	if (m_waveDelayTimer.IsActive() && !m_waveDelayTimer.IsFinished()) 
		return false;
	// Spawn if the enemies spawned is less than max allowed enemies at one time and less than total enemies to spawn
	if (m_enemiesSpawned < m_maxEnemies && m_enemiesSpawned < m_enemiesThisRound) 
		return true;
	// Don't spawn enemy in case no conditions are met
	else
		return false;		
}

// --- Function that returns the enemies remaining ---
int WaveManager::GetRemainingEnemies() const
{
	return m_enemiesRemaining;
}

// --- Function that returns the current wave ---
int WaveManager::GetCurrentWave() const
{
	return m_currentWave;
}

// --- Function that updates when an enemy has been spawned ---
void WaveManager::EnemySpawned()
{
	// Increment the amount of enemies spawned
	m_enemiesSpawned++;

	// Start the enemy spawn timer
	m_enemySpawnTimer.Start();
}

// --- Function that updates when an enemy has been defeated ---
void WaveManager::EnemyDefeated()
{
	// Decrement the amount of enemies remain
	m_enemiesRemaining--;
}

// --- Main update function ---
void WaveManager::Update(float deltaTime)
{
	// --- Update timers if they are active
	if (m_waveDelayTimer.IsActive())
		m_waveDelayTimer.Update(deltaTime);

	if (m_enemySpawnTimer.IsActive())
		m_enemySpawnTimer.Update(deltaTime);
}
