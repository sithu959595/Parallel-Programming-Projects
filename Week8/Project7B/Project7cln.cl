kernel
void
AutoCorrelate( global const float *dA, global float *dC )
{
	int Size = get_global_size( 0 );	// the dA size is actually twice this big
        int gid  = get_global_id( 0 );
	int shift = gid;

	float sum = 0.;
	for( int i = 0; i < Size; i++ )
	{
		sum += dA[i] * dA[i + shift];
	}
	dC[shift] = sum;
}
