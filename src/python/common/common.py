# https://docs.python.org/3/library/struct.html
import struct


FLOAT_BE = ">f"
FLOAT_LE = "<f"
DOUBLE_BE = ">d"
DOUBLE_LE = "<d"

def np_value_to_hex(value, byte_format):
    return bytearray(struct.pack(byte_format, value)).hex()

# byte_format is target format for output
def np_array_to_hex(array, byte_format):
    return map(
        lambda layer: list(
            map(lambda v: np_value_to_hex(v, byte_format), layer)
        ),
        array,
    )