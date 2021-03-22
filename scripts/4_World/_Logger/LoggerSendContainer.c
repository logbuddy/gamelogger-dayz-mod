class LoggerSendContainer
{
    string m_UserId;
    string m_ServerId;
    string m_ApiKeyId

    ref array<ref LoggerEvent> m_Events;

    Logger m_Logger;

    void LoggerSendContainer()
    {
        Logger m_Logger = Logger.GetInstance();
        LoggerSettings Settings = m_Logger.GetSettings();

        m_UserId = Settings.GetUserId();
        m_ServerId = Settings.GetServerId();
        m_ApiKeyId = Settings.GetApiKeyId();

        m_Events = new array<ref LoggerEvent>;
    }

    void InsertEvent(LoggerEvent loggerEvent)
    {
        if(!m_Logger) m_Logger = Logger.GetInstance();
        if(!m_Events) m_Events = new array<ref LoggerEvent>;

        m_Logger.m_Log.Log("Inserting " + loggerEvent + " into LoggerSendContainer");
        m_Events.Insert(loggerEvent);
    }

    string AsJsonString()
    {
        m_Logger.m_Log.Log("Found " + m_Events.Count() + " event(s) to convert");

        string json = "{";

        json = json + "\"userId\": \"" + m_UserId + "\",";
        json = json + "\"serverId\": \"" + m_ServerId + "\",";
        json = json + "\"apiKeyId\": \"" + m_ApiKeyId + "\",";

        json = json + "\"events\": [";
        string payload;

        foreach(int i, LoggerEvent Event: m_Events)
        {
            if(i > 0) json = json + ",";

            json = json + "{";
                json = json + "\"createdAt\": \"" + Event.createdAt + "\",";
                json = json + "\"source\": \"" + Event.source + "\",";
                json = json + "\"payload\": "+ Event.payload.AsJsonString();
            json = json + "}";
        }
        json = json + "]";

        json = json + "}";
        return json;
    }
}
