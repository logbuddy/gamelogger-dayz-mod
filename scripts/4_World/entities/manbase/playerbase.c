modded class PlayerBase
{
    ref Logger m_Logger;

    void PlayerBase()
    {
        if(!m_Logger) m_Logger = Logger.GetInstance();
    }

    void LoggerOnConnect(string Mode)
    {
        Log("Starting PlayerBase.LoggerOnConnect");

        LoggerPayload loggerPayload = new LoggerPayload();
        loggerPayload.AddPlayer(this, "joiner");

        loggerPayload.AddActionItem("joiner", "joiner");
        loggerPayload.AddActionItem("position", GetPosition().ToString());

        m_Logger.Ingest("Player" + Mode, loggerPayload);
    }

    override void OnConnect()
    {
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.LoggerOnConnect, 1000, false, "Connect");
        super.OnConnect();
    }

    override void OnReconnect()
    {
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.LoggerOnConnect, 1000, false, "Reconnect");
        super.OnReconnect();
    }

    override void EEKilled(Object killer)
    {
        Log("Starting PlayerBase.EEKilled");

        LoggerPayload loggerPayload = new LoggerPayload();
        loggerPayload.AddPlayer(this, "victim");

        loggerPayload.AddActionItem("victim", "victim");
        loggerPayload.AddActionItem("position", GetPosition().ToString());

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