#ifndef __OPCODE_H
#define __OPCODE_H

namespace packet
{
    enum  opcode //: unsigned short
    {
        SC_ERROR = 1800,
        CS_LOG_IN = 2000,
        SC_LOG_IN = 2001,
        CS_LOG_OUT = 2002,
        SC_LOG_OUT = 2003,
        SC_ENTER_FIELD = 3000,
        CS_ENTER_FIELD = 3001,
        CS_LEAVE_FIELD = 3002,
        SC_LEAVE_FIELD = 3003,
        CS_MOVE_CHARACTER = 3010,
        SC_MOVE_CHARACTER = 3011,
        CS_CAST_SKILL = 4000,
        SC_CAST_SKILL = 4001
    };
}

#endif
