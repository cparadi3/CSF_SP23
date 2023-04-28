Names: CJ Paradis and Joe Zahner

In our implementation we determined the critical sections of our code to be those which contained sets of data that could
be modified by the concurrent processes. These sections were: the room map in server, the user set in Room, and the deque of 
messages in message queue. In both server and room, we used a mutex object to lock the data to keep it safe from other threads.
In Server, we used the guard object, which locked the data structure to outside threads,to protect our room map while it was being 
accessed by a processes in order to ensure that no data was lost when removing or adding rooms. In Room, we used a similar approach 
by using the guard object to lock the set of users so that the set could be safely modified without another thread encroaching on the 
processes being carried out by the current thread. In Message queue, we used both a mutex and a semaphore to lock and protect the 
message deque. We used a semaphore in this file because it acts as a flag to tell each thread when it is time to receive a message, 
and in doing this it allowed us to limit access to the data so no data races occured. 

In each of our critical sections, the mutex ensures that no data races will occur because it locks access to the data so that
only the current thread may access it. Thus, eliminiating the possibility of a data race. The semaphore that we use in message queue
helps to prevent dead locks by sending a signal to each thread when a message is ready to be read, this allows the thread to access
the data as soon as it's ready, and prevent all the threads from waiting for a message when it can be doing something else. 

