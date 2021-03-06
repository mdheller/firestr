/*
 * Copyright (C) 2014  Maxim Noah Khailo
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either vedit_refsion 3 of the License, or
 * (at your option) any later vedit_refsion.
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

#ifndef FIRESTR_APP_LUA_AUDIO_H
#define FIRESTR_APP_LUA_AUDIO_H

#include "gui/lua/base.hpp"
#include "util/audio.hpp"

#include <QAudioFormat>
#include <QAudioInput>
#include <QAudioOutput>

#include <string>
#include <unordered_map>
#include <opus/opus.h>

namespace fire
{
    namespace gui
    {
        namespace lua
        {
            enum codec_type { pcm, opus};

            struct microphone_ref : public basic_ref
            {
                std::string callback;
                void set_callback(const std::string&);
                void stop();
                void start();
            };
            using microphone_ref_map = std::unordered_map<int, microphone_ref>;


            struct speaker_ref : public basic_ref
            {
                void mute();
                void unmute();
                void play(const bin_data&);
            };

            using speaker_ref_map = std::unordered_map<int, speaker_ref>;
        }
    }
}

#endif
