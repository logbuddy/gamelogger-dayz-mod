class LoggerSendContainer
{
    string userId;
    string serverId;
    string apiKeyId
    ref array<ref LoggerEvent> events;

    Logger m_logger;

    void LoggerSendContainer()
    {
        Logger m_logger = Logger.GetInstance();
        LoggerSettings settings = m_logger.GetSettings();

        userId = settings.GetUserId();
        serverId = settings.GetServerId();
        apiKeyId = settings.GetApiKeyId();

        events = new array<ref LoggerEvent>;
    }

    void InsertEvent(LoggerEvent loggerEvent)
    {
        if(!m_logger) m_logger = Logger.GetInstance();
        if(!events) events = new array<ref LoggerEvent>;

        m_logger.Log("Inserting " + loggerEvent + " into LoggerSendContainer");
        events.Insert(loggerEvent);
    }

    string AsJsonString()
    {
        m_logger.Log("Found " + events + " event(s) to convert");

        string json = "{";

        json = json + "\"userId\": \"" + userId + "\",";
        json = json + "\"serverId\": \"" + serverId + "\",";
        json = json + "\"apiKeyId\": \"" + apiKeyId + "\",";

        json = json + "\"events\": [";
        string payload;

        int backslash = 92;
        int quote = 34;

        foreach(int i, LoggerEvent loggerEvent: events)
        {
            if(i > 0) json = json + ",";

            payload = loggerEvent.payload.GetJson();
            payload.Replace("\"", backslash.AsciiToString() + quote.AsciiToString());

            json = json + "{";
                json = json + "\"createdAt\": \"" + loggerEvent.createdAt + "\",";
                json = json + "\"source\": \"" + loggerEvent.source + "\",";
                json = json + "\"payload\": \"" + payload + "\"";
            json = json + "}";
        }
        json = json + "]";

        json = json + "}";
        return json;
    }
}

class LoggerEvent
{
    string createdAt;
    string source;
    ref LoggerPayload payload;
}

class LoggerPayload extends JsonObject {}
