/**
 * bcd2gmdic.ino - BCD396XT to GM DIC Arduino firmware
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

#include <SPI.h>
#include <MCP2515.h>
#include <SoftwareSerial.h>
#include "bcd2gmdic.h"

static SoftwareSerial sSerial(8, 9);

static TalkgroupInfo gCurrentTalkgroup;

void abort(char *msg)
{
    Serial.println(msg);
    //Serial.println(MCP2515::getCANTxErrCnt(), HEX);
    //while (true);
}

void printTgid(TalkgroupInfo talkgroup)
{
    Serial.print("TGID: ");
    Serial.print(talkgroup.tgid);
    Serial.print(", Group: ");
    Serial.print(talkgroup.group);
    Serial.print(", Name: ");
    Serial.println(talkgroup.name);
}

void setDic(String message)
{
    byte data[16];
    byte data1[8];
    byte data2[8];
    memset(data, 0x20, sizeof(data));
    message.getBytes(data, sizeof(message));

    memcpy(data1, data, 8);
    memcpy(data2, data + 8, 8);

    Serial.println((char*)data1);
    Serial.println((char*)data2);

    CANMSG txmsg;
    txmsg.isExtendedAdrs = false;
    txmsg.rtr = false;
    txmsg.dataLength = 8;
    memcpy(txmsg.data, data1, 8);
    txmsg.adrsValue = 142;

    if (!MCP2515::transmitCANMessage(txmsg, 100))
        abort("Failed to Tx, 142");

    memcpy(txmsg.data, data2, 8);
    txmsg.adrsValue = 148;

    if (!MCP2515::transmitCANMessage(txmsg, 100))
        abort("Failed to Tx, 148");
}

void requestTgid()
{
    sSerial.print("GID\r");

    delay(25);

    String tgid = "";
    String group = "";
    String name = "";

    int8_t field = 0;
    char inChar;
    while (sSerial.available())
    {
        inChar = sSerial.read();
        if (inChar > 127)
        {
            continue;
        }

        if (inChar == ',')
        {
            field++;
            continue;
        }
        else if (inChar == '\r')
        {
            sSerial.flush();
            break;
        }

        if (field == 2)  // TGID
        {
            tgid += inChar;
        }
        else if (field == 5)
        {
            group += inChar;
        }
        else if (field == 6)
        {
            name += inChar;
        }
    }

    if (tgid != "")
    {
        gCurrentTalkgroup.tgid = tgid;
        gCurrentTalkgroup.group = group;
        gCurrentTalkgroup.name = name;
        printTgid(gCurrentTalkgroup);
        setDic(gCurrentTalkgroup.name);
    }
}

void setup()
{
    Serial.begin(115200);

    if (!MCP2515::initCAN(CAN_BAUD_500K))
        abort("Failed initCAN");

    if (!MCP2515::setCANNormalMode(false))
        abort("Failed CANNormalMode");

    sSerial.begin(38400);
}

void loop()
{
    requestTgid();

    // for testing can
    //setDic("29FBPDIS");
    //delay(1000);
}
