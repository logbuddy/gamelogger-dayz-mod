modded class PluginAdminLog
{
    override void LogPrint(string message)
    {
        Logger logger = GetModuleManager().GetModule(Logger);
        logger.Ingest("AdminLog", message);

        super.LogPrint(message);
    }
}