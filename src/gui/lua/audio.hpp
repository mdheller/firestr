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
 */

#ifndef FIRESTR_APP_LUA_AUDIO_H
#define FIRESTR_APP_LUA_AUDIO_H

#include "gui/lua/base.hpp"

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
            class microphone
            {
                public:
                    microphone(lua_api*, int id, const std::string& codec = "pcm");
                    ~microphone();
                    void stop();
                    void start();
                    QAudioInput* input();
                    QIODevice* io();
                    bool recording() const;
                    codec_type codec() const;
                    util::bytes encode(const util::bytes&);

                private:
                    QAudioFormat _f;
                    QAudioDeviceInfo _inf;
                    QAudioInput* _i;
                    codec_type _t;
                    int _id;
                    QIODevice* _d = nullptr;
                    bool _recording = false;
                    lua_api* _api;

                    //opus specific members
                    OpusEncoder* _opus = nullptr;
                    util::bytes _buffer;
                    size_t _skip = 0;
                    size_t _channels = 0;
            };
            using microphone_ptr = std::shared_ptr<microphone>;

            struct microphone_ref : public basic_ref
            {
                std::string callback;
                void set_callback(const std::string&);
                void stop();
                void start();
                microphone_ptr mic;
            };
            using microphone_ref_map = std::unordered_map<int, microphone_ref>;


            struct speaker 
            {
                public:
                    speaker(lua_api*, const std::string& code = "pcm");
                    ~speaker();
                    void mute();
                    void unmute();
                    void play(const bin_data&);
                    codec_type codec() const;
                    util::bytes decode(const util::bytes&);
                private:
                    bool _mute = false;
                    QAudioFormat _f;
                    codec_type _t;
                    QAudioOutput* _o;
                    QIODevice* _d = nullptr;
                    lua_api* _api;

                    //opus specific members
                    OpusDecoder* _opus = nullptr;
                    size_t _rep = 0;
                    size_t _channels = 0;
            };
            using speaker_ptr = std::shared_ptr<speaker>;

            struct speaker_ref : public basic_ref
            {
                void mute();
                void unmute();
                void play(const bin_data&);
                speaker_ptr spkr;
            };
            using speaker_ref_map = std::unordered_map<int, speaker_ref>;
            extern const size_t MAX_SAMPLE_BYTES;
        }
    }
}

#endif
