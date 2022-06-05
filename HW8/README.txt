HOMEWORK 8: QUAD TREES AND TREE ITERATION


NAME:  <Grey Ding>


COLLABORATORS AND OTHER RESOURCES:
List the names of everyone you talked to about this assignment
(classmates, TAs, ALAC tutors, upperclassmen, students/instructor via
LMS, etc.), and all of the resources (books, online reference
material, etc.) you consulted in completing this assignment.

mentor:YiZhou Guo,Shelly,Sutherland ,Connor,Jason
classmates:Qi Jin; Dong Lin;WeiJun Li;ZiJian Zhong;ZeYu Ren;YiFan Wang;
ChenHao Tan; JiaWei Zhao
cplusplus.com

https://blog.csdn.net/gzylongxingtianxia/article/details/80077431
https://zh.wikipedia.org/wiki/%E5%9B%9B%E5%8F%89%E6%A0%91
https://en.wikipedia.org/wiki/Quadtree
https://codereview.stackexchange.com/questions/143955/quadtree-c-implementation

Remember: Your implementation for this assignment must be done on your
own, as described in "Collaboration Policy and Academic Integrity".



ESTIMATE OF # OF HOURS SPENT ON THIS ASSIGNMENT:  < around 25 >



ORDER NOTATION ANALYSIS: 
Give the big O notation of each of the QuadTree operations and justify
your answer for the non trivial operations (please be concise!)
Analyze both the running time and the additional memory usage needed
(beyond the space allocated for the existing tree structure).  You may
assume that the tree is reasonably well balanced for all operations.


n = the number of elements in the tree


size()
  running time: O(1)
  memory usage: O(1)

insert()
  running time:O(log4N)
  memory usage:O(1)

find()
  running time:O(log4N)
  memory usage:O(1)

height()
  running time:O(log4N)
  memory usage:O(1)

begin()
  running time:O(1)
  memory usage:O(1)

end()
  running time:O(1)
  memory usage:O(1)

bf_begin()
  running time:O(1)
  memory usage:O(1)

bf_end()
  running time:O(1)
  memory usage:O(1)

operator++()
  running time:O(log4N)
  memory usage:O(n)

operator*()
  running time:O(1)
  memory usage:O(1)

getLabel()
  running time:O(1)
  memory usage:O(1)

getDepth()
  running time:O(log4N)
  memory usage:O(1)

copy constructor
  running time:
  memory usage:

assignment operator
  running time:
  memory usage:

destructor
  running time:O(log4N)
  memory usage:O(1)



EXTRA CREDIT:  TREE BALANCING
How does the point insertion order affect the shape of the resulting
QuadTree object? What are specific examples of the worst case and best
case?  Discuss your stratgy for reordering the input to rebalance the
tree.  Paste example output with and without your rebalancing.






MISC. COMMENTS TO GRADER:  
(optional, please be concise!)


