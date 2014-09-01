.. image:: https://travis-ci.org/willemt/heap.png
   :target: https://travis-ci.org/willemt/heap

What?
-----
A queue that uses an array instead of a linked list.

Benefits of using an array:

* helps prevent unnecessary malloc/free calls you'll usually find in a linked list implementation
* reduces the likelihood of cache misses
