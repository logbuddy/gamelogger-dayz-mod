class GameLogger
{
    static ref GameLogger m_instance;

    static void Ingest(string modname, string message)
    {
        Print("[GameLogger] " + message);

        if(GameLogger.m_instance == NULL) GameLogger.m_instance = new GameLogger();

        switch(modname)
        {
            case "Trader":
            {
                if(message.Substring(0, 18) == "[TRADER] Player: (")
                {
                    GameLogger.m_instance.Trader(message.Substring(9, message.Length() - 9));
                }

                break;
            }
        }
    }

    void Trader(string message)
    {
        Print("[GameLogger] Trader sale: " + message);

        JsonObject json = new JsonObject;

        json.AddString("mod", "trader");
        json.AddString("action", "buy");
        json.AddString("message", message);

        RestCallback cbxcb = new RestCallback;
        RestContext ctx = GetRestApi().GetRestContext("https://webhook.site/5275dbb6-93ab-47e7-90e2-2a5d587ddf08");
        ctx.POST(cbxcb, "", json.GetJson());
    }
}