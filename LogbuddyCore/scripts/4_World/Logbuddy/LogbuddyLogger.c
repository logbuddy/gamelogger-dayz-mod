class LogbuddyLogger
{
    static const string m_LogPath = LogbuddyCore.m_ProfilePath + "/log";
    static const string m_LogFile = m_LogPath + "/main.txt";
    static const string m_NotSentLogFile = m_LogPath + "/notsent.txt";

    void Log(string message, LogType logType = LogType.MAIN)
    {
        string logFile;

        switch (logType)
        {
        case LogType.NOTSENT:
        {
            logFile = LogbuddyLogger.m_NotSentLogFile;
            break;
        }

        default:
        {
            logFile = LogbuddyLogger.m_LogFile;
        }
        }

        FileHandle file = OpenFile(logFile, FileMode.APPEND);

        if (file)
        {
            string timestamp = LogbuddyHelper.GetTimestamp();
            FPrintln(file, "[" + timestamp + "] " + message.Trim());
            CloseFile(file);
        }
        else
        {
            Print("[Logbuddy] Could not create logfile " + m_LogFile);
        }
    }
}
