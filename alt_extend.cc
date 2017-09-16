/**
    CNEFinder
    Copyright (C) 2017 Lorraine A. K. Ayad, Solon P. Pissis, Dimitris Polychronopoulos

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
**/

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <math.h> 
#include <vector>
#include <unordered_map>
#include <string.h>
#include <sys/time.h>
#include <omp.h>
#include "cnef.h"
#include "edlib.h"

using namespace std;



int alt_extend( unsigned int * edit_distance, int * q_start,  int * q_end, int * r_start, int * r_end, unsigned char * xInput, unsigned char * yInput, TSwitch sw, int alt_ext )
{
	int toAddStartQuery = 1;
	int toAddEndQuery = 1;
	int toAddStartRef = 1;
	int toAddEndRef = 1;

	int q_start_temp = *q_start;
	int q_end_temp = *q_end;
	int r_start_temp = *r_start;
	int r_end_temp = *r_end;

	int qS = *q_start;
	int rS = *r_start;	
	int qE = *q_end;
	int rE = *r_end;

	char rsc;
	char qsc;
	char rec;
	char qec;

	int rs = *r_start;
	int qs = *q_start;
	int re = *r_end;
	int qe = *q_end;

	int edit_distance_total_L = 0;
	int edit_distance_total_R = 0;

	int edit_distance_temp = *edit_distance;
	int edit_distance_updated = *edit_distance;

	char operationEnd;
	char operationStart;

	double minLen = min( q_end_temp - q_start_temp, r_end_temp - r_start_temp );
	while( q_start_temp >= 0 || r_start_temp >= 0 || q_end_temp <=  strlen( ( char* ) yInput ) -1 || r_end_temp <=  strlen( ( char* ) xInput ) )
	{
		/************************************************ Score for extending right ***************************************************/
		int edit_distance_R;

		char sRref;
		char sRquery;
		char iRref;
		char iRquery;
		char dRref;
		char dRquery;

		int maxLen = max(  strlen( ( char* ) yInput ),strlen( ( char* ) xInput ) );

		if (  q_end_temp  < strlen( ( char* ) yInput )  &&  r_end_temp  < strlen( ( char* ) xInput ) ) 
		{	
			int editDist_S;
			unsigned char * m_ref_R  = ( unsigned char * ) calloc (  toAddEndRef + 1, sizeof ( unsigned char ) );
			unsigned char * m_query_R  = ( unsigned char * ) calloc ( toAddEndQuery + 1, sizeof ( unsigned char ) );

			if( xInput[rE + toAddEndRef -1 ] == '$' || yInput[ qE + toAddEndQuery -1 ] == '$' )
			{
				editDist_S = maxLen + 1;
			}
			else
			{
				memcpy( &m_ref_R[0], &xInput[rE],  toAddEndRef  );
				memcpy( &m_query_R[0], &yInput[qE],  toAddEndQuery  );
				m_ref_R[ toAddEndRef ] = '\0';
				m_query_R[ toAddEndQuery ] = '\0';

				editDist_S = editDistanceMyers( m_ref_R, m_query_R );

				sRref = m_ref_R[toAddEndRef - 1];
				sRquery = m_query_R[toAddEndQuery -1];
			}
				
			int editDist_I;
			int editDist_D;
			
	
			if( toAddEndRef > 1 )
			{
				if( yInput[ qE + toAddEndQuery - 1] == '$' )
					editDist_I = maxLen + 1;
				else
				{
					memcpy( &m_ref_R[0], &xInput[rE],  toAddEndRef  );
					memcpy( &m_query_R[0], &yInput[qE],  toAddEndQuery  );
					m_ref_R[ toAddEndRef - 1 ] = '\0';
					m_query_R[ toAddEndQuery ] = '\0';

					editDist_I = editDistanceMyers( m_ref_R, m_query_R );

					iRref = m_ref_R[toAddEndRef - 2];
					iRquery = m_query_R[toAddEndQuery -1];
				}
	
				if( xInput[ rE + toAddEndRef - 1] == '$' )
					editDist_D = maxLen + 1;
				else
				{
					memcpy( &m_ref_R[0], &xInput[rE],  toAddEndRef  );
					memcpy( &m_query_R[0], &yInput[qE],  toAddEndQuery  );
					m_ref_R[ toAddEndRef ] = '\0';
					m_query_R[ toAddEndQuery - 1 ] = '\0';

					editDist_D = editDistanceMyers( m_ref_R, m_query_R );

					dRref = m_ref_R[toAddEndRef - 1];
					dRquery = m_query_R[toAddEndQuery -2];
				}

			}
			else
			{
				editDist_I = maxLen + 1;
				editDist_D = maxLen + 1;
			}

			edit_distance_R =  min( editDist_S, min( editDist_I, editDist_D ) );

			if( edit_distance_R == editDist_S && sRref == sRquery )
			{
				operationEnd = 'S';
				rec = sRref;
				qec = sRquery;

			}
			else if( edit_distance_R == editDist_I && iRref == iRquery )
			{
				operationEnd = 'I';
				rec = iRref;
				qec = iRquery;

			}
			else if( edit_distance_R == editDist_D && dRref == dRquery )
			{
				operationEnd = 'D';
				rec = dRref;
				qec = dRquery;
			}
			else if( edit_distance_R == editDist_S )
			{
				operationEnd = 'S';
				rec = sRref;
				qec = sRquery;

			}
			else if( edit_distance_R == editDist_I  )
			{
				operationEnd = 'I';
				rec = iRref;
				qec = iRquery;
			}
			else if( edit_distance_R == editDist_D  )
			{
				operationEnd = 'D';
				rec = dRref;
				qec = dRquery;
			}

			free( m_ref_R );
			free( m_query_R );

		}
		else if( qE == strlen( ( char* ) yInput ) && rE != strlen( ( char* ) xInput ) && r_end_temp < strlen( ( char* ) xInput )  )
		{
			if( xInput[ r_end_temp + 1] == '$' )
				edit_distance_R = maxLen + 1;
			else
			{
				edit_distance_R = edit_distance_total_R + 1;
				operationEnd = 'D';

				rec = xInput[ r_end_temp  ];
				qec = yInput[ strlen( ( char* ) yInput ) - 1];
			}
		}
		else if( rE == strlen( ( char* ) xInput ) && qE != strlen( ( char* ) yInput ) && q_end_temp < strlen( ( char* ) yInput ) )
		{
			
			if( yInput[ q_end_temp + 1] == '$' )
				edit_distance_R = maxLen + 1;
			else
			{
				edit_distance_R = edit_distance_total_R + 1;
				operationEnd = 'I';

				qec = yInput[ q_end_temp  ];
				rec = xInput[ strlen( ( char* ) xInput ) - 1];
			}
		}
		else if ( q_end_temp  < strlen( ( char* ) yInput ) && r_end_temp >= strlen( ( char* ) xInput ) )	
		{
		
			if( yInput[ qE + toAddEndQuery  -1 ] == '$' )
				edit_distance_R = maxLen + 1;
			else
			{
				unsigned char * m_ref_R = ( unsigned char * ) calloc (  toAddEndRef + 1, sizeof ( unsigned char ) );
				unsigned char * m_query_R = ( unsigned char * ) calloc ( toAddEndQuery + 1, sizeof ( unsigned char ) );

				memcpy( &m_ref_R[0], &xInput[rE],  strlen( ( char* ) xInput ) - rE );
				memcpy( &m_query_R[0], &yInput[qE], toAddEndQuery  );
				m_ref_R[ toAddEndRef ] = '\0';
				m_query_R[ toAddEndQuery ] = '\0';
		
				edit_distance_R =  editDistanceMyers( m_ref_R, m_query_R );
				operationEnd = 'I';

				rec = m_ref_R[ strlen( ( char* ) xInput )  - 1];
				qec = m_query_R[ toAddEndQuery - 1];

				free( m_ref_R );
				free( m_query_R );
			}
		}
		else if ( q_end_temp  >= strlen( ( char* ) yInput ) - 1 && r_end_temp < strlen( ( char* ) xInput ) - 1 )	
		{
			if( xInput[ rE+toAddEndRef  -1 ] == '$' )
				edit_distance_R = maxLen + 1;
			else
			{
				unsigned char * m_ref_R = ( unsigned char * ) calloc (  toAddEndRef + 1, sizeof ( unsigned char ) );
				unsigned char * m_query_R = ( unsigned char * ) calloc ( toAddEndQuery + 1, sizeof ( unsigned char ) );

				memcpy( &m_ref_R[0], &xInput[rE],  toAddEndRef );
				memcpy( &m_query_R[0], &yInput[qE], strlen( ( char* ) yInput ) - qE   );
				m_ref_R[ toAddEndRef ] = '\0';
				m_query_R[ toAddEndQuery ] = '\0';
		
				edit_distance_R =  editDistanceMyers( m_ref_R, m_query_R );
				operationEnd = 'D';

				rec = m_ref_R[ toAddEndRef - 1 ];
				qec = m_query_R[  strlen( ( char* ) yInput )  - 1];

				free( m_ref_R );
				free( m_query_R );
			}
		}
		else 
		{	
			edit_distance_R = maxLen + 1;
			rec = xInput[ strlen( ( char * ) xInput ) - 1 ];
			qec = yInput[ strlen( ( char * ) yInput ) - 1 ];

		}


		/*********************************************** score for extending left *************************************************/
		int edit_distance_L;

		char sLref;
		char sLquery;
		char iLref;
		char iLquery;
		char dLref;
		char dLquery;

		if(  q_start_temp  > 0 &&  r_start_temp > 0   )  
		{
			int editDist_S;
			unsigned char * m_ref_L = ( unsigned char * ) calloc ( toAddStartRef + 1, sizeof ( unsigned char ) );
			unsigned char * m_query_L = ( unsigned char * ) calloc ( toAddStartQuery + 1, sizeof ( unsigned char ) );
			

			if( xInput [rS - toAddStartRef] == '$' || yInput [qS - toAddStartQuery] == '$' )
				editDist_S  = maxLen +1;
			else
			{

				memcpy( &m_ref_L[0], &xInput [rS - toAddStartRef], toAddStartRef );
				memcpy( &m_query_L[0], &yInput [qS - toAddStartQuery], toAddStartQuery );
				m_ref_L[ toAddStartRef ] = '\0'; 
				m_query_L[ toAddStartQuery ] = '\0';

				editDist_S = editDistanceMyers( m_ref_L, m_query_L );

				sLref = m_ref_L[0];
				sLquery = m_query_L[0];
			}

			int editDist_I;
			int editDist_D;

			if( toAddStartRef > 1 )
			{
				if(  yInput [qS - toAddStartQuery] == '$' )
				{
					editDist_I  = maxLen +1;
				}
				else
				{
					memcpy( &m_ref_L[0], &xInput [ rS - toAddStartRef + 1  ], toAddStartRef  );
					memcpy( &m_query_L[0], &yInput [qS - toAddStartQuery], toAddStartQuery );
					m_ref_L[ toAddStartRef - 1 ] = '\0';
					m_query_L[ toAddStartQuery ] = '\0';
					
					editDist_I = editDistanceMyers( m_ref_L, m_query_L );
				
					iLref = m_ref_L[0];
					iLquery = m_query_L[0];
				}


				if( xInput [rS - toAddStartRef] == '$' )
					editDist_D  = maxLen +1;
				else
				{
					memcpy( &m_ref_L[0], &xInput [rS - toAddStartRef], toAddStartRef );
					memcpy( &m_query_L[0], &yInput [ qS - toAddStartQuery + 1 ], toAddStartQuery   );
					m_ref_L[ toAddStartRef ] = '\0';
					m_query_L[ toAddStartQuery - 1 ] = '\0';

					editDist_D = editDistanceMyers( m_ref_L, m_query_L );
	
					dLref = m_ref_L[0];
					dLquery = m_query_L[0];
				}
			}
			else
			{
				editDist_I = maxLen + 1;
				editDist_D = maxLen + 1;
			}

			edit_distance_L =  min( editDist_S, min( editDist_I, editDist_D ) );

			if( edit_distance_L == editDist_S && sLref == sLquery  )
			{
				operationStart = 'S';
				rsc = sLref;
				qsc = sLquery;
			}
			else if( edit_distance_L == editDist_I && iLref == iLquery   )
			{
				operationStart = 'I';
				rsc = iLref;
				qsc = iLquery;
			}
			else if( edit_distance_L == editDist_D && dLref == dLquery  )
			{
				operationStart = 'D';
				rsc = dLref;
				qsc = dLquery;
			}
			else if( edit_distance_L == editDist_S  )
			{
				operationStart = 'S';
				rsc = sLref;
				qsc = sLquery;
			}
			else if( edit_distance_L == editDist_I  )
			{
				operationStart = 'I';
				rsc = iLref;
				qsc = iLquery;
			}
			else if( edit_distance_L == editDist_D  )
			{
				operationStart = 'D';
				rsc = dLref;
				qsc = dLquery;
			}

			free( m_ref_L );
			free( m_query_L );

		}
		else if( qS == 0 && rS != 0 && r_start_temp > 0 )
		{
			if( xInput [r_start_temp - 1] == '$'  )
				edit_distance_L  = maxLen +1;
			else
			{
				edit_distance_L = edit_distance_total_L + 1;
				operationStart = 'D';

				rsc = xInput[ r_start_temp - 1 ];
				qsc = yInput[0];
			}
		}
		else if( rS == 0 && qS != 0 && q_start_temp > 0 )
		{	
			if( yInput [q_start_temp - 1] == '$'  )
				edit_distance_L  = maxLen +1;
			else
			{
				edit_distance_L = edit_distance_total_L + 1;
				operationStart = 'I';

				qsc = yInput[ q_start_temp - 1];
				rsc = xInput[0];
			}
		}
		else if ( q_start_temp  <= 0 && r_start_temp > 0 )	
		{
			if( xInput [rS - toAddStartRef] == '$'  )
				edit_distance_L  = maxLen +1;
			else
			{
				unsigned char * m_ref_L = ( unsigned char * ) calloc ( toAddStartRef + 1, sizeof ( unsigned char ) );
				unsigned char * m_query_L = ( unsigned char * ) calloc ( toAddStartQuery + 1, sizeof ( unsigned char ) );
				
				memcpy( &m_ref_L[0], &xInput [rS - toAddStartRef], toAddStartRef );
				memcpy( &m_query_L[0], &yInput [0], qS );
				m_ref_L[ toAddStartRef ] = '\0';
				m_query_L[ toAddStartQuery ] = '\0';
			
				edit_distance_L = editDistanceMyers( m_ref_L, m_query_L );
				operationStart = 'D';

				rsc = m_ref_L[0];
				qsc = m_query_L[0];

				free( m_ref_L );
				free( m_query_L );
			}

		}
		else if ( q_start_temp  > 0 && r_start_temp <= 0 )	
		{	
			if( yInput [qS - toAddStartQuery] == '$' )
				edit_distance_L = maxLen + 1;
			else
			{
				unsigned char * m_ref_L = ( unsigned char * ) calloc ( toAddStartRef + 1, sizeof ( unsigned char ) );
				unsigned char * m_query_L = ( unsigned char * ) calloc ( toAddStartQuery + 1, sizeof ( unsigned char ) );
				
				memcpy( &m_ref_L[0], &xInput [0], rS );
				memcpy( &m_query_L[0], &yInput [qS - toAddStartQuery], toAddStartQuery );
				m_ref_L[ toAddStartRef ] = '\0';
				m_query_L[ toAddStartQuery ] = '\0';

				edit_distance_L = editDistanceMyers( m_ref_L, m_query_L );
				operationStart = 'I';

				rsc = m_ref_L[0];
				qsc = m_query_L[0];

				free( m_ref_L );
				free( m_query_L );

			}
		}
		else 
		{	
			rsc = xInput[0];
			qsc = yInput[0];
			edit_distance_L = maxLen + 1;
		}

		/*********************************************** computing extension *************************************************/

		if( alt_ext == 2 )
		{
			if(  edit_distance_temp + edit_distance_total_R + edit_distance_L <= edit_distance_temp + edit_distance_R + edit_distance_total_L  && (edit_distance_temp + edit_distance_total_R + edit_distance_L ) / ( minLen+1 ) <= sw.t + sw.s ) //extend left
			{
				if( operationStart == 'S' )
				{
					q_start_temp--;
					r_start_temp--;
					toAddStartQuery++;
					toAddStartRef++;
					
				}
				else if( operationStart == 'I' )
				{
					toAddStartQuery++;
					q_start_temp--;
				}

				else if( operationStart == 'D' )
				{
					toAddStartRef++;
					r_start_temp--;
				}

			
				edit_distance_total_L = edit_distance_L;
				edit_distance_updated = edit_distance_temp + edit_distance_total_R + edit_distance_L;

				minLen = min( q_end_temp - q_start_temp, r_end_temp - r_start_temp );
				if( rsc == qsc && ( edit_distance_updated / minLen ) <= sw.t  )
				{
			
					rs = r_start_temp;
					qs = q_start_temp;
				}
				
			}
			else if (  edit_distance_temp + edit_distance_R + edit_distance_total_L <= edit_distance_temp + edit_distance_total_R + edit_distance_L && (edit_distance_temp + edit_distance_R + edit_distance_total_L ) / ( minLen+1 ) <= sw .t + sw.s) //extend right
			{
				if( operationEnd == 'S' )
				{
					q_end_temp++;
					r_end_temp++;
					toAddEndQuery++;
					toAddEndRef++;
				}
				else if( operationEnd == 'I' )
				{		
					toAddEndQuery++;

					q_end_temp++;
				}
				else if( operationEnd == 'D' )
				{	
					toAddEndRef++;
					r_end_temp++;
				}

				edit_distance_total_R = edit_distance_R;
				edit_distance_updated = edit_distance_temp + edit_distance_R + edit_distance_total_L;

				minLen = min( q_end_temp - q_start_temp, r_end_temp - r_start_temp );
				if( rec == qec && edit_distance_updated/minLen <= sw.t)
				{
					re = r_end_temp;
					qe = q_end_temp;
				}
			}
			else break;
			
		}	
		else if( alt_ext == 3 )
		{
			if (  edit_distance_temp + edit_distance_R + edit_distance_total_L <= edit_distance_temp + edit_distance_total_R + edit_distance_L && (edit_distance_temp + edit_distance_R + edit_distance_total_L ) / ( minLen+1 ) <= sw .t + sw.s) //extend right
			{
				if( operationEnd == 'S' )
				{
					q_end_temp++;
					r_end_temp++;
					toAddEndQuery++;
					toAddEndRef++;
				}
				else if( operationEnd == 'I' )
				{		
					toAddEndQuery++;

					q_end_temp++;
				}
				else if( operationEnd == 'D' )
				{	
					toAddEndRef++;
					r_end_temp++;
				}

				edit_distance_total_R = edit_distance_R;
				edit_distance_updated = edit_distance_temp + edit_distance_R + edit_distance_total_L;

				minLen = min( q_end_temp - q_start_temp, r_end_temp - r_start_temp );
				if( rec == qec && edit_distance_updated/minLen <= sw.t)
				{
					re = r_end_temp;
					qe = q_end_temp;
				}
			}
			else if(  edit_distance_temp + edit_distance_total_R + edit_distance_L <= edit_distance_temp + edit_distance_R + edit_distance_total_L  && (edit_distance_temp + edit_distance_total_R + edit_distance_L ) / ( minLen+1 ) <= sw.t + sw.s ) //extend left
			{
				if( operationStart == 'S' )
				{
					q_start_temp--;
					r_start_temp--;
					toAddStartQuery++;
					toAddStartRef++;
					
				}
				else if( operationStart == 'I' )
				{
					toAddStartQuery++;
					q_start_temp--;
				}

				else if( operationStart == 'D' )
				{
					toAddStartRef++;
					r_start_temp--;
				}

			
				edit_distance_total_L = edit_distance_L;
				edit_distance_updated = edit_distance_temp + edit_distance_total_R + edit_distance_L;

				minLen = min( q_end_temp - q_start_temp, r_end_temp - r_start_temp );
				if( rsc == qsc && ( edit_distance_updated / minLen ) <= sw.t  )
				{
			
					rs = r_start_temp;
					qs = q_start_temp;
				}
				
			}
			else break;
		}


	}

	if( qs < 0 )
		*q_start = 0;
	else *q_start = qs;
		
	if( rs < 0 )
		*r_start = 0;
	else *r_start = rs;

	*edit_distance = edit_distance_updated;
	
	if( qe > strlen( ( char* ) yInput ) )
		*q_end =  strlen( ( char* ) yInput );
	else *q_end = qe;

	if( re >  strlen( ( char* ) xInput ) )
		*r_end = strlen( ( char* ) xInput );
	else *r_end = re;
	
return 0;
}


