class CfgPatches
{
	class LogbuddyMuchCarKey
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {
			"DZ_Data",
			"DZ_Scripts",
			"LogbuddyCore",
			"MuchCarKey"
		};
	};   
};

class CfgMods
{
	class LogbuddyMuchCarKey
	{
		dir = "inkihh\Logbuddy\LogbuddyMuchCarKey";
		picture = "";
		action = "";
		hideName = 0;
		hidePicture = 0;
		name = "LogbuddyMuchCarKey";
		credits = "";
		author = "inkihh";
		authorID = "0";
		version = "1";
		extra = 0;
		type = "mod";

		dependencies[] =
		{
			"Game",
			"World",
			"Mission"
		};

		class defs
		{
			class gameScriptModule
			{
				value="";
				files[] =
				{
					"inkihh\Logbuddy\LogbuddyMuchCarKey\scripts\3_Game"
				};
			};
			class worldScriptModule
			{
				value="";
				files[] =
				{
					"inkihh\Logbuddy\LogbuddyMuchCarKey\scripts\4_World"
				};
			};

			class missionScriptModule
			{
				value="";
				files[] =
				{
					"inkihh\Logbuddy\LogbuddyMuchCarKey\scripts\5_Mission"
				};
			};
		};
	};
};
