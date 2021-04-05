class LogbuddyPayloadObject
{
    ref map<string, string> m_LogbuddyPayloadObjectItems;

    void LogbuddyPayloadObject(string objtype, string objref)
    {
        m_LogbuddyPayloadObjectItems = new map<string, string>;

        m_LogbuddyPayloadObjectItems["objtype"] = objtype;
        m_LogbuddyPayloadObjectItems["objref"] = objref;
    }

    void AddItem(string Key, string Value)
    {
        m_LogbuddyPayloadObjectItems[Key] = Value;
    }

    string AsJsonString()
    {
        string json = "{";
        int i = 0;
        foreach (string Key, string Value: m_LogbuddyPayloadObjectItems)
        {
            if (i > 0) json = json + ",";
            json = json + "\"" + Key + "\": \"" + Value + "\"";
            i++;
        }
        json = json + "}";

        return json;
    }
}
