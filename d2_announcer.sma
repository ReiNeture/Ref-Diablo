#include <amxmodx>
#include <d2lod>

new PLUGIN_NAME[] = "Diablo II LOD 登入資訊"
new PLUGIN_AUTHOR[] = "xbatista"
new PLUGIN_VERSION[] = "1.0"

new const HEROES[][] = 
{
    "可莉",
    "錦木千束",
    "法師",
    "野蠻人",
    "聖劍士",
    "德魯伊",
    "術士"
} 

public plugin_init() 
{
	register_plugin(PLUGIN_NAME, PLUGIN_VERSION, PLUGIN_AUTHOR)
}
public d2_logged(id, log_type)
{
	static name[32] ; get_user_name(id, name, charsmax(name));

	if ( log_type == LOGGED )
	{
		client_printcolor( 0, "/y[ /g%s /y] - /ctr%s /y已登入.", name, HEROES[ get_p_hero(id) ]);
	}
	else
	{
		client_printcolor( 0, "/y[ /g%s /y] - /ctr%s /y已登出.", name, HEROES[ get_p_hero(id) ]);
	}
}