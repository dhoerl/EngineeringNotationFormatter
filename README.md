EngineeringNotationFormatter
============================

iOS Project demoing C-based Engineering Notation Formatter

Several years ago I tripped on a great formatter to turn an arbitrary floating point number into a proper engineering notation format written by Jukka Korpela, at http://www.cs.tut.fi/~jkorpela/c/eng.html.

This is useful when you want to display say, length, in units people (not machines) can understand. Thus, instead of 1.67e-4 meters you could show either 16.7e-3 or using International System of Units (SI) prefixes (http://physics.nist.gov/cuu/Units/prefixes.html) as 17.7 m.

As Jukka points out, there is no POSIX formatter that will do this, so he wrote one. In using his code, I found an edge condition and provided updated code, which can be found on his site mentioned earlier.

For sure, this feature is not one many people need, but it you do need it, this code is invaluable, and I can only imagine that Jukka spent many days working on it.

We jointly offer this code here with a unattributed BSD style license.
