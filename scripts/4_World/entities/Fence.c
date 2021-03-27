modded class Fence
{
	override void OnPartDestroyedServer(Man player, string part_name, int action_id, bool destroyed_by_connected_part = false)
	{

		super.OnPartDestroyedServer(player, part_name, action_id, destroyed_by_connected_part);
	}


}