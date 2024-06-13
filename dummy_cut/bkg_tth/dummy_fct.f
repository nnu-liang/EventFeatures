      logical FUNCTION dummy_cuts(P)
      implicit none
      include 'nexternal.inc'
      REAL*8 P(0:3,nexternal)
      real, dimension(0:3) :: b, u, antid, b1, b2
      real, dimension(0:3) :: antib, antiu, d
      real :: px1, py1, Pt1, px2, py2, Pt2, px3, py3, Pt3
      dummy_cuts = .true.
      b = P(:, 3) 
      u = P(:, 4)
      antid = P(:, 5)
      antib = P(:, 6)
      d = P(:, 7)
      antiu = P(:, 8)
      b1 = P(:, 9)
      b2 = P(:, 10)
      
      px1 = b(1) + u(1) + antid(1)
      py1 = b(2) + u(2) + antid(2)
      px2 = antib(1) + d(1) + antiu(1)
      py2 = antib(2) + d(2) + antiu(2)
      px3 = b1(1) + b2(1)
      py3 = b1(2) + b2(2)

      Pt1 = sqrt( px1**2 + py1**2 )
      Pt2 = sqrt( px2**2 + py2**2 )
      Pt3 = sqrt( px3**2 + py3**2 )
      if (Pt1 <= 400.0 .or. Pt1 >= 1200.0) dummy_cuts = .false.
      if (Pt2 <= 400.0 .or. Pt2 >= 1200.0) dummy_cuts = .false.
      if (Pt3 <= 400.0 .or. Pt3 >= 1200.0) dummy_cuts = .false.
      
      return
      end


      subroutine get_dummy_x1(sjac, X1, R, pbeam1, pbeam2, stot, shat)
      implicit none
      include 'maxparticles.inc'
      include 'run.inc'
c      include 'genps.inc'
      double precision sjac ! jacobian. should be updated not reinit
      double precision X1   ! bjorken X. output
      double precision R    ! random value after grid transfrormation. between 0 and 1
      double precision pbeam1(0:3) ! momentum of the first beam (input and/or output)
      double precision pbeam2(0:3) ! momentum of the second beam (input and/or output)
      double precision stot        ! total energy  (input and /or output)
      double precision shat        ! output

c     global variable to set (or not)
      double precision cm_rap
      logical set_cm_rap
      common/to_cm_rap/set_cm_rap,cm_rap
      
      set_cm_rap=.false. ! then cm_rap will be set as .5d0*dlog(xbk(1)*ebeam(1)/(xbk(2)*ebeam(2)))
                         ! ebeam(1) and ebeam(2) are defined here thanks to 'run.inc'
      shat = x1*ebeam(1)*ebeam(2)
      return 
      end

      subroutine get_dummy_x1_x2(sjac, X, R, pbeam1, pbeam2, stot,shat)
      implicit none
      include 'maxparticles.inc'
      include 'run.inc'
c      include 'genps.inc'
      double precision sjac ! jacobian. should be updated not reinit
      double precision X(2)   ! bjorken X. output
      double precision R(2)    ! random value after grid transfrormation. between 0 and 1
      double precision pbeam1(0:3) ! momentum of the first beam
      double precision pbeam2(0:3) ! momentum of the second beam
      double precision stot        ! total energy
      double precision shat        ! output

c     global variable to set (or not)
      double precision cm_rap
      logical set_cm_rap
      common/to_cm_rap/set_cm_rap,cm_rap
      
      set_cm_rap=.false. ! then cm_rap will be set as .5d0*dlog(xbk(1)*ebeam(1)/(xbk(2)*ebeam(2)))
                         ! ebeam(1) and ebeam(2) are defined here thanks to 'run.inc'
      shat = x(1)*x(2)*ebeam(1)*ebeam(2)
      return 
      end


      logical  function dummy_boostframe()
      implicit none
c
c      
      dummy_boostframe = .false.
      return
      end
      

      double precision function user_dynamical_scale(P)
c     allow to define your own dynamical scale, need to set dynamical_scale_choice to 0 (or 10) to use it
      implicit none
      include 'nexternal.inc'
      double precision P(0:3, nexternal)
c     Commmon to have access to all variable defined in the run_card      
      include 'genps.inc'
      include 'run.inc'
      write(0,*) "dynamical scale set to 0"
      write(0,*) "need to be defined via user_hook method"
      stop 1
c     fixed scale
      return
      end

      
      
C ************************************************************
C default for the library implementing a dummy bias function
C ************************************************************
      subroutine bias_wgt_custom(p, original_weight, bias_weight)
      implicit none
C
C Parameters
C
          include 'nexternal.inc'

C     
C Arguments
C
          double precision p(0:3, nexternal)
          double precision original_weight, bias_weight
C
C local variables
C
C
C Global variables
C
C      common block with metadata for the bias
C
          double precision stored_bias_weight
c          data stored_bias_weight/1.0d0/
          logical impact_xsec, requires_full_event_info
C         Impact_xsec 
C         Not impacting the xsec since the bias is 1.0. Therefore
C         bias_wgt will not be written in the lhe event file.
C         Setting it to .True. makes sure that it will not be written.
C         Default: True
C         Requires_full_event_info          
C         Of course this module does not require the full event
C         information (color, resonances, helicities, etc..)
c         Default: False          
          common/bias/stored_bias_weight,impact_xsec,
     &                requires_full_event_info

C --------------------
C BEGIN IMPLEMENTATION
C --------------------
          bias_weight = 1.0d0

      return
      end subroutine bias_wgt_custom

