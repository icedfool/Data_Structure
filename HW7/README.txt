HOMEWORK 7: HALLOWEEN COSTUME MAPS


NAME:  < Grey Ding >


COLLABORATORS AND OTHER RESOURCES:
List the names of everyone you talked to about this assignment
(classmates, TAs, ALAC tutors, upperclassmen, students/instructor via
the forum, etc.), and all of the resources (books, online reference
material, etc.) you consulted in completing this assignment.


mentor:YiZhou Guo,Shelly,Sutherland ,Connor,Jason
classmates:Qi Jin; Dong Lin;WeiJun Li;ZiJian Zhong;ZeYu Ren;YiFan Wang;
ChenHao Tan; JiaWei Zhao
cplusplus.com

Remember: Your implementation for this assignment must be done on your
own, as described in "Collaboration Policy and Academic Integrity".



ESTIMATE OF # OF HOURS SPENT ON THIS ASSIGNMENT:  < 10-15 >



ORDER NOTATION ANALYSIS (ORIGINAL VERSION W/MAPS):
of each of the costume shop operations (please be concise!)
Let n be the number of different costumes in the shop,
m be the maximum number of copies of a given costume, and
c be the number of customers who visit the shop.

a (add):
worst:O(logn),best:O(1),since this function need to 
call for map.find(),and the big O of find is O(logN).

r (rent):
worst:O(logn+logc+m)
the worst case is that the shop doesn't has that costume and 
the customer never rent a costume before,then the two find function 
will both reach the end of two maps(costume_shop and people).

l (look up):
worst:O(log n + m)
the worst case is that all the costume is send out, so the Big O would 
be O(logn), and since they need to find the number of the copies, the Big O 
also need to find m more times.

p (print):
worst:O(c)
the worst case is that the name of the customer is at the last postion 
of the list, so the big O of finding that name is O(c).



EXTRA CREDIT (W/O MAPS)
What alternate data structures did you use?  What is the order
notation of each operation without maps?  What is the expected
performance difference?  Summarize the results of your testing.



MISC. COMMENTS TO GRADER:  
(optional, please be concise!)


