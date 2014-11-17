/*! \mainpage

<strong>
(C) Copyright 2010-2013 E. Brady Trexler and Kladiusz R. Weiss</strong> <br><br>
( http://www.gothamsci.com , ebtrexler (at) gothamsci.com )

( http://www.mountsinai.org/profiles/klaudiusz-r-weiss )

<br><br>
Coding practices:

This is a C++ application.

 Classnames are prefixed with T, as in TClassName,
 to denote a new Type. Most class data members have private access and are
 "got" and "set" by the __property (closure) keyword, or public get and set methods.
 Private data members are prefixed with F for Field (eg. FName).


Sotware Divisions:
   -# Class framework for cellular connections that handles wiring up network
   and saving/loading from disk.  Future expansion of model with derived classes
   does not need to be concerned with plumbing, only the math and calculations.
   -# Class framework(s) for data acquisition.  These classes will provide
   public interfaces that are the same regardless of the hardware and drivers
   used.
   -# Class framework for user interface.  Currently implemented in Borland
   (Embarcadero) C++ Builder (RAD Studio 2010)

<br><br>
\section RTFramework *******Overview of Real Time Network Class Framework*******

 Implements a set of serializable base classes that describe a cellular network.
  Each class has a member function called Update that takes a time step (in ms)
  (and if necessary the membrane voltages (in mV) of the cell or the pre and post
   synaptic cells), and returns the current to inject (in nA).


 Design considerations:<br>
     - The class framework is designed to be independent of data acquisition
  hardware.
     - The class framework is designed to be thread safe.
     - Derived classes can implement calculations as desired.
     - The class framework is designed to be used with a GUI, but does not
  specify how it should be implemented.
     - Object creation and destruction (memory management) occurs by use of
     boost::shared_ptr.
     - Applications that use this framework should keep with the convention of
         - milliVolts (mV)
         - nanoAmperes (nA)
         - microSiemens (uS)
         - nanoFarads (nF) and
         - milliseconds (ms)
         .
         as units for computation.




<strong> Class organization </strong>
<pre>
*      TNetwork            // owns all others and the master Update function
*      |      |            // handles object creation and deletion
*    TCell    |
*      |      |
* TSynapse - TCell        // TCell owns TSynapse and vice versa
*      |      |  |
*   TCurrent  | TCurrent   // derive from TCurrent to implement new flavors
*             |            // synapses and cells have TCurrent arrays
*             |
*          TElectrode      // Allows injection of arbitrary current
</pre>

\subsection Derived ####Derived classes implementing network functionality####

Currents:<br>
THHKineticsFactor <br>
THHCurrent <br>
THH2Current <br>
TGapJunctionCurrent <br>
Calculations for above currents are based on dynamicclamp software from Farzan Nadim's laboratory<br>
( http://stg.rutgers.edu/farzan/ , farzan (at) njit.edu <br>
-- See Tohidi and Nadim 2009 J Neurosci. and Willms et al 1999 J Comp Neurosci.
<br><br>
TVoltageClamp_PID_Current<br>
<br>
<br>
Synapses:<br>
TGenBiDirSynapse <br>
TGapJunctionSynapse <br>
<br>
Cells:<br>
TBiologicalCell <br>
TModelCell <br>
TPlaybackCell<br>
<br>
Electrodes: <br>
TInjectionElectrode <br>
TPlaybackElectrode <br>

*/

