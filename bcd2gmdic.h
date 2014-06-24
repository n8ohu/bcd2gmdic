/**
 * bcd2gmdic.h - BCD396XT to GM DIC Arduino firmware
 * Copyright (C) 2014 Jared Szechy
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

 #ifndef BCD2GMDIC_H
 #define BCD2GMDIC_H

 typedef struct {
     String tgid;
     String group;
     String name;
 } TalkgroupInfo;

 #endif
