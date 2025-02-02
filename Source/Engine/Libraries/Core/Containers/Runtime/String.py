class StringPrinter:
    def __init__(self, val):
        self.val = val

    def to_string (self):
        return "String \"" + self.val['backingArray'] +  "\""

def lookup_type (val):
    if str(val.type) == 'Djinn::Core::String':
        return StringPrinter(val)
    return None

gdb.pretty_printers.append(lookup_type)