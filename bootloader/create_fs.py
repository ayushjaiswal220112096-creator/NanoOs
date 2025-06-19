import struct

# Define file entries (name, start_sector, size)
files = [
    ("readme.txt", 21, len("This is NanoOS!\n")),
    ("hello.txt", 22, len("Hello from file system!\n"))
]

# Write fakedir.img
with open("src/fakedir.img", "wb") as f:
    for name, start, size in files:
        name_bytes = name.encode("ascii").ljust(12, b'\x00')
        f.write(struct.pack("<12sII", name_bytes, start, size))
    # Pad rest of 512 bytes
    f.write(b'\x00' * (512 - len(files) * 20))

# Write file contents to separate images
with open("bin/filedata.img", "wb") as f:
    f.seek((21 - 1) * 512)  # skip to sector 21
    f.write(b"This is NanoOS!\n")
    f.seek((22 - 1) * 512)
    f.write(b"Hello from file system!\n")
