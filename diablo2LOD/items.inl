// Diablo II LOD 道具效果.

public bool:Player_WItemType(id, type)
{
	for(new item_id = 0; item_id <= g_charcounter ; item_id++) 
	{
		if ( g_iPlayerItemWorn[id][g_CurrentChar[id]][item_id] && g_iPlayerItem[id][g_CurrentChar[id]][item_id] > 0  && item_data[item_id][CHAR_TYPE] == type )
		return true;
	}

	return false;
}
public Player_WItemTypeValue(id, type, item_id2)
{
	new Value = 0;

	for(new item_id = 0; item_id <= g_charcounter ; item_id++) 
	{
		if ( g_iPlayerItemWorn[id][g_CurrentChar[id]][item_id] && g_iPlayerItem[id][g_CurrentChar[id]][item_id] > 0 && item_data[item_id][CHAR_TYPE] == type && item_data[item_id2][CHAR_TYPE] == type )
			Value++;
	}

	return Value;
}
public bool:Player_WItemDataActive(id, data)
{
	for(new item_id = 0; item_id <= g_charcounter ; item_id++) 
	{
		if ( g_iPlayerItemWorn[id][g_CurrentChar[id]][item_id] && g_iPlayerItem[id][g_CurrentChar[id]][item_id] > 0 && item_data[item_id][data] > 0 )
			return true;
	}

	return false;
}
public Player_WItemTypeValue2(id, type)
{
	new Value = 0;

	for(new item_id = 0; item_id <= g_charcounter ; item_id++) 
	{
		if ( g_iPlayerItemWorn[id][g_CurrentChar[id]][item_id] && g_iPlayerItem[id][g_CurrentChar[id]][item_id] > 0 && item_data[item_id][CHAR_TYPE] == type )
			Value++;
	}

	return Value;
}

public Get_Player_Items(id)
{
	static ItemNum; ItemNum = 0;

	for(new item_id = 0; item_id <= g_charcounter ; item_id++) 
	{
		if( g_iPlayerItem[id][g_CurrentChar[id]][item_id] > 0 )
			ItemNum += g_iPlayerItem[id][g_CurrentChar[id]][item_id]
	}

	return ItemNum;
}
public Get_Player_ItemsInv(id)
{
	static ItemNum; ItemNum = 0;

	for(new item_id = 0; item_id <= g_charcounter ; item_id++) 
	{
		if( g_iPlayerItemInv[id][g_CurrentChar[id]][item_id] > 0 )
			ItemNum += g_iPlayerItemInv[id][g_CurrentChar[id]][item_id]
	}

	return ItemNum;
}
public Get_Player_Potions(id)
{
	static ItemNum; ItemNum = 0;

	for(new item_id = 0; item_id < MAX_POTIONS ; item_id++) 
	{
		if( g_iPlayerPotions[id][g_CurrentChar[id]][item_id] > 0 )
			ItemNum += g_iPlayerPotions[id][g_CurrentChar[id]][item_id]
	}

	return ItemNum;
}
public Get_Player_PotionsInv(id)
{
	static ItemNum; ItemNum = 0;

	for(new item_id = 0; item_id < MAX_POTIONS ; item_id++) 
	{
		if( g_iPlayerPotionsInv[id][g_CurrentChar[id]][item_id] > 0 )
			ItemNum += g_iPlayerPotionsInv[id][g_CurrentChar[id]][item_id]
	}

	return ItemNum;
}
public Set_Player_Item_Repair(id, item_id, value)
{
	g_iPlayerItemRepair[id][g_CurrentChar[id]][item_id] = value;

	if ( g_iPlayerItemRepair[id][g_CurrentChar[id]][item_id] <= 0 )
	{
		g_iPlayerItem[id][g_CurrentChar[id]][item_id]--;

		client_printcolor(id, "/ctr%s /y壞了.", item_name[item_id]);

		if ( g_iPlayerItem[id][g_CurrentChar[id]][item_id] < 1 )
		{
			g_iPlayerItemWorn[id][g_CurrentChar[id]][item_id] = ITEM_NOT_WORN;

			if ( item_data[item_id][CHAR_TYPE] == TYPE_WEAPON )
			{
				entity_set_string(id, EV_SZ_viewmodel, g_brassknuckles);
				entity_set_string(id, EV_SZ_weaponmodel, "");
			}

			ExecuteForward( g_iItemTook, g_iReturn, id, item_id);
		}
	}

	if( g_iPlayerItemRepair[id][g_CurrentChar[id]][item_id] > item_data[item_id][CHAR_DURABILITY] )
	{
		g_iPlayerItemRepair[id][g_CurrentChar[id]][item_id] = item_data[item_id][CHAR_DURABILITY];
	}
}

