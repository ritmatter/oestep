import os
import subprocess


pid = os.fork()
if pid == 0:
  os.write("file.txt", "Child")
else:
  os.write("file.txt", "Parent")

