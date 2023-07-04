#include <amxmodx>
#include <d2lod>
#include <engine>
#include <hamsandwich>
#include <fakemeta>

new PLUGIN_NAME[] = "Diablo II LOD 怪物補助插件"
new PLUGIN_AUTHOR[] = "xbatista"
new PLUGIN_VERSION[] = "1.0"

#define MAX_MONSTERS 18

#define COINS_CLASSNAME "CoinsMonster"

new const Monster_Models[MAX_MONSTERS][] =
{
	"models/agrunt.mdl",
	"models/big_mom.mdl",
	"models/bullsquid.mdl",
	"models/controller.mdl",
	"models/garg.mdl",
	"models/headcrab.mdl",
	"models/houndeye.mdl",
	"models/islave.mdl",
	"models/w_squeak.mdl",
	"models/zombie.mdl",
	"models/hgrunt.mdl",
	"models/tentacle2.mdl",
	"models/babygarg.mdl",
	"models/bigrat.mdl",
	"models/gonome.mdl",
	"models/apache.mdl",
	"models/gonome_x.mdl",
	"models/okasi_zombie.mdl"
}
new const Monster_Xp[MAX_MONSTERS] =
{
	1500,		// 異型戰士	
	50000,		// 大媽	
	4000,		// 鱷魚	
	1200,		// 首腦	
	80000,		// 大藍
	60,			// 食腦蟲
	500,		// 百目狗	
	1200,		// 弗地崗人	
	0,			// 聖甲蟲
	700,		// 殭屍	
	0,			// 人類戰士
	90000,		// 鷹爪
	5000,		// 小灰	
	0,			// 老鼠
	8000,		// 殭屍王
	50000,      // 阿帕契	
	10000,		// 變異殭屍王
	10000		// 可莉殭屍
}
new const Monster_Coins[MAX_MONSTERS] =
{
	20,					// 異型戰士	
	8000,				// 大媽	
	1500,				// 鱷魚	
	20,					// 首腦	
	15000,				// 大藍
	15,					// 食腦蟲
	52,					// 百目狗	
	25,					// 弗地崗人	
	0,					// 聖甲蟲
	60,					// 殭屍	
	0,					// 人類戰士
	10000,				// 鷹爪
	3000,				// 小灰	
	0,					// 老鼠
	2000,				// 殭屍王
	5000,				// 阿帕契	
	6000,				// 變異殭屍王
	200					// 可莉殭屍
}
new const Monster_Names[MAX_MONSTERS][] =
{
	"異型戰士",
	"大媽",
	"鱷魚",
	"首腦",
	"大藍",
	"食腦蟲",
	"百目狗",
	"弗地崗人",
	"聖甲蟲",
	"殭屍",
	"人類戰士",
	"鷹爪",
	"小灰",
	"老鼠",
	"殭屍王",
	"阿帕契",
	"變異殭屍王",
	"#3 殭屍"
}

new const Monster_BossPoints[MAX_MONSTERS] =
{
	0,			// 異型戰士
	200,		// 大媽
	20,			// 鱷魚
	0,			// 首腦
	250,		// 大藍
	0,			// 食腦蟲
	0,			// 百目狗
	0,			// 弗地崗人
	0,			// 聖甲蟲
	0,			// 殭屍
	0,			// 人類戰士
	160,		// 鷹爪
	50,			// 小灰
	0,			// 老鼠
	40,			// 殭屍王
	10000,		// 阿帕契
	70,			// 變異殭屍王
	0			// 可莉殭屍
}

#define MAX_DROPS_MONSTERS 8
new const Drops_Models[MAX_DROPS_MONSTERS][] =
{
	"models/headcrab.mdl",
	"models/houndeye.mdl",
	"models/zombie.mdl",
	"models/garg.mdl",
	"models/tentacle2.mdl",
	"models/gonome.mdl",
	"models/bullsquid.mdl",
	"models/babygarg.mdl"
}

new const Drops_Id[MAX_DROPS_MONSTERS] =
{
	0,		//	食腦蟲鱗片
	1,		//	百目狗的皮
	2,		//	A 血液
	3,		//	藍色核心
	4,		//	鷹爪嘴巴
	6,		//  殭屍王証明
	8,		//  鱷魚牙齒
	9,		//  小灰通行證
}

// 1 = 0.1%, 10 = 1%, 100 = 10%, 1000 = 100%;
new const Drops_Chance[MAX_DROPS_MONSTERS] =
{
	500,
	1,
	1,
	50,
	50,
	50,
	100,
	50
}

new g_iMaxPlayers;
new Float:g_view_time[33];
new d2_exp_scale;


