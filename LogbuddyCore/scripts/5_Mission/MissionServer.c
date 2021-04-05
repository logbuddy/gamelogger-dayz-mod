/*
modded class MissionServer
{

    int fps = 0;
    void MissionServer()
    {
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(SaveFPS, 1000, true);
    }

    void ~MissionServer()
    {
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(SaveFPS);
    }

    void SaveFPS()
    {
        int timeRunning = GetGame().GetTime() / 1000;
        int playerCount = GetOnlinePlayerCount();
        Print("Fps: " + fps + " Time up: " + timeRunning + " Players: " + playerCount);
        fps = 0;
    }

    override void OnUpdate(float timeslice)
    {
        super.OnUpdate(timeslice);
        fps++;
    }
}
*/