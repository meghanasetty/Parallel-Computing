Question: Compare performance at 16 threads across the different synchronization modes. Why are the
speedup this way?

Observation:
	1. Speed of Thread based sync is higher than the chunk based as the access of shared variable is less in thread based sync.
	2. The speed of thread is increasing and when n=granularity, both thread and chunk speed is equal because at that point the access to shared varaiable will be same for both.


Question: For thread level synchronization, compare the performance at 16 threads of different n and
intensity. Why are the plots this way?

Observation:
	For a higher n values and lower intensity value, as the granularity increases the speed is increasing linearly.
	1. when there is very low granualarity, the lock we applied for the calculating the begin and end values of the for loop is taking a lot of time(because of the lock waiting and releasing).
	2. When the intersity is high, the time taken by the compute function is high, so for small values of n(n<intensity), the speedup doesnot increase.
