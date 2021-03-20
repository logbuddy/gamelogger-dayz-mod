/*
modded class TraderMessage
{
    protected static bool Filter(string str)
    {
        if (str.Substring(0, 18) == "[TRADER] Player: (") return false;
        return true;
    }

    override static void ServerLog(string str)
    {
        Print(str); // should still go to the regular log
        if(TraderMessage.Filter(str)) return;

        LoggerJsonApiStruct json = new LoggerJsonApiStruct();
        json.StoreString("message", str);

        Logger l = Logger.GetInstance();
        l.Ingest("Trader", json);
    }
}
*/