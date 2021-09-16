class LogbuddyCore
{
    protected static ref LogbuddyCore s_Instance;

    static const string m_ProfilePath = "$profile:Logbuddy";
    static const string m_ConfigFile = m_ProfilePath + "/LogbuddyConfig.json";

    ref array<ref LogbuddyEvent> m_Events;
    ref LogbuddySettings m_Settings;
    ref LogbuddyLogger m_Log;

    bool m_UploaderRunning = false;

    void LogbuddyCore()
    {
        MakeDirectory(m_ProfilePath);
        MakeDirectory(LogbuddyLogger.m_LogPath);
        DeleteFile(LogbuddyLogger.m_LogFile);

        m_Events = new array<ref LogbuddyEvent>;
        m_Log = new LogbuddyLogger();
        
        m_Settings = LogbuddySettings.GetInstance(m_ConfigFile, this);
        m_Log.LogbuddySetSettings(m_Settings);

        if (!m_Settings.IsLoaded())
        {
            Print("[Logbuddy] Settings incomplete. Please check $profile:Logbuddy/LogbuddyConfig.json");
            Print("[Logbuddy] If it didn't exist at all, an empty one was created right now.");
        }
        else
        {
            Print("[Logbuddy] Ready.");
            GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.Uploader, m_Settings.GetLogbuddyPollFrequencyMinutes() * 1000 * 60, false);
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

    void Ingest(string logbuddyCollection, string logbuddySource, LogbuddyPayload logbuddyPayload)
    {
        if(!this.m_Settings.GetActive()) {
            m_Log.Log("Logbuddy inactive", LogbuddyLogger.LOGLEVEL_DEBUG);
            return;
        }

        m_Log.Log("Ingest:  " + logbuddyCollection + ", " + logbuddySource + ", " + logbuddyPayload.AsJsonString(), LogbuddyLogger.LOGLEVEL_DEBUG);

        LogbuddyEvent logbuddyEvent = new LogbuddyEvent();
        logbuddyEvent.createdAt = LogbuddyHelper.GetTimestamp();
        logbuddyEvent.source = logbuddySource;
        logbuddyEvent.collection = logbuddyCollection;
        logbuddyEvent.payload = logbuddyPayload;

        m_Log.Log("Inserting " + logbuddyEvent + " into main events array during ingest", LogbuddyLogger.LOGLEVEL_DEBUG);
        m_Events.Insert(logbuddyEvent);
    }

    void Uploader()
    {
        if(!this.m_Settings.GetActive()) {
            m_Log.Log("Logbuddy inactive", LogbuddyLogger.LOGLEVEL_DEBUG);
            GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.Uploader, m_Settings.GetLogbuddyPollFrequencyMinutes() * 1000 * 60, false);
            return;
        }

        if (m_UploaderRunning) return;
        m_UploaderRunning = true;

        m_Log.Log("Got " + m_Events.Count() + " events delivered to Uploader, max poll amount " + m_Settings.GetLogbuddyMaxEventsPerPoll(), LogbuddyLogger.LOGLEVEL_DEBUG);

        LogbuddySendContainer SendContainer = new LogbuddySendContainer();
        LogbuddyEvent Event;

        int numObjects = 0;
        int numEvents = m_Events.Count();

        for (int i = 0; i < numEvents; i++)
        {
            Event = m_Events.Get(0);

            m_Log.Log("Got event " + Event + " during uploader", LogbuddyLogger.LOGLEVEL_DEBUG);

            SendContainer.InsertEvent(Event);
            m_Events.RemoveOrdered(0);

            numObjects++;
            
            if (numObjects >= m_Settings.GetLogbuddyMaxEventsPerPoll())
            {
                break;
            }
        }

        if (numObjects)
        {            
            this.SendData(SendContainer);
        }

        if (m_Events.Count())
            m_Log.Log(m_Events.Count().ToString() + " event(s) left in queue, max send per poll " + m_Settings.GetLogbuddyMaxEventsPerPoll(), LogbuddyLogger.LOGLEVEL_QUEUE);

        m_UploaderRunning = false;
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.Uploader, m_Settings.GetLogbuddyPollFrequencyMinutes() * 1000 * 60, false);
    }

    void SendData(LogbuddySendContainer SendContainer)
    {
        string JsonString = SendContainer.AsJsonString();

        m_Log.Log("SendData JsonString: " + JsonString, LogbuddyLogger.LOGLEVEL_DEBUG);

        LogbuddyRestCallback Cbxcb = new LogbuddyRestCallback;
        Cbxcb.SetLogbuddyCore(this);

        RestContext Ctx = GetRestApi().GetRestContext("http://127.0.0.1:8069");
        Ctx.SetHeader("application/json");
        Ctx.POST(Cbxcb, "", JsonString);
    }
}