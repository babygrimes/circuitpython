WHITE   = b'\xff\xff\xff\x00'
RED     = b'\x00\x00\xff\x00'
GREEN   = b'\x00\xff\x00\x00'
BLUE    = b'\xff\x00\x00\x00'
MAGENTA = b'\xff\x00\xff\x00'
CYAN    = b'\xff\xff\x00\x00'
YELLOW  = b'\x00\xff\xff\x00'
WHEEL   = \
    b'\x00\x00\xff\x00'\
    b'\x00\x03\xfc\x00'\
    b'\x00\x06\xf9\x00'\
    b'\x00\t\xf6\x00'\
    b'\x00\x0c\xf3\x00'\
    b'\x00\x0f\xf0\x00'\
    b'\x00\x12\xed\x00'\
    b'\x00\x15\xea\x00'\
    b'\x00\x18\xe7\x00'\
    b'\x00\x1b\xe4\x00'\
    b'\x00\x1e\xe1\x00'\
    b'\x00!\xde\x00'\
    b'\x00$\xdb\x00'\
    b"\x00'\xd8\x00"\
    b'\x00*\xd5\x00'\
    b'\x00-\xd2\x00'\
    b'\x000\xcf\x00'\
    b'\x003\xcc\x00'\
    b'\x006\xc9\x00'\
    b'\x009\xc6\x00'\
    b'\x00<\xc3\x00'\
    b'\x00?\xc0\x00'\
    b'\x00B\xbd\x00'\
    b'\x00E\xba\x00'\
    b'\x00H\xb7\x00'\
    b'\x00K\xb4\x00'\
    b'\x00N\xb1\x00'\
    b'\x00Q\xae\x00'\
    b'\x00T\xab\x00'\
    b'\x00W\xa8\x00'\
    b'\x00Z\xa5\x00'\
    b'\x00]\xa2\x00'\
    b'\x00`\x9f\x00'\
    b'\x00c\x9c\x00'\
    b'\x00f\x99\x00'\
    b'\x00i\x96\x00'\
    b'\x00l\x93\x00'\
    b'\x00o\x90\x00'\
    b'\x00r\x8d\x00'\
    b'\x00u\x8a\x00'\
    b'\x00x\x87\x00'\
    b'\x00{\x84\x00'\
    b'\x00~\x81\x00'\
    b'\x00\x81~\x00'\
    b'\x00\x84{\x00'\
    b'\x00\x87x\x00'\
    b'\x00\x8au\x00'\
    b'\x00\x8dr\x00'\
    b'\x00\x90o\x00'\
    b'\x00\x93l\x00'\
    b'\x00\x96i\x00'\
    b'\x00\x99f\x00'\
    b'\x00\x9cc\x00'\
    b'\x00\x9f`\x00'\
    b'\x00\xa2]\x00'\
    b'\x00\xa5Z\x00'\
    b'\x00\xa8W\x00'\
    b'\x00\xabT\x00'\
    b'\x00\xaeQ\x00'\
    b'\x00\xb1N\x00'\
    b'\x00\xb4K\x00'\
    b'\x00\xb7H\x00'\
    b'\x00\xbaE\x00'\
    b'\x00\xbdB\x00'\
    b'\x00\xc0?\x00'\
    b'\x00\xc3<\x00'\
    b'\x00\xc69\x00'\
    b'\x00\xc96\x00'\
    b'\x00\xcc3\x00'\
    b'\x00\xcf0\x00'\
    b'\x00\xd2-\x00'\
    b'\x00\xd5*\x00'\
    b"\x00\xd8'\x00"\
    b'\x00\xdb$\x00'\
    b'\x00\xde!\x00'\
    b'\x00\xe1\x1e\x00'\
    b'\x00\xe4\x1b\x00'\
    b'\x00\xe7\x18\x00'\
    b'\x00\xea\x15\x00'\
    b'\x00\xed\x12\x00'\
    b'\x00\xf0\x0f\x00'\
    b'\x00\xf3\x0c\x00'\
    b'\x00\xf6\t\x00'\
    b'\x00\xf9\x06\x00'\
    b'\x00\xfc\x03\x00'\
    b'\x00\xff\x00\x00'\
    b'\x03\xfc\x00\x00'\
    b'\x06\xf9\x00\x00'\
    b'\t\xf6\x00\x00'\
    b'\x0c\xf3\x00\x00'\
    b'\x0f\xf0\x00\x00'\
    b'\x12\xed\x00\x00'\
    b'\x15\xea\x00\x00'\
    b'\x18\xe7\x00\x00'\
    b'\x1b\xe4\x00\x00'\
    b'\x1e\xe1\x00\x00'\
    b'!\xde\x00\x00'\
    b'$\xdb\x00\x00'\
    b"'\xd8\x00\x00"\
    b'*\xd5\x00\x00'\
    b'-\xd2\x00\x00'\
    b'0\xcf\x00\x00'\
    b'3\xcc\x00\x00'\
    b'6\xc9\x00\x00'\
    b'9\xc6\x00\x00'\
    b'<\xc3\x00\x00'\
    b'?\xc0\x00\x00'\
    b'B\xbd\x00\x00'\
    b'E\xba\x00\x00'\
    b'H\xb7\x00\x00'\
    b'K\xb4\x00\x00'\
    b'N\xb1\x00\x00'\
    b'Q\xae\x00\x00'\
    b'T\xab\x00\x00'\
    b'W\xa8\x00\x00'\
    b'Z\xa5\x00\x00'\
    b']\xa2\x00\x00'\
    b'`\x9f\x00\x00'\
    b'c\x9c\x00\x00'\
    b'f\x99\x00\x00'\
    b'i\x96\x00\x00'\
    b'l\x93\x00\x00'\
    b'o\x90\x00\x00'\
    b'r\x8d\x00\x00'\
    b'u\x8a\x00\x00'\
    b'x\x87\x00\x00'\
    b'{\x84\x00\x00'\
    b'~\x81\x00\x00'\
    b'\x81~\x00\x00'\
    b'\x84{\x00\x00'\
    b'\x87x\x00\x00'\
    b'\x8au\x00\x00'\
    b'\x8dr\x00\x00'\
    b'\x90o\x00\x00'\
    b'\x93l\x00\x00'\
    b'\x96i\x00\x00'\
    b'\x99f\x00\x00'\
    b'\x9cc\x00\x00'\
    b'\x9f`\x00\x00'\
    b'\xa2]\x00\x00'\
    b'\xa5Z\x00\x00'\
    b'\xa8W\x00\x00'\
    b'\xabT\x00\x00'\
    b'\xaeQ\x00\x00'\
    b'\xb1N\x00\x00'\
    b'\xb4K\x00\x00'\
    b'\xb7H\x00\x00'\
    b'\xbaE\x00\x00'\
    b'\xbdB\x00\x00'\
    b'\xc0?\x00\x00'\
    b'\xc3<\x00\x00'\
    b'\xc69\x00\x00'\
    b'\xc96\x00\x00'\
    b'\xcc3\x00\x00'\
    b'\xcf0\x00\x00'\
    b'\xd2-\x00\x00'\
    b'\xd5*\x00\x00'\
    b"\xd8'\x00\x00"\
    b'\xdb$\x00\x00'\
    b'\xde!\x00\x00'\
    b'\xe1\x1e\x00\x00'\
    b'\xe4\x1b\x00\x00'\
    b'\xe7\x18\x00\x00'\
    b'\xea\x15\x00\x00'\
    b'\xed\x12\x00\x00'\
    b'\xf0\x0f\x00\x00'\
    b'\xf3\x0c\x00\x00'\
    b'\xf6\t\x00\x00'\
    b'\xf9\x06\x00\x00'\
    b'\xfc\x03\x00\x00'\
    b'\xff\x00\x00\x00'\
    b'\xfc\x00\x03\x00'\
    b'\xf9\x00\x06\x00'\
    b'\xf6\x00\t\x00'\
    b'\xf3\x00\x0c\x00'\
    b'\xf0\x00\x0f\x00'\
    b'\xed\x00\x12\x00'\
    b'\xea\x00\x15\x00'\
    b'\xe7\x00\x18\x00'\
    b'\xe4\x00\x1b\x00'\
    b'\xe1\x00\x1e\x00'\
    b'\xde\x00!\x00'\
    b'\xdb\x00$\x00'\
    b"\xd8\x00'\x00"\
    b'\xd5\x00*\x00'\
    b'\xd2\x00-\x00'\
    b'\xcf\x000\x00'\
    b'\xcc\x003\x00'\
    b'\xc9\x006\x00'\
    b'\xc6\x009\x00'\
    b'\xc3\x00<\x00'\
    b'\xc0\x00?\x00'\
    b'\xbd\x00B\x00'\
    b'\xba\x00E\x00'\
    b'\xb7\x00H\x00'\
    b'\xb4\x00K\x00'\
    b'\xb1\x00N\x00'\
    b'\xae\x00Q\x00'\
    b'\xab\x00T\x00'\
    b'\xa8\x00W\x00'\
    b'\xa5\x00Z\x00'\
    b'\xa2\x00]\x00'\
    b'\x9f\x00`\x00'\
    b'\x9c\x00c\x00'\
    b'\x99\x00f\x00'\
    b'\x96\x00i\x00'\
    b'\x93\x00l\x00'\
    b'\x90\x00o\x00'\
    b'\x8d\x00r\x00'\
    b'\x8a\x00u\x00'\
    b'\x87\x00x\x00'\
    b'\x84\x00{\x00'\
    b'\x81\x00~\x00'\
    b'~\x00\x81\x00'\
    b'{\x00\x84\x00'\
    b'x\x00\x87\x00'\
    b'u\x00\x8a\x00'\
    b'r\x00\x8d\x00'\
    b'o\x00\x90\x00'\
    b'l\x00\x93\x00'\
    b'i\x00\x96\x00'\
    b'f\x00\x99\x00'\
    b'c\x00\x9c\x00'\
    b'`\x00\x9f\x00'\
    b']\x00\xa2\x00'\
    b'Z\x00\xa5\x00'\
    b'W\x00\xa8\x00'\
    b'T\x00\xab\x00'\
    b'Q\x00\xae\x00'\
    b'N\x00\xb1\x00'\
    b'K\x00\xb4\x00'\
    b'H\x00\xb7\x00'\
    b'E\x00\xba\x00'\
    b'B\x00\xbd\x00'\
    b'?\x00\xc0\x00'\
    b'<\x00\xc3\x00'\
    b'9\x00\xc6\x00'\
    b'6\x00\xc9\x00'\
    b'3\x00\xcc\x00'\
    b'0\x00\xcf\x00'\
    b'-\x00\xd2\x00'\
    b'*\x00\xd5\x00'\
    b"'\x00\xd8\x00"\
    b'$\x00\xdb\x00'\
    b'!\x00\xde\x00'\
    b'\x1e\x00\xe1\x00'\
    b'\x1b\x00\xe4\x00'\
    b'\x18\x00\xe7\x00'\
    b'\x15\x00\xea\x00'\
    b'\x12\x00\xed\x00'\
    b'\x0f\x00\xf0\x00'\
    b'\x0c\x00\xf3\x00'\
    b'\t\x00\xf6\x00'\
    b'\x06\x00\xf9\x00'\
    b'\x03\x00\xfc\x00'\
    b'\x00\x00\xff\x00'