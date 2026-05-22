A = 1
C = 100e-9
f = 980

R=1/(4*A*f*C)
print(f"R = {R:.2f} Ohms")

RF=8*A*R
print(f"RF = {RF:.2f} Ohms")

Rp = 1/(1/R + 1/RF)
print(f"Rp = {Rp:.2f} Ohms")