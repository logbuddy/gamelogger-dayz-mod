class LogbuddySendContainer
{
    ref array<ref LogbuddyEvent> m_Events;
    
    LogbuddyCore m_LogbuddyCore;
    LogbuddySettings m_LogbuddySettings;

    void LogbuddySendContainer()
    {
        m_LogbuddyCore = LogbuddyCore.GetInstance();
        m_LogbuddySettings = m_LogbuddyCore.GetSettings();

        m_Events = new array<ref LogbuddyEvent>;
    }

    void InsertEvent(LogbuddyEvent Event)
    {
        m_LogbuddyCore.m_Log.Log("Inserting " + Event + " into LogbuddySendContainer", LogbuddyLogger.LOGLEVEL_DEBUG);
        m_Events.Insert(Event);
    }

    string AsJsonString()
    {
        m_LogbuddyCore.m_Log.Log("Found " + m_Events.Count() + " event(s) to convert", LogbuddyLogger.LOGLEVEL_DEBUG);

        string json = "{";

        json = json + "\"database\": \"" + m_LogbuddySettings.GetLogbuddyDatabase() + "\",";
        json = json + "\"events\": [";

        string payload;

        foreach(int i, LogbuddyEvent Event: m_Events)
        {
            if(i > 0) json = json + ",";

            json = json + "{";
                json = json + "\"createdAt\": \"" + Event.createdAt + "\",";
                json = json + "\"source\": \"" + Event.source + "\",";
                json = json + "\"collection\": \"" + Event.collection + "\",";
                json = json + "\"payload\": "+ Event.payload.AsJsonString();
            json = json + "}";
        }
        json = json + "]";

        json = json + "}";
        return json;
    }
}