// 獲得玩家重生時的護甲.
public Get_Spawn_Armor_Item(id, item_id)
{
	if ( !is_user_alive(id) )
		return 0;

	if ( g_iPlayerItemWorn[id][g_CurrentChar[id]][item_id] && g_iPlayerItem[id][g_CurrentChar[id]][item_id] > 0 
	&& g_PlayerLevel[id][g_CurrentChar[id]] >= item_data[item_id][CHAR_LEVEL] && g_Strength[id][g_CurrentChar[id]] >= item_data[item_id][CHAR_STR] 
	&& g_Dexterity[id][g_CurrentChar[id]] >= item_data[item_id][CHAR_DEX] )
	{
		return item_data[item_id][CHAR_ARMOR];
	}

	return 0;
}

// 設定玩家的護甲.
public Set_Player_Armor_Item(id, item_id)
{
	if ( !is_user_alive(id) )
		return;

	if ( g_iPlayerItemWorn[id][g_CurrentChar[id]][item_id] && g_iPlayerItem[id][g_CurrentChar[id]][item_id] > 0 
	&& g_PlayerLevel[id][g_CurrentChar[id]] >= item_data[item_id][CHAR_LEVEL] && g_Strength[id][g_CurrentChar[id]] >= item_data[item_id][CHAR_STR] 
	&& g_Dexterity[id][g_CurrentChar[id]] >= item_data[item_id][CHAR_DEX] && item_data[item_id][CHAR_ARMOR] > 0 )
	{
		g_Armor[id] += item_data[item_id][CHAR_ARMOR];
	}
}

// 扣除玩家的護甲.
public Take_Player_Armor_Item(id, item_id)
{
	if ( !is_user_alive(id) )
		return;

	if ( item_data[item_id][CHAR_ARMOR] > 0 )
	{
		g_Armor[id] -= item_data[item_id][CHAR_ARMOR];
	}
}

public Set_Player_Energy_Item(id, item_id)
{
	if ( g_iPlayerItemWorn[id][g_CurrentChar[id]][item_id] && g_iPlayerItem[id][g_CurrentChar[id]][item_id] > 0 
	&& g_PlayerLevel[id][g_CurrentChar[id]] >= item_data[item_id][CHAR_LEVEL] && g_Strength[id][g_CurrentChar[id]] >= item_data[item_id][CHAR_STR] 
	&& g_Dexterity[id][g_CurrentChar[id]] >= item_data[item_id][CHAR_DEX] && item_data[item_id][CHAR_GIVEENE] > 0 )
	{
		g_Energy[id][g_CurrentChar[id]] += item_data[item_id][CHAR_GIVEENE];
	}
}
public Set_Player_Vitality_Item(id, item_id)
{
	if ( g_iPlayerItemWorn[id][g_CurrentChar[id]][item_id] && g_iPlayerItem[id][g_CurrentChar[id]][item_id] > 0 
	&& g_PlayerLevel[id][g_CurrentChar[id]] >= item_data[item_id][CHAR_LEVEL] && g_Strength[id][g_CurrentChar[id]] >= item_data[item_id][CHAR_STR] 
	&& g_Dexterity[id][g_CurrentChar[id]] >= item_data[item_id][CHAR_DEX] && item_data[item_id][CHAR_GIVEVIT] > 0 )
	{
		g_Vitality[id][g_CurrentChar[id]] += item_data[item_id][CHAR_GIVEVIT];
	}
}
public Set_Player_Skills_Item(id, item_id)
{
	if ( g_iPlayerItemWorn[id][g_CurrentChar[id]][item_id] && g_iPlayerItem[id][g_CurrentChar[id]][item_id] > 0 
	&& g_PlayerLevel[id][g_CurrentChar[id]] >= item_data[item_id][CHAR_LEVEL] && g_Strength[id][g_CurrentChar[id]] >= item_data[item_id][CHAR_STR] 
	&& g_Dexterity[id][g_CurrentChar[id]] >= item_data[item_id][CHAR_DEX] && item_data[item_id][CHAR_SKILLSID] > 0 )
	{
		for (new skill_id = 0; skill_id <= g_skillcounter; skill_id++)
		{
			if ( g_skillhero[skill_id] == item_data[item_id][CHAR_SKILLSID] )
			{
				g_iSkills[id][g_CurrentChar[id]][skill_id]++;
			}
		}
	}
}

