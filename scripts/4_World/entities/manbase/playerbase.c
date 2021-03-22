modded class PlayerBase
{
    ref Logger m_Logger;

    void PlayerBase()
    {
        if(!m_Logger) m_Logger = Logger.GetInstance();
    }

    override void EEKilled(Object killer)
    {
        Log("Starting PlayerBase.EEKilled");

        LoggerPayload loggerPayload = new LoggerPayload();
        loggerPayload.AddPlayer(this, "victim");

        loggerPayload.AddActionItem("victim", "victim");
        loggerPayload.AddActionItem("position", player.GetPosition().ToString());

        string cause = "(unknown)";
        string tool = "(unknown)";

        if (killer)
        {
            if (this == killer)
            {
                cause = "NATURAL";
            }
            else if (killer.IsWeapon() || killer.IsMeleeWeapon())
            {
                cause = "PLAYER";

                PlayerBase killerPlayer = PlayerBase.Cast(EntityAI.Cast(killer).GetHierarchyParent());
                loggerPayload.AddPlayer(killerPlayer, "killer");
                loggerPayload.AddActionItem("killer", "killer");

                tool = killer.GetDisplayName();

                if(!killer.IsMeleeWeapon())
                {
                    float distance = vector.Distance(GetPosition(), killerPlayer.GetPosition());
                    loggerPayload.AddActionItem("distance", distance.ToString());
                }
            }
            else
            {
                cause = "OTHER";
                tool = killer.GetType();
            }
        }

        loggerPayload.AddActionItem("cause", cause);
        loggerPayload.AddActionItem("tool", tool);

        m_Logger.Ingest("PlayerKilled", loggerPayload);

        super.EEKilled(killer);
    }
}