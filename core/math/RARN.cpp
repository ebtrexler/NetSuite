/* /////////////////////////////////////////////////////////////////////////////
NetSuite: NetClamp/NetSim/NetFit Neural Network Modeling and Dynamic Clamp Software
	 Copyright (C) 2011-2014  E. Brady Trexler, Ph.D.

	 This program is free software: you can redistribute it and/or modify
	 it under the terms of the GNU General Public License as published by
	 the Free Software Foundation, either version 3 of the License, or
	 (at your option) any later version.

	 This program is distributed in the hope that it will be useful,
	 but WITHOUT ANY WARRANTY; without even the implied warranty of
	 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	 GNU General Public License for more details.

	 You should have received a copy of the GNU General Public License
	 along with this program.  If not, see <http://www.gnu.org/licenses/>.

Please direct correspondence to ebtrexler _at_ hudsonvalleysci _dot_ com
///////////////////////////////////////////////////////////////////////////// */

//---------------------------------------------------------------------------


#pragma hdrstop

#include <math.h>
#include "RARN.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

TRationalFraction RARN(double x, long maxden)
{
   long m[2][2];
   double startx;
   long ai;

   double testerror1, testerror2;
   TRationalFraction test1, test2;
   startx = x;

   /* initialize matrix */
   m[0][0] = m[1][1] = 1;
   m[0][1] = m[1][0] = 0;

   /* loop finding terms until denom gets too big */
   while (m[1][0] *  ( ai = (long)x ) + m[1][1] <= maxden) {
      long t;
      t = m[0][0] * ai + m[0][1];
      m[0][1] = m[0][0];
      m[0][0] = t;
      t = m[1][0] * ai + m[1][1];
      m[1][1] = m[1][0];
      m[1][0] = t;
      if(x==(double)ai) break;     // AF: division by zero
      x = 1/(x - (double) ai);
      if(x>(double)0x7FFFFFFF) break;  // AF: representation failure
   }

   /* now remaining x is between 0 and 1/ai */
   /* approx as either 0 or 1/m where m is max that will fit in maxden */
   /* first try zero */
   test1.Numerator =   m[0][0];
   test1.Denominator = m[1][0];
   testerror1 = startx - ((double) m[0][0] / (double) m[1][0]);

   /* now try other possibility */
   ai = (maxden - m[1][1]) / m[1][0];
   m[0][0] = m[0][0] * ai + m[0][1];
   m[1][0] = m[1][0] * ai + m[1][1];
   test2.Numerator =   m[0][0];
   test2.Denominator = m[1][0];
   testerror2 = startx - ((double) m[0][0] / (double) m[1][0]);

   return (fabs(testerror1) < fabs(testerror2)) ? test1 : test2;
}
