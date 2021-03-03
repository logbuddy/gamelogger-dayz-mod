class GameLogger : JMModuleBase
{
    protected static ref GameLogger s_Instance;

    static const string m_ProfilePath = "$profile:GameLogger";
    static const string m_UploadPath = m_ProfilePath + "/upload";
    static const string m_InProgressPath = m_ProfilePath + "/inprogress";
    static const string m_DonePath = m_ProfilePath + "/done";

    void GameLogger()
    {
        s_Instance = this;

        MakeDirectory(m_ProfilePath);
        MakeDirectory(m_UploadPath);
        MakeDirectory(m_InProgressPath);
        MakeDirectory(m_DonePath);

        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.LogReader, 1000, false);
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.Uploader, 1000, false);
    }

    static GameLogger GetInstance()
    {
        if (NULL == s_Instance) s_Instance = new GameLogger;
        return s_Instance;
    }

    override bool IsClient()
    {
        return false;
    }

    void Log(string message)
    {
        int year = 0;
        int month = 0;
        int day = 0;
        int hour = 0;
        int minute = 0;
        int second = 0;

        GetHourMinuteSecond(hour, minute, second);
        GetYearMonthDay(year, month, day);
       
        Print("[GameLogger][" + year.ToStringLen(2) + "-" + month.ToStringLen(2) + "-" + day.ToStringLen(2) + " " + hour.ToStringLen(2) + ":" + minute.ToStringLen(2) + ":" + second.ToStringLen(2) + "] " + message);
    }

    void Ingest(string modname, string message)
    {
        Print(message);

        switch (modname)
        {
            case "Trader":
            {
                if (message.Substring(0, 18) == "[TRADER] Player: (")
                {
                    s_Instance.Trader(message.Substring(9, message.Length() - 9));
                }

                break;
            }
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
            fileList.Insert(m_InProgressPath + "/" + fileName);
            CopyFile(m_UploadPath + "/" + fileName, m_InProgressPath + "/" + fileName);
            DeleteFile(m_UploadPath + "/" + fileName);
        }

        fileName = "";

        while (FindNextFile(findFileHandle, fileName, fileAttributes))
        {
            if (fileName)
            {
                fileList.Insert(m_InProgressPath + "/" + fileName);
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

        string payload;
        string readLine;

        foreach(string payloadFile: fileList)
        {
            fileHandle = OpenFile(payloadFile, FileMode.READ);
            while (FGets(fileHandle, readLine) > 0)
            {
                payload = payload + readLine + ",";
            }
            CloseFile(fileHandle);
        }

        payload = payload.Substring(0, payload.Length() - 1);
        payload = "[" + payload  + "]";

        Log("Doing call with " + payload);

        RestCallback cbxcb = new RestCallback;
        RestContext ctx = GetRestApi().GetRestContext("https://webhook.site/5275dbb6-93ab-47e7-90e2-2a5d587ddf08");
        ctx.POST(cbxcb, "", payload);

        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.Uploader, 60000, false);
    }

    void AddToUploadQueue(JsonObject json)
    {
        int year = 0;
        int month = 0;
        int day = 0;
        int hour = 0;
        int minute = 0;
        int second = 0;

        GetHourMinuteSecond(hour, minute, second);
        GetYearMonthDay(year, month, day);

        string fileDate = year.ToStringLen(2) + month.ToStringLen(2) + day.ToStringLen(2) + hour.ToStringLen(2) + minute.ToStringLen(2) + second.ToStringLen(2);
        FileHandle file = OpenFile(m_UploadPath + "/upload_" + fileDate + "_" + Math.RandomInt(0, 1000000) + ".json", FileMode.WRITE);

        if(file)
        {
            FPrint(file, json.GetJson());
            CloseFile(file);
        }
    }

    // INGEST

    void Trader(string message)
    {
        this.Log("Trader sale: " + message);

        JsonObject json = new JsonObject;

        json.AddString("mod", "trader");
        json.AddString("action", "buy");
        json.AddString("message", message);

        this.AddToUploadQueue(json);
    }
}

