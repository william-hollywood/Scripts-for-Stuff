path = "C:\Program Files (x86)\Steam\steamapps\common\Risk of Rain 2\Risk of Rain 2_Data\StreamingAssets\Language\en\\"

def process(fname):
    src = path + fname

    f = open(src, "r")
    lines = f.readlines()

    for i in range(len(lines)):
        if "_DESC" in lines[i]:
            lines[i-1] = lines[i].replace("_DESC", "_PICKUP")

    f.close()

    # easiest way to open the file at the beginning again
    f = open(src, "w")
    f.writelines(lines)
    f.close()

process("Items.txt")
process("Equipment.txt")