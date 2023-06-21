#include <amxmodx>
#include <fakemeta>

#define PLUGIN_NAME "觀測者"
#define PLUGIN_VERSION "1.0"
#define PLUGIN_AUTHOR "Reff"

public plugin_init()
{
    register_plugin(PLUGIN_NAME, PLUGIN_VERSION, PLUGIN_AUTHOR);
    register_forward(FM_PlayerPreThink, "fw_PlayerPreThink");
}

public fw_PlayerPreThink(id)
{
    if ( !is_user_connected(id) )
		return FMRES_IGNORED;

	static target, body;
    static szName[32];
    static healths;

	get_user_aiming(id, target, body)

	if ( pev_valid(target) )
	{
        pev(target, pev_classname, szName, 31);
        healths = pev(target, pev_health);
		client_print(id, print_center, "Id:%s, Classname:%s, Health:%d", target, szName, healths);
	}

    return FMRES_IGNORED;
}