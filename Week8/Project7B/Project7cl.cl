kernel
void
ArrayMult( global const float *dA, global const int *dD, local float *prods, global float *dC )

{
        int gid = get_global_id( 0 );
        int numItems=get_local_size(0);
        int tnum=get_local_id(0);
        int wgNum = get_group_id( 0 );
        int shift=gid+(*dD); 
        prods[ tnum ] = dA[ gid ] * dA[shift];
     

for( int offset = 1; offset < numItems; offset *= 2 )
{
int mask = 2*offset - 1;
barrier( CLK_LOCAL_MEM_FENCE ); // wait for completion 
if( (tnum&mask)==0)
{
prods[ tnum ] += prods[ tnum + offset ]; 
}
}	
barrier( CLK_LOCAL_MEM_FENCE ); 
if( tnum == 0 )
dC[ wgNum ] = prods[ 0 ];   

}
