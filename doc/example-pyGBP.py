import libGBP
import yaml
import json
import math

'''
Computes the beam diameter of a Gaussian laser beam as it propagates through a 7x50 telescope.
'''

config = '''
evaluation_points:
  z:
    min: 0
    max: 10
    n: 2000
beam:
  wavelength: 532
  power : 1
  divergence : 10
  waist:
    position: 0
optical_system:
  elements:
    - position: 0
      type: Thin Lens
      focal_length : 7.0
    - position: 8.0
      type: Thin Lens
      focal_length : 1.0
    - position: 9.0
      type: Spherical Interface
      refractive_index:
        initial: 1
        final: 1.333
      radius_of_curvature: 0.61

'''

config = yaml.safe_load(config)

calc = libGBP.GBPCalc()
for pos in [9, 0,-1,-10,-30,-100,-300]:
  calc.clear()
  config['beam']['waist']['position'] = pos
  calc.configure( json.dumps(config) )
  beam = calc.calculateBeam()
  for b in beam:
    print(b.getCurrentPosition(),b.getOneOverEDiameterAtCurrentPosition())
  print()
