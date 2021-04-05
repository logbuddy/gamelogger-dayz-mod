class LogbuddySendContainer
{
    string m_UserId;
    string m_ServerId;
    string m_ApiKeyId

    ref array<ref LogbuddyEvent> m_Events;

    LogbuddyCore m_LogbuddyCore;

    void LogbuddySendContainer()
    {
        LogbuddyCore m_LogbuddyCore = LogbuddyCore.GetInstance();
        LogbuddySettings Settings = m_LogbuddyCore.GetSettings();

        m_UserId = Settings.GetUserId();
        m_ServerId = Settings.GetServerId();
        m_ApiKeyId = Settings.GetApiKeyId();

        m_Events = new array<ref LogbuddyEvent>;
    }

    void InsertEvent(LogbuddyEvent Event)
    {
        if(!m_LogbuddyCore) m_LogbuddyCore = LogbuddyCore.GetInstance();
        if(!m_Events) m_Events = new array<ref LogbuddyEvent>;

        m_LogbuddyCore.m_Log.Log("Inserting " + Event + " into LogbuddySendContainer");
        m_Events.Insert(Event);
    }

    string AsJsonString()
    {
        m_LogbuddyCore.m_Log.Log("Found " + m_Events.Count() + " event(s) to convert");

        string json = "{";

        json = json + "\"userId\": \"" + m_UserId + "\",";
        json = json + "\"serverId\": \"" + m_ServerId + "\",";
        json = json + "\"apiKeyId\": \"" + m_ApiKeyId + "\",";

        json = json + "\"events\": [";
        string payload;

        foreach(int i, LogbuddyEvent Event: m_Events)
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
