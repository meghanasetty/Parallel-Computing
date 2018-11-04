Question:Why do you think some measurements are so erratic?

Observation: 
	For iteration/thread level, the speedup of threads(~6) were almost equal to that sqequential becasue of the overhead of creation of the threads.
	Comparing the thread and iteration, the iteration was taking a  lot of time when intensity is 1. It is becasue of the continuous access of the 'sum' varaible which is in mutex.
	As the intensity increases, both the iteration and thread speedup is almost same. This is because of the intensity parameter. As the intensity increases, the work needed for executing the f1,f2,f3,f4 will overide the work need for the mutexes.
	When n is very large, the thread is more speed than iteration because we have very less access to 'sum' mutex varaible for the thread.


Question: Compare the speedup of iteration-level synchronization to thread-level synchronization. Why is
it that way?

Observation:
	Iteration level sync is slower than the thread level sync.
	As mentioned before, the iteration level sync is slower becasue the threads have to wait for the mutexed vairable 'sum' for each iteration in the for loop.


Question: Why is the speedup of low intensity runs with iteration-level synchronization the way it is?

Observation:
	the speed did not increase much with the addtion of threads for a low intesity - iteration level sync. This may be because, the work to execute f1,f2,f3,f4 with such intensity was so minimal, that it doesnot matter much.