public Set_Player_Potion_HP(id, item_id)
{
	if ( !is_user_alive(id) )
		return;

	new get_p_hp = get_user_health(id);
	new counted_hp = g_MaxHealth[id];

	switch(item_id)
	{
		case PTS_HP_MINOR:
		{
			set_user_health(id, get_p_hp + Potion_Multi[PTS_HP_MINOR] );

			if ( get_user_health(id) > counted_hp )
			{
				set_user_health(id, counted_hp);
			}
		}
		case PTS_HP_GREATER:
		{
			set_user_health(id, get_p_hp + Potion_Multi[PTS_HP_GREATER] );

			if ( get_user_health(id) > counted_hp )
			{
				set_user_health(id, counted_hp);
			}
		}
		case PTS_HP_SUPER:
		{
			set_user_health(id, get_p_hp + Potion_Multi[PTS_HP_SUPER] );

			if ( get_user_health(id) > counted_hp )
			{
				set_user_health(id, counted_hp);
			}
		}
		case PTS_MP_MINOR:
		{
			Set_Player_Mana(id, g_CurrentMana[id] + Potion_Multi[PTS_MP_MINOR]);
		}
		case PTS_MP_GREATER:
		{
			Set_Player_Mana(id, g_CurrentMana[id] + Potion_Multi[PTS_MP_GREATER]);
		}
		case PTS_MP_SUPER:
		{
			Set_Player_Mana(id, g_CurrentMana[id] + Potion_Multi[PTS_MP_SUPER]);
		}
		case PTS_RP_MINOR:
		{
			set_user_health(id, get_p_hp + (Potion_Multi[PTS_HP_MINOR] / Potion_Multi[PTS_RP_MINOR]) );

			if ( get_user_health(id) > counted_hp )
			{
				set_user_health(id, counted_hp);
			}

			Set_Player_Mana(id, g_CurrentMana[id] + (Potion_Multi[PTS_MP_MINOR] / Potion_Multi[PTS_RP_MINOR]) );
		}
		case PTS_RP_GREATER:
		{
			set_user_health(id, get_p_hp + (Potion_Multi[PTS_HP_GREATER] * Potion_Multi[PTS_RP_GREATER]) );

			if ( get_user_health(id) > counted_hp )
			{
				set_user_health(id, counted_hp);
			}

			Set_Player_Mana(id, g_CurrentMana[id] + (Potion_Multi[PTS_MP_GREATER] * Potion_Multi[PTS_RP_GREATER]) );
		}
		case PTS_RP_SUPER:
		{
			set_user_health(id, get_p_hp + (Potion_Multi[PTS_HP_SUPER] * Potion_Multi[PTS_RP_SUPER]) );

			if ( get_user_health(id) > counted_hp )
			{
				set_user_health(id, counted_hp);
			}

			Set_Player_Mana(id, g_CurrentMana[id] + (Potion_Multi[PTS_MP_SUPER] * Potion_Multi[PTS_RP_SUPER]) );
		}
	}
}