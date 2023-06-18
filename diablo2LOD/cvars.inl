// Cvars,pcvars,指令,功能.

public LoadCvars()
{
	d2_save_type = register_cvar("d2_save_type", "0"); // 存檔類型 : 1 = MYSQL, 0 = Nvault.

	d2_save_by = register_cvar("d2_save_by", "2");// 存檔方式 : 0 = SteamID, 1 = IP, 2 = 名字.

	mysqlx_host = register_cvar ("d2_host", ""); // 數據庫主機.

	mysqlx_user = register_cvar ("d2_user", ""); // 數據庫帳號.

	mysqlx_pass = register_cvar ("d2_pass", ""); // 數據庫密碼.

	mysqlx_db = register_cvar ("d2_dbname", ""); // 數據庫名稱.

	d2_xp_kill = register_cvar("d2_xp_kill","120"); // 殺人經驗.

	d2_3rdperson = register_cvar("d2_3rdperson","1"); // 開啟第三人稱? 1 - 是, 0 - 否.

	d2_advert = register_cvar("d2_advertisement","1"); // 開啟公告? 1 - 是, 0 - 否.

	d2_advert_time = register_cvar("d2_adv_time","180.0"); // 公告出現時間.

	d2_block_attack2 = register_cvar("d2_block_attack2","1"); // 鎖住重砍? 1 - 是, 0 - 否.

	d2_cl_corpsestay = register_cvar("d2_cl_corpsestay","5.000000"); // 屍體消失時間, 1.000000 = 1 秒.

	d2_max_random_value = register_cvar("d2_max_random","1000"); // 迴避率/命中率最大值.

	d2_stat_points_levelup = register_cvar("d2_stat_points","5"); // 升等給予多少能力點.

	d2_high_level = register_cvar("d2_high_level","30"); // 擊殺高等玩家的獎勵,等級需要差距多少.

	d2_high_level_xp = register_cvar("d2_high_level_xp","100"); // 擊殺高等玩家的獎勵,獎勵多少經驗.

	d2_enable_respawn = register_cvar("d2_spawn","1"); // 開啟重生? 1 = 是,  0 = 否.

	d2_spawn_cooldown = register_cvar("d2_spawn_cooldown","5"); // 重生冷卻時間.

	d2_spawn_protection = register_cvar("d2_spawn_protection","1"); // 開啟重生保護? 1 = 是, 0 = 否.

	d2_spawn_protection_time = register_cvar("d2_spawn_protection_time","3"); // 重生保護時間.

	d2_block_kill_command = register_cvar("d2_block_kill_command","1"); // 鎖住自殺指令? 1 = 是, 0 = 否.

	d2_show_damage = register_cvar("d2_show_damage","1"); // 顯示傷害?

	d2_show_damage_received = register_cvar("d2_show_damage_r","1"); // 顯示受到的傷害?

	d2_minlevel_coins = register_cvar("d2_minlevel_drop_coins","5"); // 至少要幾等才會掉錢.

	d2_minlevel_items = register_cvar("d2_minlevel_drop_item","20"); // 至少要幾等才會掉道具.

	d2_bolts_value = register_cvar("d2_bolts_value","100"); // 一組弓箭有幾隻.

	d2_chance_to_drop = register_cvar("d2_chance_drop_item","0"); // 裝備掉落機率 ( 百分比 ).

	d2_minlevel_potions = register_cvar("d2_minlevel_drop_potion","10"); // 至少要幾等才會掉藥水.

	d2_chance_to_dropp = register_cvar("d2_chance_drop_potion","15"); // 藥水掉落機率 ( 百分比 ).

	d2_exp_scale = register_cvar("d2_exp_scale","15"); // 藥水掉落機率 ( 百分比 ).

	register_clcmd("say /d2menu","main_mod_menu");
	register_clcmd("say d2menu","main_mod_menu");

	register_clcmd("say /level","HUD_Say_Info_Task");
	register_clcmd("say level","HUD_Say_Info_Task");

	register_clcmd("say trade","main_trade_menu");
	register_clcmd("say /trade","main_trade_menu");

	register_clcmd("say giveitem","main_give_menu");
	register_clcmd("say /giveitem","main_give_menu");

	register_clcmd("amenu", "Open_MapItemConfig")
	register_clcmd("say amenu", "Open_MapItemConfig")
	register_clcmd("say /amenu", "Open_MapItemConfig")

	register_clcmd("say /worn","main_worn_menu");
	register_clcmd("say worn","main_worn_menu");

	register_clcmd("say","handle_say");
	register_clcmd("say_team","handle_say");

	register_clcmd("say /potion","main_potion_menu");
	register_clcmd("say potion","main_potion_menu");
	register_clcmd("potion","main_potion_menu");

	register_clcmd("say /spec","force_player_not_joined");
	register_clcmd("say spec","force_player_not_joined");

	register_clcmd("+skills","skill_handle");

	register_clcmd("chooseteam", "clcmd_changetm")
	register_clcmd("jointeam", "clcmd_changetm" );

	register_concmd("set_level", "cmd_set_level", ADMIN_RCON, "<名稱> <數量>" );
	register_concmd("set_gold", "cmd_set_gold", ADMIN_RCON, "<名稱> <數量>" );
}

