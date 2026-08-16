import os
import random
import string

os.makedirs("tests/scenario1", exist_ok=True)
os.makedirs("tests/scenario2", exist_ok=True)

pattern = "AAAAABBBBBCCCCCDDDDD"
with open("tests/scenario2/repetitive_1mb.txt", "w", encoding="utf-8") as f:
    f.write((pattern * 53000)[:1024 * 1024])

with open("tests/scenario2/random_1mb.txt", "w", encoding="utf-8") as f:
    f.write(''.join(random.choices(string.ascii_letters + string.digits, k=1024 * 1024)))

print("-> Đã khởi tạo xong các bộ dữ liệu test trong thư mục tests/")