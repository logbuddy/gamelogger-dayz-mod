/*
modded class Debug
{
    override static void ActionLog(string message = LOG_DEFAULT, string plugin = LOG_DEFAULT, string author = LOG_DEFAULT, string label = LOG_DEFAULT, string entity = LOG_DEFAULT)
    {
        Logger logger = Logger.GetInstance();
        logger.Ingest("ActionLog", message);
    }

    override static void InventoryLog(string message = LOG_DEFAULT, string plugin = LOG_DEFAULT, string author = LOG_DEFAULT, string label = LOG_DEFAULT, string entity = LOG_DEFAULT)
    {
        Logger logger = Logger.GetInstance();
        logger.Ingest("InventoryLog", message);
    }

    override static void SymptomLog(string message = LOG_DEFAULT, string plugin = LOG_DEFAULT, string author = LOG_DEFAULT, string label = LOG_DEFAULT, string entity = LOG_DEFAULT)
    {
        Logger logger = Logger.GetInstance();
        logger.Ingest("SymptomLog", message);
    }
}
*/