/*
 * Copyright (C) 2014  Maxim Noah Khailo
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
 */

#include <QtWidgets>

#include "gui/app/chat.hpp"
#include "gui/util.hpp"
#include "util/dbc.hpp"
#include "util/log.hpp"
#include "util/time.hpp"
#include "util/uuid.hpp"

#include <QTimer>

namespace m = fire::message;
namespace ms = fire::messages;
namespace us = fire::user;
namespace s = fire::session;
namespace u = fire::util;

namespace fire
{
    namespace gui
    {
        namespace app
        {
            const std::string CHAT = "CHAT_APP";

            namespace
            {
                const size_t TIMER_SLEEP = 100;//in milliseconds
                const size_t PADDING = 20;
                const std::string MESSAGE = "m";
            }

            struct text_message
            {
                std::string from_id;
                std::string text;
            };

            m::message convert(const text_message& t)
            {
                m::message m;
                m.meta.type = MESSAGE;
                m.data = u::to_bytes(t.text);

                return m;
            }

            void convert(const m::message& m, text_message& t)
            {
                REQUIRE_EQUAL(m.meta.type, MESSAGE);
                t.from_id = m.meta.extra["from_id"].as_string();
                t.text = u::to_str(m.data);
            }

            QWidget* make_message_widget(const std::string& name, const std::string& text)
            {
                std::stringstream s;
                s << "<font color='gray'>" << u::timestamp() << "</font> <b>" << name << "</b>: " << text;
                return new QLabel{s.str().c_str()};
            }

            chat_app::chat_app(
                    s::session_service_ptr session_s,
                    s::session_ptr session) :
                message{},
                _id{u::uuid()},
                _session_service{session_s},
                _session{session}
            {
                REQUIRE(session_s);
                REQUIRE(session);
                init();
            }

            chat_app::chat_app(
                    const std::string& id, 
                    s::session_service_ptr session_s,
                    s::session_ptr session) :
                message{},
                _id{id},
                _session_service{session_s},
                _session{session}
            {
                REQUIRE(session_s);
                REQUIRE(session);
                init();
            }

            chat_app::~chat_app()
            {
                INVARIANT(_session_service);
                INVARIANT(_session);
            }

            void chat_app::init()
            {
                INVARIANT(root());
                INVARIANT(layout());
                INVARIANT(_session);

                _mail = std::make_shared<m::mailbox>(_id);
                _sender = std::make_shared<ms::sender>(_session->user_service(), _mail);

                //message list
                _messages = new list;
                _messages->auto_scroll(true);
                layout()->addWidget(_messages, 0, 0, 1, 2);

                //text edit
                _message = new QLineEdit;
                layout()->addWidget(_message, 1, 0);

                //send button
                _send = new QPushButton{tr("send")};
                layout()->addWidget(_send, 1, 1);

                connect(_message, SIGNAL(returnPressed()), this, SLOT(send_message()));
                connect(_send, SIGNAL(clicked()), this, SLOT(send_message()));

                setMinimumHeight(layout()->sizeHint().height() + PADDING);

                //if no contacts added, disable app
                if(_session->contacts().empty())
                {
                    _messages->add(make_message_widget(gui::convert(tr("alert")), gui::convert(tr("no contacts in session"))));
                    _message->setEnabled(false);
                    _send->setEnabled(false);
                    return;
                }

                //setup message timer
                auto *t = new QTimer(this);
                connect(t, SIGNAL(timeout()), this, SLOT(check_mail()));
                t->start(TIMER_SLEEP);

                INVARIANT(_session);
                INVARIANT(_mail);
                INVARIANT(_sender);
            }

            const std::string& chat_app::id()
            {
                ENSURE_FALSE(_id.empty());
                return _id;
            }

            const std::string& chat_app::type()
            {
                ENSURE_FALSE(CHAT.empty());
                return CHAT;
            }

            m::mailbox_ptr chat_app::mail()
            {
                ENSURE(_mail);
                return _mail;
            }

            void chat_app::send_message()
            {
                INVARIANT(_message);
                INVARIANT(_send);
                INVARIANT(_session);

                auto text = gui::convert(_message->text());
                _message->clear();

                auto self = _session->user_service()->user().info().name();
                _messages->add(make_message_widget(self, text));

                text_message tm;
                tm.text = text;

                bool sent = false;
                for(auto c : _session->contacts().list())
                {
                    CHECK(c);
                    if(!_session->user_service()->contact_available(c->id())) continue;
                    _sender->send(c->id(), convert(tm)); 
                    sent = true;
                }

                if(!sent) _messages->add(make_message_widget("app", "nobody here..."));
            }

            void chat_app::check_mail() 
            try
            {
                INVARIANT(_mail);
                INVARIANT(_session);
                INVARIANT(_session_service);

                m::message m;
                while(_mail->pop_inbox(m))
                {
                    if(m.meta.type == MESSAGE)
                    {
                        text_message t;
                        convert(m, t);

                        auto c = _session->contacts().by_id(t.from_id);
                        if(!c) continue;

                        _messages->add(make_message_widget(c->name(), t.text));
                        _messages->verticalScrollBar()->scroll(0, _messages->verticalScrollBar()->maximum());
                        _session_service->fire_session_alert(_session->id());
                    }
                    else
                    {
                        LOG << "chat sample recieved unknown message `" << m.meta.type << "'" << std::endl;
                    }
                }
            }
            catch(std::exception& e)
            {
                LOG << "chat_app: error in check_mail. " << e.what() << std::endl;
            }
            catch(...)
            {
                LOG << "chat_app: unexpected error in check_mail." << std::endl;
            }
        }
    }
}
