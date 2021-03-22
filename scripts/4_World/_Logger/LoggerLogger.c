class LoggerLogger
{
    static const string m_LogPath = Logger.m_ProfilePath + "/log";
    static const string m_LogFile = m_LogPath + "/main.txt";
    static const string m_NotSentLogFile = m_LogPath + "/notsent.txt";

    void Log(string message, LogType logType = LogType.MAIN)
    {
        string logFile;

        switch (logType)
        {
        case LogType.NOTSENT:
        {
            logFile = LoggerLogger.m_NotSentLogFile;
            break;
        }

        default:
        {
            logFile = LoggerLogger.m_LogFile;
        }
        }

        FileHandle file = OpenFile(logFile, FileMode.APPEND);

        if (file)
        {
            string timestamp = LoggerHelper.GetTimestamp();
            FPrintln(file, "[" + timestamp + "] " + message.Trim());
            CloseFile(file);
        }
        else
        {
            Print("[LOGGER] Could not create logfile " + m_LogFile);
        }
    }
}
