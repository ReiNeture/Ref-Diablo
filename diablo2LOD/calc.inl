public Float:get_totaldmg_of_item(id)
{
    new Float: counted_damage = 0.0;
    for (new item_id = 0; item_id <= g_charcounter; item_id++)
	{
		if ( g_iPlayerItemWorn[id][g_CurrentChar[id]][item_id] && g_iPlayerItem[id][g_CurrentChar[id]][item_id] > 0 
			&& g_PlayerLevel[id][g_CurrentChar[id]] >= item_data[item_id][CHAR_LEVEL] 
			&& g_Strength[id][g_CurrentChar[id]] >= item_data[item_id][CHAR_STR] && g_Dexterity[id][g_CurrentChar[id]] >= item_data[item_id][CHAR_DEX] && float(item_data[item_id][CHAR_DAMAGE]) > 0.0 )
		{
			counted_damage += float(item_data[item_id][CHAR_DAMAGE]);
		}
	}

    return counted_damage;
}