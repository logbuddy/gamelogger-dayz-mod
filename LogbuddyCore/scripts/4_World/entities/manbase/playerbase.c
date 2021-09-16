/* modded class PlayerBase
{
    ref LogbuddyCore m_LogbuddyCore;

    void PlayerBase()
    {
        if(!m_LogbuddyCore) m_LogbuddyCore = LogbuddyCore.GetInstance();
    }

    void LogbuddyOnConnect(string Mode)
    {
        Log("Starting PlayerBase.LogbuddyOnConnect");

        LogbuddyPayload Payload = new LogbuddyPayload();
        Payload.AddPlayer(this, "joiner");

        Payload.AddActionItem("joiner", "joiner");
        Payload.AddActionItem("position", GetPosition().ToString());

        m_LogbuddyCore.Ingest("Player" + Mode, Payload);
    }

    override void OnConnect()
    {
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.LogbuddyOnConnect, 1000, false, "Connect");
        super.OnConnect();
    }

    override void OnReconnect()
    {
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.LogbuddyOnConnect, 1000, false, "Reconnect");
        super.OnReconnect();
    }

    override void EEKilled(Object killer)
    {
        Log("Starting PlayerBase.EEKilled");

        LogbuddyPayload Payload = new LogbuddyPayload();
        Payload.AddPlayer(this, "victim");

        Payload.AddActionItem("victim", "victim");
        Payload.AddActionItem("position", GetPosition().ToString());

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
                Payload.AddPlayer(killerPlayer, "killer");
                Payload.AddActionItem("killer", "killer");

                tool = killer.GetDisplayName();

                if(!killer.IsMeleeWeapon())
                {
                    float distance = vector.Distance(GetPosition(), killerPlayer.GetPosition());
                    Payload.AddActionItem("distance", distance.ToString());
                }
            }
            else
            {
                cause = "OTHER";
                tool = killer.GetType();
            }
        }

        Payload.AddActionItem("cause", cause);
        Payload.AddActionItem("tool", tool);

        m_LogbuddyCore.Ingest("PlayerKilled", Payload);

        super.EEKilled(killer);
    }
} */