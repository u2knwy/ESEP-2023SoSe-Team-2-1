#! python3
"""
Functions for interpretation of data excanged with Fest Transfer System.
'In' and 'Out' are from the perspective of the controlling computer
system.
"""

in_signals = [
["Lightbarrier Begin", 0x0001, 0, [[0, 'i', "interrupted"], [1, 'c', "closed"]]],
["Lightbarrier Hight Measurement", 0x0002, 0, [[0, 'i', "interrupted"], [1, 'c', "closed"]]],
["Hight Correct Sensor", 0x0004, 0, [[0, 'g', "correct height"], [1, 'w', "wrong height"]]],
["Lightbarrier Feed Separator", 0x0008, 0, [[0, 'i', "interrupted"], [1, 'c', "closed"]]],
["Metal Sensor", 0x0010, 0, [[0, 'n', "non-metal"], [1, 'm', "metal"]]],
["Feed Separator Sensor", 0x0020, 0, [[0, 'c', "closed"], [1, 'o', "open"]]],
["Lightbarrier Slide", 0x0040, 0, [[0, 'i', "interrupted"], [1, 'c', "closed"]]],
["Lightbarrier End", 0x0080, 0, [[0, 'i', "interrupted"], [1, 'c', "closed"]]],
["Button Start", 0x0100, 0, [[1, 'p', "pressed"], [0, 'r', "released"]]],
["Button Stop", 0x0200, 0, [[0, 'p', "pressed"], [1, 'r', "released"]]],
["Button Reset", 0x0400, 0, [[1, 'p', "pressed"], [0, 'r', "released"]]],
["Button E-Stop", 0x0800, 0, [[0, 'p', "pressed"], [1, 'r', "released"]]]
]

out_signals = [
["Belt", 0x000F, 0, [[0, 'st', "stop"],
[1, 'r ', "moves right"],
[2, 'l ', "moves left"],
[3, 'il', "illegal"],
[5, 'sr', "moves slow right"],
[6, 'sl', "moves slow left"],
[7, 'il', "illegal"],
[8, 'sc', "stop command"],
[9, 'sc', "stop command"],
[10, 'sc', "stop command"],
[11, 'sc', "stop command"],
[12, 'sc', "stop command"],
[13, 'sc', "stop command"],
[14, 'sc', "stop command"],
[15, 'sc', "stop command"],
]],
["Lamp Red", 0x0010, 0, [[0, 'o', "off"], [1, 'e', "enlighted"]]],
["Lamp Yellow", 0x020, 0, [[0, 'o', "off"], [1, 'e', "enlighted"]]],
["Lamp Green", 0x0040, 0, [[0, 'o', "off"], [1, 'e', "enlighted"]]],
["Feed Separator Open", 0x0080, 0, [[0, 'c', "closed"], [1, 'o', "open"]]],
["LED Start Button", 0x0100, 0, [[0, 'o', "off"], [1, 'e', "enlighted"]]],
["LED Reset Button", 0x0200, 0, [[0, 'o', "off"], [1, 'e', "enlighted"]]],
["LED Q1", 0x0400, 0, [[0, 'o', "off"], [1, 'e', "enlighted"]]],
["LED Q2", 0x0800, 0, [[0, 'o', "off"], [1, 'e', "enlighted"]]]
]

def a_b_c2in_out(a, b, c):
    """
    Translate the old register expression into the new in/out representation.
    """
    value_in = b | (c & 0xF0) << 4
    value_out = a | (c & 0x0F) << 8
    return value_in, value_out

def in_out2a_b_c(value_in, value_out):
    """
    Translates the new in/out representation into the old A, B, C register representation.
    """
    a = value_out & 0x00FF
    b = value_in & 0x00FF
    c = ((value_in & 0x0F00) >> 4) | ((value_out & 0x0F00) >> 8)
    return a, b, c

def parse(reg, inout):
    if inout == 'in':
        signals = in_signals
    elif inout == 'out':
        signals = out_signals
    else:
        return []
    parsed = []
    #print(bin(reg))
    for signal in signals:
        mask =  signal[1]
        masked = reg & signal[1]
        #print(mask,masked  )
        while (mask & 0x01) != 1:
            mask = mask >> 1
            masked = masked >> 1
        #print(signal[0], mask, masked)
        for values in signal[3]:
            if values[0] == masked:
                description = values
                #print(values)
        parsed.append([signal[0], masked, description])
    return parsed

def show_ascii_headline(indent=""):
    print(generate_ascii_headline(indent))

def generate_ascii_headline(indent=""):
    result = ""
    names_in = []
    max_length = 0
    for signal in in_signals:
        names_in.append(signal[0])
        max_length = max(max_length, len(signal[0]))
    names_out = []
    for signal in out_signals:
        names_out.append(signal[0])
        max_length = max(max_length, len(signal[0]))
    #print(max_length)
    names_in = [x.rjust(max_length+1) for x in names_in]
    names_out = [x.rjust(max_length+1) for x in names_out]
    #print(names_in)
    for pos in range(1, max_length+1):
        result = result + indent
        #print(indent, end="")
        for sig in names_in:
            result = result + sig[pos] + " "
            #print(sig[pos], " ", end="")
        #print(" ", end="")
        result = result + " "
        for sig in names_out:
            result = result + sig[pos] + " "
            #print(sig[pos], " ", end="")
        result = result + "\n"
        #print("")
    return result

def show_ascii(parsed_signals):
    print(generate_ascii(parsed_signals), end="")

def generate_ascii(parsed_signals):
    result = ""
    for signal in parsed_signals:
        result = result + signal[2][1] + " "
        #print(signal[2][1], " ", end ="")
    return result

def show_signals():
    print("##In-Signals##")
    for signal in in_signals:
        print("###{}###".format(signal[0]))
        print("|value|short|long|")
        print("|----:|----:|:---|")
        for value in signal[3]:
            print("|{}|{}|{}|".format(str(value[0]).rjust(5), value[1].rjust(5), value[2]))
    print("##Out-Signals##")
    for signal in out_signals:
        print("###{}###".format(signal[0]))
        print("|value|short|long|")
        print("|----:|----:|:---|")
        for value in signal[3]:
            print("|{}|{}|{}|".format(str(value[0]).rjust(5), value[1].rjust(5), value[2]))

if __name__ == '__main__':
    show_signals()
    # test value translation
    a = 0x81
    b = 0xC3
    c = 0xA5
    v_in, v_out = a_b_c2in_out(a, b, c)
    print(hex(v_in), hex(v_out))
    a, b, c = in_out2a_b_c(v_in, v_out)
    print(hex(a), hex(b), hex(c))

    # test the translation into signal names
    print(parse(v_in, 'in'))
    print(parse(v_out, 'out'))

    # show values on terminal
    show_ascii_headline()
    show_ascii(parse(v_in, 'in'))
    show_ascii(parse(v_out, 'out'))
    print("")
