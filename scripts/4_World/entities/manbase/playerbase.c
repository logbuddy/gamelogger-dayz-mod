modded class PlayerBase
{
    ref Logger m_logger;

    void PlayerBase()
    {
        if(!m_logger) m_logger = Logger.GetInstance();
    }

    override void EEKilled(Object killer)
    {
        super.EEKilled(killer);
    }
}