public plugin_init() 
{
	register_plugin(PLUGIN_NAME, PLUGIN_VERSION, PLUGIN_AUTHOR);
	d2_exp_scale = register_cvar("d2_exp_scale", "1");

	RegisterHam(Ham_Killed, "func_wall", "Monster_Killed");
	RegisterHam(Ham_Player_PreThink, "player", "fw_Monster_PreThink");

	register_touch( COINS_CLASSNAME, "player", "Coins_Pickup");
	register_logevent("Event_Round_End", 2, "1=Round_End");

	g_iMaxPlayers = get_maxplayers();
}

public Event_Round_End()
{
	Remove_All_Coin_Ents();
}

public Monster_Killed(this, idattacker, shouldgib)
{
	if ( !( 1 <= idattacker <= g_iMaxPlayers ) || !is_valid_ent(this) || !get_player_logged(idattacker) )
		return HAM_IGNORED;

	static MonsterMdl[32];
	entity_get_string( this, EV_SZ_model, MonsterMdl, charsmax(MonsterMdl) );
	// client_print(0, print_chat, "%s", MonsterMdl);

	static names[32];
	get_user_name(idattacker, names, charsmax(names))

	for(new monsters = 0; monsters < MAX_MONSTERS; monsters++) 
	{
		if( equal( MonsterMdl, Monster_Models[monsters] ) ) 
		{
			if ( Monster_Xp[monsters] > 0 )
			{
				set_p_xp(idattacker, get_p_xp(idattacker) + Monster_Xp[monsters] * get_pcvar_num(d2_exp_scale));
				set_p_gold(idattacker, get_p_gold(idattacker) + Monster_Coins[monsters] + (get_p_level(idattacker)/4) );
				client_print( idattacker, print_center, "你殺了 %s, +%d經驗, +%d$", Monster_Names[monsters], Monster_Xp[monsters] * get_pcvar_num(d2_exp_scale), Monster_Coins[monsters] + (get_p_level(idattacker)/4) );
			}

			if ( Monster_BossPoints[monsters] > 0 ) {
				set_p_bosspoint(idattacker, get_p_bosspoint(idattacker) + Monster_BossPoints[monsters]);
				client_printcolor(0, "/ctr%s /g殺了 /ctr%s/g, 獲得 /y%d /gBossPoint", names, Monster_Names[monsters], Monster_BossPoints[monsters]);
			}

			// if ( Monster_Coins[monsters] > 0 )
			// 	drop_coins( this, COINS_CLASSNAME, Monster_Coins[monsters] + (get_p_level(idattacker) / 4) );
		}
	}

	// 掉落物品
	for(new monsters = 0; monsters < MAX_DROPS_MONSTERS; monsters++) 
	{
		if( equal( MonsterMdl, Drops_Models[monsters] ) && Drops_Id[monsters] > -1 ) 
		{
			if( random_num(1, 1000) <= Drops_Chance[monsters] )
			{
				set_p_drops(idattacker, Drops_Id[monsters], get_p_drops(idattacker, Drops_Id[monsters]) + 1);
				
				if( Drops_Chance[monsters] <= 20 )
				{
					client_printcolor(idattacker, "/g獲得稀有物品/ctr!");
				}
			}
		}
	}

	return HAM_IGNORED;
}

public fw_Monster_PreThink(this, idattacker)
{
	if( !is_valid_ent(this) )
		return HAM_IGNORED

	if( g_view_time[this] > get_gametime() )
		return HAM_IGNORED

	g_view_time[this] = get_gametime() + 0.2;

	new iTarget, iTemp, szMonsterModel[33], szClassname[33]
	get_user_aiming(this, iTarget, iTemp)
	entity_get_string(iTarget, EV_SZ_model, szMonsterModel, sizeof(szMonsterModel))
	entity_get_string(iTarget, EV_SZ_classname, szClassname, sizeof(szClassname))

	for(new i = 0; i < MAX_MONSTERS; i++)
	{
		if(equal(szClassname, "func_wall"))
		{
			if(equal(szMonsterModel, Monster_Models[i]))
			{
				set_hudmessage(0, 191, 255, 0.10, 0.55, 0, 0.2, 0.4, 0.1, 0.1, 4)
				show_hudmessage(this, "怪物: %s^n血量: %d^n經驗: %d", Monster_Names[i], floatround(entity_get_float(iTarget, EV_FL_health)), Monster_Xp[i] * get_pcvar_num(d2_exp_scale) )
				break;
			}
		}
	}
	return HAM_HANDLED
}

// Touch, coins
public Coins_Pickup(ptr, ptd)
{
	if( is_user_alive(ptd) && pev_valid(ptr) ) 
	{ 	
		new gold = entity_get_int(ptr , EV_INT_iuser1)

		set_p_gold(ptd, get_p_gold(ptd) + gold)
					
		remove_entity(ptr)
	}
}
public Remove_All_Coin_Ents()
{
	new coin_ent = find_ent_by_class(-1, COINS_CLASSNAME)
	
	while ( coin_ent ) 
	{
		remove_entity(coin_ent)
		coin_ent = find_ent_by_class(coin_ent, COINS_CLASSNAME)
	}
}