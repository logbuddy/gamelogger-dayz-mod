class LoggerRestCallback extends RestCallback
{
    ref Logger m_logger;

    void SetLogger(Logger logger)
    {
        m_logger = logger;
    }

    override void OnError(int errorCode)
    {
        m_logger.Log("RestCallback OnError, code " + errorCode);
    };

    override void OnTimeout()
    {
        m_logger.Log("RestCallback OnTimeout");
    };

    override void OnSuccess(string data, int dataSize)
    {
        m_logger.Log("RestCallback OnSuccess size=" + dataSize);
    };

    override void OnFileCreated(string fileName, int dataSize)
    {
        m_logger.Log("RestCallback OnFileCreated, file=" + fileName + " size=" + dataSize);
    };
}
