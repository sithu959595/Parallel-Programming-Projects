import os
for t in[1,2,4,6,8]:
	print "NUMT = %d" % t
	for s in [ 2, 4, 8, 16, 32 ]:
		print "NUMS = %d" % s
		cmd = "g++ -DNUMS=%d -DNUMT=%d Project1.cpp -o prog -lm -fopenmp" % ( s, t )
		os.system( cmd )
		cmd = "./prog"
		os.system( cmd )
