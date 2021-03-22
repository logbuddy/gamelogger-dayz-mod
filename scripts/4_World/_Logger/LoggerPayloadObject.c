class LoggerPayloadObject
{
    ref map<string, string> m_LoggerPayloadObjectItems;

    void LoggerPayloadObject(string objtype, string objref)
    {
        m_LoggerPayloadObjectItems = new map<string, string>;

        m_LoggerPayloadObjectItems["objtype"] = objtype;
        m_LoggerPayloadObjectItems["objref"] = objref;
    }

    void AddItem(string Key, string Value)
    {
        m_LoggerPayloadObjectItems[Key] = Value;
    }

    string AsJsonString()
    {
        string json = "{";
        int i = 0;
        foreach (string Key, string Value: m_LoggerPayloadObjectItems)
        {
            if (i > 0) json = json + ",";
            json = json + "\"" + Key + "\": \"" + Value + "\"";
            i++;
        }
        json = json + "}";

        return json;
    }
}
