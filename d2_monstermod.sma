#include <amxmodx>
#include <d2lod>
#include <engine>
#include <hamsandwich>
#include <fakemeta>

new PLUGIN_NAME[] = "Diablo II LOD 怪物補助插件"
new PLUGIN_AUTHOR[] = "xbatista"
new PLUGIN_VERSION[] = "1.0"

#define MAX_MONSTERS 14

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
	"models/tentacle.mdl",
	"models/babygarg.mdl",
	"models/bigrat.mdl"
}
new const Monster_Xp[MAX_MONSTERS] =
{
	150,
	600,
	100,
	120,
	0,
	50,
	0,
	120,
	0,
	80,
	0,
	0,
	0,
	0
}
new const Monster_Coins[MAX_MONSTERS] =
{
	20,
	70,
	10,
	20,
	0,
	3,
	0,
	25,
	0,
	15,
	0,
	0,
	0,
	0
}
new const Monster_Names[MAX_MONSTERS][] =
{
	"異型戰士",
	"大媽 (王)",
	"鱷魚",
	"首腦",
	"巨人 (王)",
	"食腦蟲",
	"百募狗",
	"弗地崗人",
	"聖甲蟲",
	"殭屍",
	"人類戰士",
	"鷹爪",
	"小型巨人 (王)",
	"老鼠"
}

new g_iMaxPlayers;

public plugin_init() 
{
	register_plugin(PLUGIN_NAME, PLUGIN_VERSION, PLUGIN_AUTHOR)

	RegisterHam(Ham_Killed, "func_wall", "Monster_Killed");

	register_touch( COINS_CLASSNAME, "player", "Coins_Pickup")

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

	new MonsterMdl[32];

	entity_get_string( this, EV_SZ_model, MonsterMdl, charsmax(MonsterMdl) );

	for(new monsters = 0; monsters < MAX_MONSTERS; monsters++) 
	{
		if( equal( MonsterMdl, Monster_Models[monsters] ) ) 
		{
			if ( Monster_Xp[monsters] > 0 )
			{
				set_p_xp( idattacker, get_p_xp(idattacker) + Monster_Xp[monsters]);
				client_print( idattacker, print_center, "你殺了 %s, +%d經驗", Monster_Names[monsters], Monster_Xp[monsters]);
			}

			if ( Monster_Coins[monsters] > 0 )
				drop_coins( this, COINS_CLASSNAME, Monster_Coins[monsters] + (get_p_level(idattacker) / 4) );
		}
	}

	return HAM_IGNORED;
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