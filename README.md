# MultithreadingSynchronization
This is a solution to a interesting synchonization problem, intend to demo Multi-threading and Synchronization.

Problem: A group of surfers decide to spend an entire day surfing in the
Beach. The surfers show up at random times during the day and come in and out of the water at random times.
A local fisherman warns the surfers that there are sharks in the water, and advices them to
never go in the water alone. These sharks are not naturally aggressive and will stay away if
they see a group of surfers. However, if they see just one surfer alone they may get confused
thinking that the surfboard is actually a big fish and attack the lonely surfer. 

Thus, to avoids shark attacks, the program have to make sure that no surfer is ever left alone in the water
Yet, all surfers must surf, (nostarvation), and all surfers must leave (no deadlock). At dusk, no more surfers 
come and all surfers still at the beach leave.
