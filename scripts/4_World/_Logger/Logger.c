class Logger
{
    protected static ref Logger s_Instance;

    static const string m_ProfilePath = "$profile:Logger";
    static const string m_ConfigFile = m_ProfilePath + "/LoggerConfig.json";

    ref array<ref LoggerEvent> m_Events;
    ref LoggerSettings m_Settings;
    ref LoggerLogger m_Log;

    const int LOGGER_POLL_FREQUENCY = 60000;

    static const int EVENTS_PER_CALL = 5;

    bool m_UploaderRunning = false;

    void Logger()
    {
        MakeDirectory(m_ProfilePath);
        MakeDirectory(LoggerLogger.m_LogPath);

        m_Events = new array<ref LoggerEvent>;
        m_Settings = new LoggerSettings(m_ConfigFile);
        m_Log = new LoggerLogger;

        if (!m_Settings.IsLoaded())
        {
            Print("[Logger] Settings incomplete. Please check $profile:Logger/LoggerConfig.json");
            Print("[Logger] If it didn't exist at all, an empty one was created right now.");
        }
        else
        {
            Print("[Logger] Ready.");
            GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.Uploader, LOGGER_POLL_FREQUENCY, false);
        }
    }

    static Logger GetInstance()
    {
        if(!s_Instance) s_Instance = new Logger();
        return s_Instance;
    }
    
    LoggerSettings GetSettings()
    {
        return m_Settings;
    }

    void InsertStatsEvent()
    {
        LoggerPayload Payload = new LoggerPayload();

        Payload.AddActionItem("eventBacklog", m_Events.Count().ToString());
        Payload.AddActionItem("fps", GetGame().GetFps().ToString());

        array<Man> Men;
        GetGame().GetPlayers(Men);

        PlayerBase player;
        LoggerPayloadObject PayloadObject;

        if (Men)
        {
            foreach (int i, Man localMan: Men)
            {
                if (Class.CastTo(player, localMan))
                {
                    PayloadObject = new LoggerPayloadObject("player", "player" + i.ToStringLen(4));

                    PayloadObject.AddItem("id", player.GetIdentity().GetPlainId());
                    PayloadObject.AddItem("name", player.GetIdentity().GetName());
                    PayloadObject.AddItem("position", player.GetPosition().ToString());

                    Payload.m_LoggerPayloadObjects.Insert(PayloadObject);
                    Payload.AddActionItem("player", "player" + i.ToStringLen(4));
                }
            }
        }

        Ingest("Stats", Payload);
    }

    void Ingest(string Source, LoggerPayload Payload)
    {
        m_Log.Log("Ingest:  " + Source + ", " + Payload.AsJsonString());

        LoggerEvent Event = new LoggerEvent();
        Event.createdAt = LoggerHelper.GetTimestamp();
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

        LoggerSendContainer loggerSendContainer = new LoggerSendContainer();
        InsertStatsEvent();
        LoggerEvent loggerEvent;

        int numObjects = 0;

        for (int i = 0; i < m_Events.Count(); i++)
        {
            loggerEvent = m_Events.Get(0);
            m_Log.Log("Got event " + loggerEvent + " during uploader");
            loggerSendContainer.InsertEvent(loggerEvent);
            m_Events.RemoveOrdered(0);

            numObjects++;
            
            if (numObjects >= Logger.EVENTS_PER_CALL)
            {
                break;
            }
        }

        if (numObjects)
        {
            this.SendData(loggerSendContainer);
        }

        m_UploaderRunning = false;
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.Uploader, LOGGER_POLL_FREQUENCY, false);
    }

    void SendData(LoggerSendContainer SendContainer)
    {
        string JsonString = SendContainer.AsJsonString();

        m_Log.Log("SendData JsonString: " + JsonString);

        LoggerRestCallback Cbxcb = new LoggerRestCallback;
        Cbxcb.SetLogger(this);
        
        RestContext Ctx = GetRestApi().GetRestContext("https://rs213s9yml.execute-api.eu-central-1.amazonaws.com/server-events");
        Ctx.POST(Cbxcb, "", JsonString);
    }
}
