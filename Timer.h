#pragma once
class Timer
{
public:
    Timer(float durationSeconds)
        : m_duration(durationSeconds), m_elapsed(0.f), m_active(false) {
    }

    void Start() { m_active = true; m_elapsed = 0.f; }
    void Stop() { m_active = false; }
    void Reset() { m_elapsed = 0.f; m_active = true; }

    void Update(float deltaTime)
    {
        if (m_active)
        {
            m_elapsed += deltaTime;
            if (m_elapsed >= m_duration)
            {
                m_active = false;
                m_elapsed = m_duration; // Clamp
            }
        }
    }

    bool IsFinished() const { return !m_active && m_elapsed >= m_duration; }
    bool IsActive() const { return m_active; }
    float GetElapsed() const { return m_elapsed; }
    float GetRemaining() const { return m_duration - m_elapsed; }

private:
    float m_duration;
    float m_elapsed;
    bool m_active;
};

