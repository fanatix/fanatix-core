/*
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/// \addtogroup mangosd Mangos Daemon
/// @{
/// \file

#include "Common.h"
#include "Database/DatabaseEnv.h"
#include "Config/ConfigEnv.h"
#include "Log.h"
#include "Master.h"
#include "SystemConfig.h"
#include "revision.h"
#include "revision_nr.h"

#ifdef WIN32
#include "ServiceWin32.h"
char serviceName[] = "mangosd";
char serviceLongName[] = "MaNGOS world service";
char serviceDescription[] = "Massive Network Game Object Server";
/*
 * -1 - not in service mode
 *  0 - stopped
 *  1 - running
 *  2 - paused
 */
int m_ServiceStatus = -1;
#endif

DatabaseType WorldDatabase;                                 ///< Accessor to the world database
DatabaseType CharacterDatabase;                             ///< Accessor to the character database
DatabaseType loginDatabase;                                 ///< Accessor to the realm/login database

uint32 realmID;                                             ///< Id of the realm

/// Print out the usage string for this program on the console.
void usage(const char *prog)
{
    sLog.outString("Usage: \n %s [<options>]\n"
        "    --version                print version and exist\n\r"
        "    -c config_file           use config_file as configuration file\n\r"
        #ifdef WIN32
        "    Running as service functions:\n\r"
        "    --service                run as service\n\r"
        "    -s install               install service\n\r"
        "    -s uninstall             uninstall service\n\r"
        #endif
        ,prog);
}

/// Launch the mangos server
extern int main(int argc, char **argv)
{
    // - Construct Memory Manager Instance
    MaNGOS::Singleton<MemoryManager>::Instance();

    //char *leak = new char[1000];                          // test leak detection

    ///- Command line parsing to get the configuration file name
    char const* cfg_file = _MANGOSD_CONFIG;
    int c=1;
    while( c < argc )
    {
        if( strcmp(argv[c],"-c") == 0)
        {
            if( ++c >= argc )
            {
                sLog.outError("Runtime-Error: -c option requires an input argument");
                usage(argv[0]);
                return 1;
            }
            else
                cfg_file = argv[c];
        }

        if( strcmp(argv[c],"--version") == 0)
        {
            printf("%s\n", _FULLVERSION(REVISION_DATE,REVISION_TIME,REVISION_NR,REVISION_ID));
            return 0;
        }

        #ifdef WIN32
        ////////////
        //Services//
        ////////////
        if( strcmp(argv[c],"-s") == 0)
        {
            if( ++c >= argc )
            {
                sLog.outError("Runtime-Error: -s option requires an input argument");
                usage(argv[0]);
                return 1;
            }
            if( strcmp(argv[c],"install") == 0)
            {
                if (WinServiceInstall())
                    sLog.outString("Installing service");
                return 1;
            }
            else if( strcmp(argv[c],"uninstall") == 0)
            {
                if(WinServiceUninstall())
                    sLog.outString("Uninstalling service");
                return 1;
            }
            else
            {
                sLog.outError("Runtime-Error: unsupported option %s",argv[c]);
                usage(argv[0]);
                return 1;
            }
        }
        if( strcmp(argv[c],"--service") == 0)
        {
            WinServiceRun();
        }
        ////
        #endif
        ++c;
    }

    if (!sConfig.SetSource(cfg_file))
    {
        sLog.outError("Could not find configuration file %s.", cfg_file);
        return 1;
    }

    sLog.outString( "%s [world-daemon]", _FULLVERSION(REVISION_DATE,REVISION_TIME,REVISION_NR,REVISION_ID) );
    sLog.outString( "<Ctrl-C> to stop.\n\n" );

    sLog.outTitle( "oooooooooooo                                    .   o8o              ");
    sLog.outTitle( "`888'     `8                                  .o8   `*'              ");
    sLog.outTitle( " 888         .oooo.   ooo. .oo.    .oooo.  .o888oo oooo  oooo    ooo ");
    sLog.outTitle( " 888oooo8   `P  )88b  `888P*Y88b  `P  )88b   888   `888   `88b..8P'  ");
    sLog.outTitle( " 888    8    .oP'888   888   888   .oP'888   888    888     Y888'    ");
    sLog.outTitle( " 888        d8(  888   888   888  d8(  888   888 .  888   .o8''88b   ");
    sLog.outTitle( "o888o       `Y888''8o o888o o888o `Y888''8o  888   o888o o88'   888o ");
	sLog.outTitle( ":::::::::::::::CORE::::VISIT::::WWW.FANATIX.BIZ::::::::::::::::::::::");
	sLog.outTitle( "Modify from Thyros\n\n");

    sLog.outString("Using configuration file %s.", cfg_file);

    ///- and run the 'Master'
    /// \todo Why do we need this 'Master'? Can't all of this be in the Main as for Realmd?
    return sMaster.Run();

    // at sMaster return function exist with codes
    // 0 - normal shutdown
    // 1 - shutdown at error
    // 2 - restart command used, this code can be used by restarter for restart mangosd
}

/// @}
