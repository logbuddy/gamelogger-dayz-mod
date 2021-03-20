enum LogType
{
    MAIN,
    NOTSENT
};

class Logger
{
    protected static ref Logger s_Instance;

    static const string m_ProfilePath = "$profile:Logger";
    static const string m_ConfigFile = m_ProfilePath + "/LoggerConfig.json";

    static const string m_LogPath = m_ProfilePath + "/log";
    static const string m_LogFile = m_LogPath + "/main.txt";
    static const string m_NotSentLogFile = m_LogPath + "/notsent.txt";

    static ref LoggerSettings m_Settings;

    static const string m_UserId;
    static const string m_ApiKeyId;
    static const string m_ServerId;

    ref array<ref LoggerEvent> m_Events;
    static const int EVENTS_PER_SECOND = 5;

    bool m_UploaderRunning = false;

    void Logger()
    {
        m_Events = new array<ref LoggerEvent>;
        m_Settings = new LoggerSettings(m_ConfigFile);

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

    void Log(string message, LogType logType = LogType.MAIN)
    {
        string logFile;

        switch(logType)
        {
            case LogType.NOTSENT:
            {
                logFile = this.m_NotSentLogFile;
                break;
            }

            default:
            {
                logFile = this.m_LogFile;
            }
        }

        FileHandle file = OpenFile(logFile, FileMode.APPEND);

        if (file)
        {
            string timestamp = this.GetTimestamp();
            FPrintln(file, "[" + timestamp + "] " + message.Trim());
            CloseFile(file);
        }
        else
        {
            Print("[LOGGER] Could not create logfile " + m_LogFile);
        }
    }

    LoggerSettings GetSettings()
    {
        return m_Settings;
    }

    string GetTimestamp()
    {
        int year = 0;
        int month = 0;
        int day = 0;
        int hour = 0;
        int minute = 0;
        int second = 0;

        GetHourMinuteSecondUTC(hour, minute, second);
        GetYearMonthDayUTC(year, month, day);

        return year.ToStringLen(4) + "-" + month.ToStringLen(2) + "-" + day.ToStringLen(2) + "T" + hour.ToStringLen(2) + ":" + minute.ToStringLen(2) + ":" + second.ToStringLen(2) + "Z";
    }

    void Ingest(string source, LoggerPayload loggerPayload)
    {
        this.Log("Ingest:  " + source + ", " + loggerPayload.GetJson());

        LoggerEvent loggerEvent = new LoggerEvent();
        loggerEvent.createdAt = this.GetTimestamp();
        loggerEvent.source = source;
        loggerEvent.payload = loggerPayload;

        this.Log("Inserting " + loggerEvent + " into main events array during ingest");
        m_Events.Insert(loggerEvent);
    }

    void Uploader()
    {
        if (m_UploaderRunning) return;
        m_UploaderRunning = true;

        Log("Got " + m_Events.Count() + " events delivered to Uploader");

        LoggerSendContainer loggerSendContainer = new LoggerSendContainer();
        LoggerEvent loggerEvent;
        int numObjects = 0;

        for (int i = 0; i < m_Events.Count(); i++)
        {
            loggerEvent = m_Events.Get(0);
            this.Log("Got event " + loggerEvent + " during uploader");
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

        this.Log("SendData jsonString: " + jsonString);

        LoggerRestCallback cbxcb = new LoggerRestCallback;
        cbxcb.SetLogger(this);
        
        RestContext ctx = GetRestApi().GetRestContext("https://rs213s9yml.execute-api.eu-central-1.amazonaws.com/server-events");
        ctx.POST(cbxcb, "", jsonString);
    }
}