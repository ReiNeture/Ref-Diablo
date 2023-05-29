// Diablo II LOD 面板.

public main_info_menu( id ) 
{
	static PlayerName[32], ShowMessage[2048];

	get_user_name( id, PlayerName, charsmax( PlayerName ) );

	copy( ShowMessage, charsmax( ShowMessage ), "<body bgcolor=#000000><font color=#FFB000>" );

	formatex( ShowMessage, charsmax( ShowMessage ), "%s<center><b>%s的個人狀態</b></center><p>", ShowMessage, PlayerName );

	add( ShowMessage, charsmax( ShowMessage ), "<center><table width=800 border=1 cellpadding=4 cellspacing=4>" );
	add( ShowMessage, charsmax( ShowMessage ), "<tr><td valign=top width=50%><font color=#FFB000>" );

	new block_value; 
	for (new item_id = 0; item_id <= g_charcounter; item_id++)
	{
		block_value = (g_iPlayerItemWorn[id][g_CurrentChar[id]][item_id] && g_iPlayerItem[id][g_CurrentChar[id]][item_id] > 0 && item_data[item_id][CHAR_BLOCK] > 0 ? item_data[item_id][CHAR_BLOCK] : 0)
	}


	// Main info
	formatex( ShowMessage, charsmax( ShowMessage ), "%s等級: %d <br>", ShowMessage, g_PlayerLevel[id][g_CurrentChar[id]] );
	formatex( ShowMessage, charsmax( ShowMessage ), "%s經驗值: %d / %d <br>", ShowMessage, g_PlayerXp[id][g_CurrentChar[id]], LEVELS[g_PlayerLevel[id][g_CurrentChar[id]]] );
	formatex( ShowMessage, charsmax( ShowMessage ), "%s剩餘能力點: %d <br>", ShowMessage, g_PlayerStPoints[id][g_CurrentChar[id]] );
	formatex( ShowMessage, charsmax( ShowMessage ), "%s剩餘技能點: %d <br>", ShowMessage, g_PlayerSkPoints[id][g_CurrentChar[id]] );
		
	add( ShowMessage, charsmax( ShowMessage ), "</td><td valign=top width=50%><font color=#FFB000>" );

	add( ShowMessage, charsmax( ShowMessage ), "</td></tr><tr><td valign=top><font color=#FFB000>" );
	add( ShowMessage, charsmax( ShowMessage ), "<center><b>Attributes</b></center><p>" );

	// Stat info
	formatex( ShowMessage, charsmax( ShowMessage ), "%s力量: %d <br>", ShowMessage, g_Strength[id][g_CurrentChar[id]] );
	formatex( ShowMessage, charsmax( ShowMessage ), "%s敏捷: %d <br>", ShowMessage, g_Dexterity[id][g_CurrentChar[id]] );
	formatex( ShowMessage, charsmax( ShowMessage ), "%s體力: %d <br>", ShowMessage, g_Vitality[id][g_CurrentChar[id]] );
	formatex( ShowMessage, charsmax( ShowMessage ), "%s能量: %d <br>", ShowMessage, g_Energy[id][g_CurrentChar[id]] );

	add( ShowMessage, charsmax( ShowMessage ), "</td><td valign=top><font color=#FFB000>" );
	add( ShowMessage, charsmax( ShowMessage ), "<center><b>Misc. Info</b></center><p>" );
	
	// Misc info
	formatex( ShowMessage, charsmax( ShowMessage ), "%s爆擊率: %d + %d%% <br>", ShowMessage, HEROES_DEXTERITY_PERSTAT[g_PlayerHero[id][g_CurrentChar[id]]] * g_Dexterity[id][g_CurrentChar[id]], block_value );
	formatex( ShowMessage, charsmax( ShowMessage ), "%s護甲: %d <br>", ShowMessage, g_Armor[id] );
	formatex( ShowMessage, charsmax( ShowMessage ), "%s血量: %d / %d <br>", ShowMessage, get_user_health(id), g_MaxHealth[id] );
	formatex( ShowMessage, charsmax( ShowMessage ), "%s能量: %d / %d <br>", ShowMessage, g_CurrentMana[id], HEROES_ENERGY_PERSTAT[g_PlayerHero[id][g_CurrentChar[id]]] * g_Energy[id][g_CurrentChar[id]] );
	formatex( ShowMessage, charsmax( ShowMessage ), "%s錢: %d / %d <br>", ShowMessage, g_Coins[id][g_CurrentChar[id]], MAX_PLAYER_MONEY[g_PlayerLevel[id][g_CurrentChar[id]]] );

	// Items
	add( ShowMessage, charsmax( ShowMessage ), "</td></tr><tr><td colspan=2 valign=top><font color=#FFB000>" );
	add( ShowMessage, charsmax( ShowMessage ), "<center><b>道具</b></center><p>" );

	for (new item_id = 0; item_id <= g_charcounter; item_id++)
	{
		if ( g_iPlayerItem[id][g_CurrentChar[id]][item_id] > 0 )
		{
			formatex( ShowMessage, charsmax( ShowMessage ), "%s %s - %d - 耐久度 %d <br>", ShowMessage, item_name[item_id], g_iPlayerItem[id][g_CurrentChar[id]][item_id], g_iPlayerItemRepair[id][g_CurrentChar[id]][item_id]);
		}
	}

	// end of lines
	add( ShowMessage, charsmax( ShowMessage ), "</td></tr>" );
	add( ShowMessage, charsmax( ShowMessage ), "</table></center>" );

	// show MOTD
	show_motd( id, ShowMessage, "角色資料" );
}