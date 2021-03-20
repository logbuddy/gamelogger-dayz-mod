/*
modded class PluginAdminLog
{
    override void LogPrint(string message)
    {
        //Logger logger = GetModuleManager().GetModule(Logger);
        Logger logger = Logger.GetInstance();
        logger.Ingest("AdminLog", message);

        super.LogPrint(message);
    }
}
*/