#!/bin/python3

import os

x = 1
print("Here")
pid = os.fork()
x += 1
if pid == 0:
  x += 1
  print(x)
else:
  print(x)
