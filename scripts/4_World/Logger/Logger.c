class Logger : JMModuleBase
{
    protected static ref Logger s_Instance;

    static const string m_ProfilePath = "$profile:Logger";
    static const string m_ConfigFile = m_ProfilePath + "/LoggerConfig.json";

    static const string m_LogPath = m_ProfilePath + "/log";
    static const string m_LogFile = m_LogPath + "/main.txt";
    static const string m_NotSentLogFile = m_LogPath + "/notsent.txt";

    static const string m_QueuePath = m_ProfilePath + "/queue";
    static const string m_CreatePath = m_QueuePath + "/create";
    static const string m_UploadPath = m_QueuePath + "/upload";
    static const string m_InProgressPath = m_QueuePath + "/inprogress";
    static const string m_DonePath = m_QueuePath + "/done";
    
    static ref LoggerSettings m_Settings;

    static const string m_UserId;
    static const string m_ApiKeyId;
    static const string m_ServerId;


    void Logger()
    {
        s_Instance = this;

        MakeDirectory(m_ProfilePath);

        MakeDirectory(m_LogPath);
        MakeDirectory(m_QueuePath);

        MakeDirectory(m_CreatePath);
        MakeDirectory(m_UploadPath);
        MakeDirectory(m_InProgressPath);
        MakeDirectory(m_DonePath);

        m_Settings = new LoggerSettings(m_ConfigFile);

        if (!m_Settings.IsLoaded())
        {
            Print("[Logger] Settings incomplete. Please check $profile:Logger/LoggerConfig.json");
            Print("[Logger] If it didn't exist at all, an empty one was created right now.");
        }
        else
        {
            GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.LogReader, 1000, false);
            GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.Uploader, 1000, false);
        }
    }

    static Logger GetInstance()
    {
        if (NULL == s_Instance) s_Instance = new Logger;
        return s_Instance;
    }

    override bool IsClient()
    {
        return false;
    }

    void Log(string message)
    {
        FileHandle file = OpenFile(m_LogFile, FileMode.APPEND);

        if (file)
        {
            string timestamp = this.GetTimestamp();
            FPrintln(file, "[" + timestamp + "] " + message);
            CloseFile(file);
        }
        else
        {
            Print("[LOGGER] Could not create logfile " + m_LogFile);
        }
    }

    string GetTimestamp()
    {
        int year = 0;
        int month = 0;
        int day = 0;
        int hour = 0;
        int minute = 0;
        int second = 0;

        GetHourMinuteSecondUTC(hour, minute, second);
        GetYearMonthDayUTC(year, month, day);

        return year.ToStringLen(4) + "-" + month.ToStringLen(2) + "-" + day.ToStringLen(2) + "T" + hour.ToStringLen(2) + ":" + minute.ToStringLen(2) + ":" + second.ToStringLen(2) + "Z";
    }

    string GetTimestampForFilename()
    {
        string timestamp = this.GetTimestamp();
        timestamp.Replace(":", "");

        return timestamp;
    }
    
    bool Filter(string source, string payload)
    {
        return false;
    }

    void Ingest(string source, string payload)
    {
        if (this.Filter(source, payload))
        {
            this.Log("NOTSENT [" + source + "] " + payload);
            return;
        }

        //this.Log("Ingest:  " + source + ", " + payload);
        JsonObject json = new JsonObject;

        json.AddString("source", source);
        json.AddString("payload", payload);
        json.AddString("createdAt", this.GetTimestamp());

        this.AddToUploadQueue(json);
    }

    void AddToUploadQueue(JsonObject json)
    {
        //this.Log("AddToUploadQueue: " + json);

        string fileDate = this.GetTimestampForFilename();
        string fileName = "upload_" + fileDate + "_" + Math.RandomInt(0, 1000000) + ".json";
        string filePath = m_CreatePath + "/" + fileName;

        FileHandle file = OpenFile(filePath, FileMode.WRITE);

        if (file)
        {
            FPrint(file, json.GetJson());
            CloseFile(file);

            CopyFile(m_CreatePath + "/" + fileName, m_UploadPath + "/" + fileName);
            DeleteFile(m_CreatePath + "/" + fileName);
        }
        else
        {
            this.Log("Could not create file " + filePath);
        }
    }

    void LogReader()
    {
        this.Log("Called LogReader...");
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.LogReader, 60000, false);
    }

    void Uploader()
    {
        this.Log("Called Uploader...");

        FileHandle fileHandle;
        string fileName;
        FileAttr fileAttributes;
        array<string> fileList = new array<string>;

        FindFileHandle findFileHandle = FindFile(m_UploadPath + "/*.json", fileName, fileAttributes, FindFileFlags.ALL);

        if(fileName)
        {
            fileList.Insert(fileName);
            CopyFile(m_UploadPath + "/" + fileName, m_InProgressPath + "/" + fileName);
            DeleteFile(m_UploadPath + "/" + fileName);
        }

        fileName = "";

        while (FindNextFile(findFileHandle, fileName, fileAttributes))
        {
            if (fileName)
            {
                fileList.Insert(fileName);
                CopyFile(m_UploadPath + "/" + fileName, m_InProgressPath + "/" + fileName);
                DeleteFile(m_UploadPath + "/" + fileName);
            }
        }

        CloseFindFile(findFileHandle);

        if (fileList.Count() == 0) 
        {
            GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.Uploader, 60000, false);
            return;
        }

        this.Log("The Uploader has found " + fileList.Count() + " events to log");

        string payload;
        string payloadFile;
        string readLine;

        int objectsPerRequest = 8;
        int numObjects = 0;

        while (fileList.Count())
        {
            payloadFile = fileList.Get(0);
            numObjects++;

            //this.Log("Processing file " + payloadFile);
            
            fileHandle = OpenFile(m_InProgressPath + "/" + payloadFile, FileMode.READ);
            while (FGets(fileHandle, readLine) > 0)
            {
                payload = payload + readLine + ",";
            }
            CloseFile(fileHandle);

            CopyFile(m_InProgressPath + "/" + payloadFile, m_DonePath + "/" + payloadFile);
            DeleteFile(m_InProgressPath + "/" + payloadFile);

            fileList.RemoveOrdered(0);

            if (numObjects >= objectsPerRequest)
            {
                this.SendData(payload);
                payload = "";
                numObjects = 0;
            }
        }

        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.Uploader, 60000, false);
    }

    void SendData(string payload)
    {
        payload = payload.Substring(0, payload.Length() - 1);
        payload = "[" + payload  + "]";
        //this.Log("SendData payload: " + payload);

        JsonObject json = new JsonObject;

        json.AddString("userId", m_Settings.GetUserId());
        json.AddString("serverId", m_Settings.GetServerId());
        json.AddString("apiKeyId", m_Settings.GetApiKeyId());

        json.AddString("events", "REPLACE");

        string jsonstring = json.GetJson();
        //this.Log("SendData jsonstring: " + jsonstring);

        jsonstring.Replace("\"REPLACE\"", payload);

        //this.Log("Doing call with " + jsonstring);

        LoggerRestCallback cbxcb = new LoggerRestCallback;
        cbxcb.SetLogger(this);
        
        RestContext ctx = GetRestApi().GetRestContext("https://rs213s9yml.execute-api.eu-central-1.amazonaws.com/server-events");
        ctx.POST(cbxcb, "", jsonstring);
    }
}
