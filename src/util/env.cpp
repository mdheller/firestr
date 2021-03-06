/*
 * Copyright (C) 2017  Maxim Noah Khailo
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * In addition, as a special exception, the copyright holders give 
 * permission to link the code of portions of this program with the 
 * Botan library under certain conditions as described in each 
 * individual source file, and distribute linked combinations 
 * including the two.
 *
 * You must obey the GNU General Public License in all respects for 
 * all of the code used other than Botan. If you modify file(s) with 
 * this exception, you may extend this exception to your version of the 
 * file(s), but you are not obligated to do so. If you do not wish to do 
 * so, delete this exception statement from your version. If you delete 
 * this exception statement from all source files in the program, then 
 * also delete it here.
 */

#include "util/env.hpp"

#include "util/rand.hpp"

#include <cstdlib>
#include <boost/filesystem.hpp>

#ifdef __APPLE__
#include <QProcess>
#include <QString>
#include <QStringList>
#include <CoreServices/CoreServices.h>
#endif

namespace bf = boost::filesystem;

namespace fire::util
{
    std::string get_home_dir()
    {
#ifdef _WIN64
        const char* home = std::getenv("USERPROFILE");
#else
        const char* home = std::getenv("HOME");
#endif
        return home != nullptr ? home : ".";
    }

    std::string get_default_firestr_home()
    {
#ifdef _WIN64
        //root in /Users/<user>/Application Data/
        const char* app_data = std::getenv("APPDATA");
        const bf::path root = app_data != nullptr ? app_data : "./";
#elif __APPLE__
        const size_t MAX_SIZE = 1024;
        //something like ~/Library/Application Support/
        FSRef ref;
        OSType type = kApplicationSupportFolderType;
        char found_path[MAX_SIZE];

        FSFindFolder( kUserDomain, type, kCreateFolder, &ref );
        FSRefMakePath( &ref, (UInt8*)&found_path, MAX_SIZE );

        const bf::path root = found_path;
#else
        //~/.config/firestr
        const char* home = std::getenv("HOME");
        bf::path root = home != nullptr ? home : "./";
        root /= ".config";
#endif

        const bf::path r = root / "firestr";
        return r.string();
    }

    void setup_env()
    {
        init_rand();

#ifdef __APPLE__
        QString p = "defaults";
        QStringList a;
        a << "write" << "com.mempko.firestr" << "NSAppSleepDisabled" << "-bool" << "YES";

        QProcess m;
        m.start(p, a);
        m.waitForFinished();
#endif
    }
}
