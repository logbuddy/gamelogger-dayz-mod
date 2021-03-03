class GameLoggerInit : ItemBase
{
    override void EEInit()
    {
        super.EEInit();

        if (IsMissionHost())
        {
            GameLogger gl = GameLogger.GetInstance();
        }
    }
}