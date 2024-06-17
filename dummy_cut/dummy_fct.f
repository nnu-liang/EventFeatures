      logical FUNCTION dummy_cuts(P)
      implicit none
      include 'nexternal.inc'
      REAL*8 P(0:3,nexternal)
      real*8 :: E1, px1, py1, pz1, E2, px2, py2, pz2, sqrt_s

      dummy_cuts = .true.

      E1 = P(0, 1)
      px1 = P(1, 1)
      py1 = P(2, 1)
      pz1 = P(3, 1)
      
      E2 = P(0, 2)
      px2 = P(1, 2)
      py2 = P(2, 2)
      pz2 = P(3, 2)

      sqrt_s = sqrt((E1 + E2)**2 - ((px1 + px2)**2 + (py1 + py2)**2 + (pz1 + pz2)**2))

      if (sqrt_s < 0 .or. sqrt_s > 500) dummy_cuts = .false.
      
      return
      end

