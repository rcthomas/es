// 
// File    : ES_Synow_MOATSource.cl
// --------------------------------
//
// Copyright (C) 2010 Rollin C. Thomas <rcthomas@lbl.gov>
// 
// This file is part of ES.
// 
// ES is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your
// option) any later version.
// 
// ES is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
// for more details.
// 
// You should have received a copy of the GNU General Public License along
// with ES.  If not, see <http://www.gnu.org/licenses/>.
//  

__kernel void source
(
    ulong       const           iw,
    ulong       const           v_size,
    ulong       const           mu_size,
    ulong       const           wl_size,
    __global    const   float*  v,
    __local             float*  vbuf,
    __global    const   float*  mu,
    __global    const   float*  dmu,
    __global    const   float*  shift,
    __global    const   float*  wl,
    __local             float*  wlbuf,
    __global    const   float*  tau,
    __global    const   ulong*  start,
    __global            float*  in,
    __local             float*  inbuf,
    __global            float*  src
)
{

    size_t  j  = get_global_id ( 0 );
    size_t  i  = iw * v_size * mu_size * 2 + j;
    size_t  iv = get_group_id  ( 0 );
    size_t  im = get_local_id  ( 0 );
    size_t  group_size = get_local_size( 0 );

    /* Read constants in coalesced fashion */
    
    size_t v_chunk = (size_t) (v_size / group_size);
    size_t v_rem = v_size - (v_chunk * group_size);
    for (size_t k = 0; k < v_chunk; k++) {
      vbuf[k * group_size + im] = v[k * group_size + im];
      barrier(CLK_LOCAL_MEM_FENCE);
    }
    
    if (im < v_rem) {
      vbuf[v_chunk * group_size + im] = v[v_chunk * group_size + im];
    }
    barrier(CLK_LOCAL_MEM_FENCE);
    
    size_t wl_chunk = (size_t) (wl_size / group_size);
    size_t wl_rem = wl_size - (wl_chunk * group_size);
    for (size_t k = 0; k < wl_chunk; k++) {
      wlbuf[k * group_size + im] = wl[k * group_size + im];
      barrier(CLK_LOCAL_MEM_FENCE);
    }
    
    if (im < wl_rem) {
      wlbuf[wl_chunk * group_size + im] = wl[wl_chunk * group_size + im];
    }
    barrier(CLK_LOCAL_MEM_FENCE);
    
    float mymu = mu[ j ];
    barrier(CLK_LOCAL_MEM_FENCE);
    
    float mydmu = dmu[ iv * mu_size * 2 + im ];
    barrier(CLK_LOCAL_MEM_FENCE);
    
    ulong mystart = start[ i ];
    barrier(CLK_LOCAL_MEM_FENCE);
    
    inbuf[im] = in[ i ];
    barrier(CLK_LOCAL_MEM_FENCE);

    /* 
      perform the calculation.  we still have un-coalesced global
      memory access to the src, and tau vectors...
    */
    
    float   vv     = vbuf[ iv ];
    float   v_phot = vbuf[ 0 ];
    float   v_step = vbuf[ 1 ] - vbuf[ 0 ];
    
    size_t chunk = (size_t) (v_size / group_size);
    size_t rem = v_size - (chunk * group_size);
    
    for( size_t ib = mystart; ib < iw; ib ++ )
    {
        float  dd = ( wlbuf[ iw ] / wlbuf[ ib ] - 1.0 ) * 299.792;
        float  vd = sqrt( vv * vv + dd * dd - 2.0 * vv * dd * mymu );
        size_t il = (size_t)( ( vd - v_phot ) / v_step );
        float  cu = ( vd - vbuf[ il ] ) / v_step;
        float  cl = 1.0 - cu;
        size_t ii = ib * v_size + il;
        
        float  et = cl * tau[ ii ] + cu * tau[ ii + 1 ];
        float  ss = cl * src[ ii ] + cu * src[ ii + 1 ];
        
        et = exp( - et );
        inbuf[ im ] = inbuf[ im ] * et + ( 1.0 - et ) * ss;
    }
    
    
    /* coalesced update of the input vector */
    barrier(CLK_LOCAL_MEM_FENCE);
    in[i] = inbuf[im];
    
    
    inbuf[im] *= mydmu;    
    
    mem_fence( CLK_GLOBAL_MEM_FENCE );
    
    if( im == 0 )
    {
      size_t offset = iw * v_size + iv;
      float srcbuf = src[ offset ];
      
      for( size_t jm = 0; jm < mu_size * 2; jm ++ )
      {
        srcbuf += inbuf[jm];
      }
      srcbuf *= 0.5;
      
      src[ offset ] = srcbuf;
      
    }
    
    mem_fence( CLK_GLOBAL_MEM_FENCE );

    return;
    
}