// 讀取道具與設定.
public LoadIniItems()
{
	// 該ini的路徑.
	static configsdir[256];
	get_configsdir( configsdir, charsmax( configsdir ) );
	
	// 創造一個inl文件.
	static configfile[256];
	formatex( configfile, charsmax( configfile ), "%s/%s", configsdir, "d2items.ini" );
	
	// 文件是不是在這裡?
	if ( !file_exists( configfile ) )
	{
		// 不,設定資訊失敗.
		static sError[256];
		formatex( sError, charsmax( sError ), "無法讀取 %s", configfile );
		set_fail_state( sError );
		
		return;
	}
	
	// 新增一些字元.
	new linedata[1024], key[64], value[960], section;
	
	// 打開並讀取自定義文件.
	new file = fopen( configfile, "rt" );
		
	while ( file && !feof( file ) )
	{
		// 讀取一次.
		fgets( file, linedata, charsmax( linedata ) );
		
		// 將空的一行代替為空格,防止BUG出現.
		replace( linedata, charsmax( linedata ), "^n", "" );
		
		// 空白行或解說.
		if ( !linedata[0] || linedata[0] == ';' ) 
			continue;
		
		// 新的一段的開始.
		if ( linedata[0] == '[' )
		{
			section++;
			continue;
		}
		
		// 獲得一些值.
		strtok( linedata, key, charsmax( key ), value, charsmax( value ), '=' );
		
		// 修飾這些值.
		trim( key );
		trim( value );
		
		switch ( section )
		{
			case 1: // 我們在這裡讀取所有道具.
			{
				// 道具達到最大值.
				if ( g_charcounter > MAX_ITEMS )
				{
					// 因此返回.
					continue;
				}
				
				if ( equal( key, "NAME" ) )
				{
					// 找到名稱後,新增一個道具.
					g_charcounter++;
					
					// 保存名字.
					formatex( item_name[g_charcounter], 31, "%s", value );
				}
				else if ( equali( key, "VMODEL" ) )
				{
					formatex( item_vmodel[g_charcounter], 63, "%s", value );
				}
				else if ( equali( key, "PMODEL" ) )
				{
					formatex( item_pmodel[g_charcounter], 63, "%s", value );
				}
				else if ( equali( key, "ARMOR" ) )
				{
					item_data[g_charcounter][CHAR_ARMOR] = str_to_num( value );
				}
				else if ( equali( key, "DAMAGE" ) )
				{
					item_data[g_charcounter][CHAR_DAMAGE] = str_to_num( value );
				}
				else if ( equali( key, "COST" ) )
				{
					item_data[g_charcounter][CHAR_COST] = str_to_num( value );
				}
				else if ( equali( key, "DURABILITY" ) )
				{
					item_data[g_charcounter][CHAR_DURABILITY] = str_to_num( value );						
				}
				else if ( equali( key, "STRENGTH_REQ" ) )
				{
					item_data[g_charcounter][CHAR_STR] = str_to_num( value );
				}
				else if ( equali( key, "DEXTERITY_REQ" ) )
				{
					item_data[g_charcounter][CHAR_DEX] = str_to_num( value );
				}
				else if ( equali( key, "LEVEL_REQ" ) )
				{
					// 儲存數值.
					item_data[g_charcounter][CHAR_LEVEL] = str_to_num( value );
				}
				else if ( equali( key, "BLOCK_CHANCE" ) )
				{
					// 儲存數值.
					item_data[g_charcounter][CHAR_BLOCK] = str_to_num( value );
				}
				else if ( equali( key, "GIVE_VITALITY" ) )
				{
					// 儲存數值.
					item_data[g_charcounter][CHAR_GIVEVIT] = str_to_num( value );
				}
				else if ( equali( key, "GIVE_ENERGY" ) )
				{
					// 儲存數值.
					item_data[g_charcounter][CHAR_GIVEENE] = str_to_num( value );
				}
				else if ( equali( key, "ITEM_TYPE" ) )
				{
					// 儲存數值.
					item_data[g_charcounter][CHAR_TYPE] = str_to_num( value );
				}
				else if ( equali( key, "HAS_POISON" ) )
				{
					// 儲存數值.
					item_data[g_charcounter][CHAR_HASP] = str_to_num( value );
				}
				else if ( equali( key, "POISON_DAMAGE" ) )
				{
					// 儲存數值.
					item_data[g_charcounter][CHAR_PDAMAGE] = str_to_num( value );
				}
				else if ( equali( key, "POISON_DURATION" ) )
				{
					// 儲存數值.
					item_data[g_charcounter][CHAR_PDURATION] = str_to_num( value );
				}
				else if ( equali( key, "CROSSBOW_DAMAGE" ) )
				{
					// 儲存數值.
					item_data[g_charcounter][CHAR_CROSSBOWDAMAGE] = str_to_num( value );
				}
			}
		}
	}
}