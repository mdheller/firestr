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

#ifndef FIRESTR_GUI_UNKNOWN_MESSAGE_H
#define FIRESTR_GUI_UNKNOWN_MESSAGE_H

#include <string>

#include <QWidget>
#include <QLabel>
namespace fire
{
    namespace gui
    {
        class unknown_message : public QWidget
        {
            Q_OBJECT

            public:
                unknown_message(const std::string&);

            private:
                QLabel* _text;
        };
    }
}

#endif
