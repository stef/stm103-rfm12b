#!/usr/bin/env python

band={433: {'c1': 1, 'c2': 43},
      868: {'c1': 2, 'c2': 43},
      915: {'c1': 3, 'c2': 30}}

# 96 <= f <= 3903
# f0 = 10 * C1 * (C2 + F/4000) [MHz]

for f in xrange(96,3904):
    print f - 96, 10 * band[433]['c1'] * (band[433]['c2'] + f/4000.0), 'MHz'
