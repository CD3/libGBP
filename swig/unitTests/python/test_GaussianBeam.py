import py_libGBP
from py_libGBP import Q_

def close(a,b,tol=0.01):
  return abs(a-b) < tol*(a+b)/2


def test_configuration():

  beam = py_libGBP.GaussianBeam()

  beam.setFrequency( Q_(100,'GHz') )
  assert close( beam.getFrequencyDP(), 100e9)
  assert close( beam.getFrequency(), Q_(100,'GHz') )
  assert close( beam.getFreeSpaceWavelength(), Q_(2.997925,'mm') )
