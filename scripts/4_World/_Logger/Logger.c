class Logger
{
    protected static ref Logger s_Instance;

    static const string m_ProfilePath = "$profile:Logger";
    static const string m_ConfigFile = m_ProfilePath + "/LoggerConfig.json";

    ref array<ref LoggerEvent> m_Events;
    ref LoggerSettings m_Settings;
    ref LoggerLogger m_Log;

    static const int EVENTS_PER_SECOND = 5;

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
            GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.Uploader, 1000, false);
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
    
    void Ingest(string source, LoggerPayload loggerPayload)
    {
        m_Log.Log("Ingest:  " + source + ", " + loggerPayload.AsJsonString());

        LoggerEvent loggerEvent = new LoggerEvent();
        loggerEvent.createdAt = LoggerHelper.GetTimestamp();
        loggerEvent.source = source;
        loggerEvent.payload = loggerPayload;

        m_Log.Log("Inserting " + loggerEvent + " into main events array during ingest");
        m_Events.Insert(loggerEvent);
    }

    void Uploader()
    {
        if (m_UploaderRunning) return;
        m_UploaderRunning = true;

        m_Log.Log("Got " + m_Events.Count() + " events delivered to Uploader");

        LoggerSendContainer loggerSendContainer = new LoggerSendContainer();
        LoggerEvent loggerEvent;
        int numObjects = 0;

        for (int i = 0; i < m_Events.Count(); i++)
        {
            loggerEvent = m_Events.Get(0);
            m_Log.Log("Got event " + loggerEvent + " during uploader");
            loggerSendContainer.InsertEvent(loggerEvent);
            m_Events.RemoveOrdered(0);

            numObjects++;

            if (numObjects >= Logger.EVENTS_PER_SECOND)
            {
                break;
            }
        }

        if (numObjects)
        {
            this.SendData(loggerSendContainer);
        }

        m_UploaderRunning = false;
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.Uploader, 1000, false);
    }

    void SendData(LoggerSendContainer loggerSendContainer)
    {
        string jsonString = loggerSendContainer.AsJsonString();

        m_Log.Log("SendData jsonString: " + jsonString);

        LoggerRestCallback cbxcb = new LoggerRestCallback;
        cbxcb.SetLogger(this);
        
        RestContext ctx = GetRestApi().GetRestContext("https://rs213s9yml.execute-api.eu-central-1.amazonaws.com/server-events");
        ctx.POST(cbxcb, "", jsonString);
    }
}
