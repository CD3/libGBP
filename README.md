# libGPB

A C++ library for doing Gaussian Beam Propagation calculations.

## Description

This library implements Gaussian beam propagation calculations based on the paraxial wave equation
(https://en.wikipedia.org/wiki/Gaussian_beam).
It started out as a fork of the code written by Aaron Hoffman for his senior thesis at
Fort Hays State University, but has since been redesigned (rewritten) based on what we learned during
his project. It supports the transformation of a beam through multiple optical elements and power transmission
through linear absorbers.

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
calculate and return the beam divergence (an implementation of the physics),
but does not allow the divergence to be set. Instead, the wavelength and beam
waist radius must be set correctly to give a desired beam divergence. The reason for
this is to keep the physics implementation as small and simple as possible.

Support for flexible configuration is achieved through builder classes. So, for example,
the builder class for beams allows the user to specify the wavelength and divergence of
a beam and have the beam waist radius automatically calculated. These builder classes can
implement whatever configuration interface is convenient (direct method calls, boost property trees, configuration
files, etc). and isolate the physics implementation classes from bloat and complexity.

The calculator classes are used to perform the basic calculations and combine the physics and builder classes.

## Todo

    - Implement a simulation class to calculate and return beam characterization at multiple locations in a setup.
    - Add electric field method to GaussianBeam class.
    - Add support for power loss from scattering.
    - Add support for specifying parameters as functions to the builder classes.
