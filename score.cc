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
#include <cstdlib>
#include <stdio.h>
#include <math.h> 
#include <stdlib.h>
#include <vector>
#include <string.h>
#include <sys/time.h>
#include <algorithm>
#include <seqan/find.h>
#include "cnef.h"


using namespace seqan;
using namespace std;


double scoring( MimOcc mim, unsigned char * ref, unsigned char * query )
{

	unsigned int ref_mim_len = mim.endRef- mim.startRef + 1;

	unsigned char * ref_mim = ( unsigned char * ) calloc ( (  ref_mim_len + 1 ) , sizeof( unsigned char ) );
	memcpy( &ref_mim[0], &ref[mim.startRef], ref_mim_len );
	ref_mim[   ref_mim_len ] = '\0';

	unsigned char * new_ref_mim = ( unsigned char * ) calloc ( (  ref_mim_len + 1 ) , sizeof( unsigned char ) );
	new_ref_mim[   ref_mim_len ] = '\0';

	unsigned int ref_occ = 0;
	memcpy( &new_ref_mim[0], &ref_mim[0], ref_mim_len );
	new_ref_mim[ ref_mim_len ] ='\0';
	search( ref, new_ref_mim, &ref_occ );

	unsigned int ref_pref_occ = 0;
	memcpy( &new_ref_mim[0], &ref_mim[0], ref_mim_len - 1 );
	new_ref_mim[ ref_mim_len - 1 ] ='\0';
	search( ref, new_ref_mim, &ref_pref_occ );
	
	unsigned int ref_suff_occ = 0;
	memcpy( &new_ref_mim[0], &ref_mim[1], ref_mim_len - 1 );
	new_ref_mim[ ref_mim_len - 1 ] ='\0';
	search( ref, new_ref_mim, &ref_suff_occ );

	unsigned int ref_inf_occ = 0;
	memcpy( &new_ref_mim[0], &ref_mim[1], ref_mim_len - 2 );
	new_ref_mim[ ref_mim_len - 2 ] = '\0';
	search( ref, new_ref_mim, &ref_inf_occ );
		
	double sc = 0;
	if( ref_inf_occ == ref_pref_occ == ref_suff_occ == ref_occ == 1 )
		sc = 0;
	else sc = ( ref_suff_occ * ref_pref_occ ) / ref_inf_occ;
	double ref_score = ( ref_occ - sc ) / max( sqrt( sc*1.0 ), 1*1.0 );

	free( ref_mim );
	free( new_ref_mim );
	
	unsigned int query_len = strlen( ( char* ) query );
	unsigned int query_mim_len = mim.endQuery- mim.startQuery + 1;
		
	unsigned char * query_mim = ( unsigned char * ) calloc ( (  query_mim_len + 1 ) , sizeof( unsigned char ) );
	memcpy( &query_mim[0], &ref[mim.startQuery], query_mim_len );
	query_mim[  query_mim_len ] = '\0';

	unsigned char * new_query_mim = ( unsigned char * ) calloc ( (  query_mim_len + 1 ) , sizeof( unsigned char ) );
	new_query_mim[  query_mim_len ] = '\0';

	unsigned int query_occ = 0;
	memcpy( &new_query_mim[0], &query_mim[0], query_mim_len );
	new_query_mim[ query_mim_len ] ='\0';
	search( query, new_query_mim, &query_occ );

	unsigned int query_pref_occ = 0;
	memcpy( &new_query_mim[0], &query_mim[0], query_mim_len - 1 );
	new_query_mim[ query_mim_len - 1 ] ='\0';
	search( query, new_query_mim, &query_pref_occ );
	
	unsigned int query_suff_occ = 0;
	memcpy( &new_query_mim[0], &query_mim[1], query_mim_len - 1 );
	new_query_mim[ query_mim_len - 1 ] ='\0';
	search( query, new_query_mim, &query_suff_occ );

	unsigned int query_inf_occ = 0;
	memcpy( &new_query_mim[0], &query_mim[1], query_mim_len - 2 );
	new_query_mim[ query_mim_len - 2 ] = '\0';
	search( query, new_query_mim, &query_inf_occ );
	
	free( query_mim );
	free( new_query_mim );

	double sc2 = 0;
	if( query_inf_occ == query_pref_occ == query_suff_occ == query_occ == 1 )
		sc2 = 0;
	else sc2 = ( query_suff_occ * query_pref_occ ) / query_inf_occ;
	double query_score = ( query_occ - sc2 ) / max( sqrt( sc2*1.0 ), 1*1.0 );

return ( ref_score + query_score ) / 2.0;	
}



unsigned int search( unsigned char * text, unsigned char * patt, unsigned int * score  )
{

	CharString haystack = text;
	CharString needle = patt;

	unsigned int occ = 0;

	Finder<CharString> finder(haystack);
    	Pattern<CharString, Simple> pattern(needle);

    	while (find(finder, pattern))
        	occ++;

	*score = occ;

return 1;
}

