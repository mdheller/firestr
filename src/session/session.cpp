/*
 * Copyright (C) 2012  Maxim Noah Khailo
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

#include "session/session.hpp"

#include "util/uuid.hpp"
#include "util/dbc.hpp"

namespace u = fire::util;
namespace us = fire::user;
namespace m = fire::message;
namespace ms = fire::messages;

namespace fire
{
    namespace session
    {
        session::session(
                us::user_service_ptr s,
                m::post_office_wptr pp) :
            _id{u::uuid()},
            _user_service{s},
            _parent_post{pp}
        {
            init();
        }

        session::session(
                const std::string id, 
                us::user_service_ptr s,
                m::post_office_wptr pp) :
            _id{id},
            _user_service{s},
            _parent_post{pp}
        {
            init();
        }

        void session::init()
        {
            INVARIANT_FALSE(_id.empty());
            INVARIANT(_user_service);

            _mail.reset(new m::mailbox{_id});
            _sender.reset(new ms::sender{_user_service, _mail});

            INVARIANT(_mail);
            INVARIANT(_sender);
        }

        const user::contact_list& session::contacts() const
        {
            return _contacts;
        }

        user::contact_list& session::contacts() 
        {
            return _contacts;
        }

        const std::string& session::id() const
        {
            ENSURE_FALSE(_id.empty());
            return _id;
        }

        m::post_office_wptr session::parent_post()
        {
            return _parent_post;
        }

        m::mailbox_ptr session::mail()
        {
            ENSURE(_mail);
            return _mail;
        }

        messages::sender_ptr session::sender()
        {
            ENSURE(_sender);
            return _sender;
        }

        user::user_service_ptr session::user_service()
        {
            ENSURE(_user_service);
            return _user_service;
        }
    }
}

