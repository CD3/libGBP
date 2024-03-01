import math as m

import pint

ureg = pint.UnitRegistry()
Q_ = ureg.Quantity


def rayleigh(l, w0, n):
    return (n * m.pi * w0**2 / l).to("m")


def new_waist_pos(s, f, zR):
    return 1 / ((1 / f) + 1 / (s + zR**2 / (s - f)))


def magnification(s, f, zR):
    return f / ((s - f) ** 2 + zR**2) ** 0.5


def spherical_refractor_focal_length(R, n1, n2):
    return R * n2 / (n2 - n1)


def lens_makers(Rf, Rb, nl, nm=1):
    return lens_makers_thick(Rf, Rb, Q_(0, "cm"), nl, nm)


def lens_makers_thick(Rf, Rb, d, nl, nm=1):
    return 1 / ((nl / nm - 1) * (1 / Rf - 1 / Rb + (nl / nm - 1) * d / nl / Rf / Rb))


assert spherical_refractor_focal_length(Q_(10, "cm"), 1, 1.5).magnitude == 30


M2 = Q_(1, "")
l = Q_(532, "nm")
w0 = Q_(10, "mm")
z0 = Q_(0, "cm")


zR = rayleigh(l, w0, 1)

print("zR:", zR)

Rf = Q_(40, "mm")
Rb = Q_(-40, "mm")
ff = spherical_refractor_focal_length(Rf, 1, 1.5)
fb = spherical_refractor_focal_length(Rb, 1.5, 1)
print(f"f front({Rf})", ff)
print(f"f back ({Rb})", fb)

print("f:", 1 / (1 / ff + 1 / fb))

print("f (thick lens with d = 0)", lens_makers(Rf, Rb, 1.5))
d = Q_(4, "mm")
print(f"f (thick lens with d = {d})", lens_makers_thick(Rf, Rb, d, 1.5))
d = Q_(8, "mm")
print(f"f (thick lens with d = {d})", lens_makers_thick(Rf, Rb, d, 1.5))


print(spherical_refractor_focal_length(Q_(-40, "mm"), 1.5, 1))
print(new_waist_pos(Q_(-120, "cm"), Q_(80, "mm"),zRp)))
