class Logger : JMModuleBase
{
    protected static ref Logger s_Instance;

    static const string m_ProfilePath = "$profile:Logger";
    static const string m_UploadPath = m_ProfilePath + "/upload";
    static const string m_InProgressPath = m_ProfilePath + "/inprogress";
    static const string m_DonePath = m_ProfilePath + "/done";

    void Logger()
    {
        s_Instance = this;

        MakeDirectory(m_ProfilePath);
        MakeDirectory(m_UploadPath);
        MakeDirectory(m_InProgressPath);
        MakeDirectory(m_DonePath);

        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.LogReader, 1000, false);
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.Uploader, 1000, false);
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

    void Log(string message)
    {
        Print("[Logger][" + this.GetTimestamp() + "] " + message);
    }

    void Ingest(string source, string payload)
    {
        Log("Ingest:  " + source + ", " + payload);
        JsonObject json = new JsonObject;

        json.AddString("source", source);
        json.AddString("payload", payload);
        json.AddString("createdAt", this.GetTimestamp());

        this.AddToUploadQueue(json);
    }

    void AddToUploadQueue(JsonObject json)
    {
        Log("AddToUploadQueue: " + json);

        string fileDate = this.GetTimestampForFilename();
        string fileName = m_UploadPath + "/upload_" + fileDate + "_" + Math.RandomInt(0, 1000000) + ".json";

        FileHandle file = OpenFile(fileName, FileMode.WRITE);

        if (file)
        {
            FPrint(file, json.GetJson());
            CloseFile(file);
        }
        else
        {
            Log("Could not create file " + fileName);
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

        Log("fileList.Count(): " + fileList.Count());

        if (fileList.Count() == 0) 
        {
            GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.Uploader, 60000, false);
            return;
        }

        this.Log("The Uploader has found " + fileList.Count() + " events to log");

        string payload = "";
        string readLine = "";

        foreach(string payloadFile: fileList)
        {
            this.Log("Processing file " + payloadFile);

            fileHandle = OpenFile(m_InProgressPath + "/" + payloadFile, FileMode.READ);
            while (FGets(fileHandle, readLine) > 0)
            {
                payload = payload + readLine + ",";
            }
            CloseFile(fileHandle);

            CopyFile(m_InProgressPath + "/" + payloadFile, m_DonePath + "/" + payloadFile);
            DeleteFile(m_InProgressPath + "/" + payloadFile);
        }

        payload = payload.Substring(0, payload.Length() - 1);
        payload = "[" + payload  + "]";

        Log("Doing call with " + payload);

        RestCallback cbxcb = new RestCallback;
        RestContext ctx = GetRestApi().GetRestContext("https://webhook.site/2ea04ec7-5c21-4fb5-8aee-bee957f83984");
        ctx.POST(cbxcb, "", payload);

        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.Uploader, 60000, false);
    }
}
