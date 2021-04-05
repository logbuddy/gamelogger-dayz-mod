class LogbuddyPayloadObject
{
    ref map<string, string> m_PayloadObjectItems;

    void LogbuddyPayloadObject(string objtype, string objref)
    {
        m_PayloadObjectItems = new map<string, string>;

        m_PayloadObjectItems["objtype"] = objtype;
        m_PayloadObjectItems["objref"] = objref;
    }

    void AddItem(string Key, string Value)
    {
        m_PayloadObjectItems[Key] = Value;
    }

    string AsJsonString()
    {
        string json = "{";
        int i = 0;
        foreach (string Key, string Value: m_PayloadObjectItems)
        {
            if (i > 0) json = json + ",";
            json = json + "\"" + Key + "\": \"" + Value + "\"";
            i++;
        }
        json = json + "}";

        return json;
    }
}
