# libGPB

A C++ library for doing Laser Beam Propagation calculations.

## Description

This library implements Gaussian beam propagation calculations based on the paraxial wave equation
(https://en.wikipedia.org/wiki/Gaussian_beam) and the [general range equations described by Seigman](
https://web.archive.org/web/20110604095354/http://www.stanford.edu/~siegman/beams_and_resonators/beam_quality_tutorial_osa.pdf).
It started out as a fork of the code written by Aaron Hoffman for his senior thesis at
Fort Hays State University, but has since been redesigned (rewritten) based on what we learned during
his project.

### Features

- Supports multiple conventions.
    - Multiple conventions for defining the "beam width" and "beam divergence" exist. Physics text books almost always use the 1/e squared beam radius and 1/e squared half width divergence, but
      the laser safety community uses 1/e beam diameter and 1/e full width divergence. The library supports both conventions (and others) and makes them explicit (methods are not named `setWaistDiameter(...)`, they
      are named `setOneOverEWaistDiameter(...)`, etc.)
- Compute the [four sigma beam diameter](https://en.wikipedia.org/wiki/Beam_diameter#D4%CF%83_or_second-moment_width) for an arbitrary laser beam profile.
    - The second-moment-based beam width obeys a universal propagation law that holds for arbitrary beam shapes. The `LaserBeam` class implements the range equations
      and supports beams with an arbitrary "M-squared" factor.
- Compute beam properties for Gaussian beams
    - For Gaussian laser beams, several additional properties can be defined and measured. The `GaussianLaserBeam` class provides methods to compute:
        - one over e squared beam radius and diameter at an arbitrary range.
        - one over e beam radius and diameter at an arbitrary range.
        - full width half max beam radius and diameter at an arbitrary range.
        - four sigma diameter at an arbitrary range.
        - diffraction limited one over e squared half width and full width divergence.
        - one over e half width and full width divergence.
        - full width half max half hand full width divergence.
        - Rayleigh range.
        - Beam radius of curvature at an arbitrary range.
- Propagation through various optical elements, including thin lenses and spherical refractive surfaces, using the ABCD ray matrix method.


## Getting Started

The core functionality of `libGBP` is implemented in the `GaussianLaserBeam` class. For simple calculations, this class
is all that is needed. For example, to compute the beam diameter of a laser pointer with a known diameter and divergence at some
range:

```cpp
#include<libGBP/GaussianBeam.hpp>

...

GaussianLaserBeam beam;

beam.setOneOverEWaistDiameter( 5*i::mm );
beam.setOneOverEFullAngleDivergence( 1.5*i::mrad );

// using Catch2
CHECK( beam.getOneOverEDiameter<t::cm>( 10*i::m ).value() == Approx(1.58).epsilon(0.01) );

```

The method calls may look a little verbose, but confusion between 1/e vs. 1/e squared, radius vs. diameter,
and half angle vs. full angle divergence is a common source or error. Here it is clear that we are
setting the 1/e beam waist diameter, the 1/e full angle divergence, and asking for the 1/e beam diameter
at range.

Note also the units. Parameters can be set in whatever units are convenient, and we can ask for calculations
in whatever units we want (as long as they are defined). libGBP uses [Boost.Units](https://www.boost.org/doc/libs/1_73_0/doc/html/boost_units.html) and all setter/getter methods of the
`GaussianLaserBeam` class support passing and returning arbitrary units.

`GaussianLaserBeam` objects can also be transformed by optical components. The following
example computes the effect of a 15-cm lens placed at the waist position of a 5 mm laser beam.
```cpp
    GaussianLaserBeam beam;
    ThinLens<t::centimeter> lens;

    beam.setWavelength(532 * i::nm);
    beam.setOneOverE2WaistDiameter(5 * i::mm);
    lens.setFocalLength( 15*i::cm );

    beam.transform(&lens);


    CHECK( beam.getWaistPosition<t::centimeter>().value() == Approx(15) );
    CHECK( beam.getOneOverE2WaistRadius<t::micrometer>().value() == Approx(1e6 * 532e-9*15e-2/M_PI/(5e-3/2)) ); // lambda f / pi omega

```

## Installing

`libGBP` can be built an installed with `cmake`. Simply clone the git repository and run:

```bash
$ mkdir build
$ cd build
$ cmake ..
$ cmake --build .
$ cmake --build . --target install
```

The install directory can be set using the `CMAKE_INSTALL_PREFIX` variable

```bash
$ cmake -DCMAKE_INSTALL_PREFIX=/path/to/custom/install/directory ..
```

## Design

The library classes are grouped into three main categories: physics
implementation, object builders, and calculators. The physics implementation
classes are the classes that implement the physics and they are kept simple.
They provide a set of methods to configure the object and a set of methods to
implement the physics. They do not provide any extra methods for flexible
configuration. For example, the class that implements a Gaussian beam can
calculate and return the Rayleigh range (an implementation of the physics),
but does not allow the Rayleigh to be set. Instead, the wavelength and beam
waist radius must be set correctly to give a desired beam Rayleigh range. The reason for
this is to keep the physics implementation as small and simple as possible.

Support for flexible configuration is achieved through builder classes. So, for example,
the builder class for beams allows the user to specify the wavelength and divergence of
a beam and have the beam waist radius automatically calculated. These builder classes can
implement whatever configuration interface is convenient (direct method calls, boost property trees, configuration
files, etc). and isolate the physics implementation classes from bloat and complexity.

The calculator classes are used to perform the basic calculations and combine the physics and builder classes.

## Todo

    - Implement a simulation class to calculate and return beam characterization at multiple locations in a setup.
    - Add electric field method to GaussianLaserBeam class.
    - Add support for power loss from scattering.
    - Add support for specifying parameters as functions to the builder classes.
