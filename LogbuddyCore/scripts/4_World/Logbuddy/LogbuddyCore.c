class LogbuddyCore
{
    protected static ref LogbuddyCore s_Instance;

    static const string m_ProfilePath = "$profile:Logbuddy";
    static const string m_ConfigFile = m_ProfilePath + "/LogbuddyConfig.json";

    ref array<ref LogbuddyEvent> m_Events;
    ref LogbuddySettings m_Settings;
    ref LogbuddyLogger m_Log;

    const int LOGBUDDY_POLL_FREQUENCY = 60000;

    static const int EVENTS_PER_CALL = 10;

    bool m_UploaderRunning = false;

    void LogbuddyCore()
    {
        MakeDirectory(m_ProfilePath);
        MakeDirectory(LogbuddyLogger.m_LogPath);
        DeleteFile(LogbuddyLogger.m_LogFile);

        m_Events = new array<ref LogbuddyEvent>;
        m_Settings = new LogbuddySettings(m_ConfigFile);
        m_Log = new LogbuddyLogger;

        if (!m_Settings.IsLoaded())
        {
            Print("[Logbuddy] Settings incomplete. Please check $profile:Logbuddy/LogbuddyConfig.json");
            Print("[Logbuddy] If it didn't exist at all, an empty one was created right now.");
        }
        else
        {
            Print("[Logbuddy] Ready.");
            GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.Uploader, LOGBUDDY_POLL_FREQUENCY, false);
        }
    }

    static LogbuddyCore GetInstance()
    {
        if(!s_Instance) s_Instance = new LogbuddyCore();
        return s_Instance;
    }
    
    LogbuddySettings GetSettings()
    {
        return m_Settings;
    }

    LogbuddyEvent StatsEvent()
    {
        LogbuddyPayload Payload = new LogbuddyPayload();

        float fps = 1000 / ftime;

        Payload.AddActionItem("eventBacklog", m_Events.Count().ToString());
        Payload.AddActionItem("fps", fps.ToString());

        array<Man> Men = new array<Man>;
        GetGame().GetPlayers(Men);

        PlayerBase player;
        LogbuddyPayloadObject PayloadObject;

        if (Men)
        {
            foreach (int i, Man localMan: Men)
            {
                if (Class.CastTo(player, localMan))
                {
                    PayloadObject = new LogbuddyPayloadObject("player", "player" + i.ToStringLen(4));

                    PayloadObject.AddItem("id", player.GetIdentity().GetPlainId());
                    PayloadObject.AddItem("name", player.GetIdentity().GetName());
                    PayloadObject.AddItem("position", player.GetPosition().ToString());

                    Payload.m_PayloadObjects.Insert(PayloadObject);
                    Payload.AddActionItem("player", "player" + i.ToStringLen(4));
                }
            }
        }

        LogbuddyEvent Event = new LogbuddyEvent();
        Event.createdAt = LogbuddyHelper.GetTimestamp();
        Event.source = "Stats";
        Event.payload = Payload;

        return Event;
    }

    void Ingest(string Source, LogbuddyPayload Payload)
    {
        m_Log.Log("Ingest:  " + Source + ", " + Payload.AsJsonString());

        LogbuddyEvent Event = new LogbuddyEvent();
        Event.createdAt = LogbuddyHelper.GetTimestamp();
        Event.source = Source;
        Event.payload = Payload;

        m_Log.Log("Inserting " + Event + " into main events array during ingest");
        m_Events.Insert(Event);
    }

    void Uploader()
    {
        if (m_UploaderRunning) return;
        m_UploaderRunning = true;

        m_Log.Log("Got " + m_Events.Count() + " events delivered to Uploader");

        LogbuddySendContainer SendContainer = new LogbuddySendContainer();
        LogbuddyEvent Event;

        int numObjects = 0;

        for (int i = 0; i < m_Events.Count(); i++)
        {
            Event = m_Events.Get(0);
            m_Log.Log("Got event " + Event + " during uploader");
            SendContainer.InsertEvent(Event);
            m_Events.RemoveOrdered(0);

            numObjects++;
            
            if (numObjects >= LogbuddyCore.EVENTS_PER_CALL)
            {
                break;
            }
        }

        if (numObjects)
        {            
            this.SendData(SendContainer);
        }

        m_UploaderRunning = false;
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.Uploader, LOGBUDDY_POLL_FREQUENCY, false);
    }

    void SendData(LogbuddySendContainer SendContainer)
    {
        string JsonString = SendContainer.AsJsonString();

        m_Log.Log("SendData JsonString: " + JsonString);

        LogbuddyRestCallback Cbxcb = new LogbuddyRestCallback;
        Cbxcb.SetLogbuddyCore(this);

        RestContext Ctx = GetRestApi().GetRestContext("https://rs213s9yml.execute-api.eu-central-1.amazonaws.com/server-events");
        Ctx.POST(Cbxcb, "", JsonString);
    }
}
