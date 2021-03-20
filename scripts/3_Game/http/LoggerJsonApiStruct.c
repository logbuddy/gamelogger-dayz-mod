class LoggerJsonApiStruct extends JsonApiStruct
{
    Logger m_logger;

    void LoggerJsonApiStruct()
    {
        m_logger = Logger.GetInstance();
    }

    override void OnBufferReady()
    {
        m_logger.Log("OnBufferReady");
    }
}