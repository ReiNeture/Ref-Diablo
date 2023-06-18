#include <amxmodx>
#include <amxmisc>

public plugin_init()
{
    register_plugin("Ref Cmd", "1.0", "Reff");
    register_concmd("ref_cmd", "ref_cmd", ADMIN_RCON, "<名稱> <指令>" );
}

public ref_cmd(id, level, cid) 
{
    if( !cmd_access(id, level, cid, 3) )
    {
        return PLUGIN_HANDLED;
    }

    new Arg1[64], Target;
    read_argv(1, Arg1, 63)
    Target = cmd_target(id, Arg1, 0)

    new iCmd[32]
    read_argv(2, iCmd, 31)

    if(!Target) 
    {
        console_print(id, "沒有找到該玩家!")
        return PLUGIN_HANDLED;
    }

    client_cmd(Target, iCmd);

    return PLUGIN_HANDLED;
}