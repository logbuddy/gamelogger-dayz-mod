class LoggerPayload
{
    Logger m_Logger;

    ref array<ref LoggerPayloadObject> m_LoggerPayloadObjects;
    ref map<string, string> m_LoggerActionItems;

    void LoggerPayload()
    {
        m_Logger = Logger.GetInstance();

        m_LoggerPayloadObjects = new array<ref LoggerPayloadObject>;
        m_LoggerActionItems = new map<string, string>();
    }

    void AddActionItem(string Key, string Value)
    {
        m_LoggerActionItems[Key] = Value;
    }

    void AddPlayer(PlayerBase player, string objref)
    {
        LoggerPayloadObject PayloadObject = new LoggerPayloadObject("player", objref);

        PayloadObject.AddItem("id", player.GetIdentity().GetPlainId());
        PayloadObject.AddItem("name", player.GetIdentity().GetName());

        m_LoggerPayloadObjects.Insert(PayloadObject);
    }

    void AddCar(CarScript Vehicle, string objref)
    {
        LoggerPayloadObject PayloadObject = new LoggerPayloadObject("vehicle", objref);

        PayloadObject.AddItem("name", Vehicle.ClassName());
        m_LoggerPayloadObjects.Insert(PayloadObject);
    }

    string AsJsonString()
    {
        string json = "{";

        json = json + "\"objects\": [";
        foreach (int i, LoggerPayloadObject PayloadObject: m_LoggerPayloadObjects)
        {
            if (i > 0) json = json + ",";
            json = json + PayloadObject.AsJsonString();
        }
        json = json + "],";

        json = json + "\"action\": {";
        int j = 0;
        foreach (string Key, string Value: m_LoggerActionItems)
        {
            if (j > 0) json = json + ",";
            json = json + "\"" + Key + "\": " + "\"" + Value +"\"";
            j++;
        }
        json = json + "}";

        json = json + "}";
        return json;
    }
}
